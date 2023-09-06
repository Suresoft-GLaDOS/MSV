// Copyright (C) 2016 Fan Long, Martin Rianrd and MIT CSAIL 
// Prophet
// 
// This file is part of Prophet.
// 
// Prophet is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Prophet is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Prophet.  If not, see <http://www.gnu.org/licenses/>.
#include "config.h"
#include "Utils.h"
#include "BenchProgram.h"
#include "RepairCandidateGenerator.h"
#include "ExprSynthesizer.h"
#include "ASTUtils.h"
#include "LocalAnalyzer.h"
#include "CodeRewrite.h"
#include "DuplicateDetector.h"
#include "FeatureParameter.h"
#include "ProfileWriter.h"

#include "llvm/Support/CommandLine.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/ASTContext.h"
#include <iostream>

using namespace clang;

#define TEST_CC "msv-testcc"
#define ISNEG_TMPFILE "/tmp/neg.tmp"
#define ISNEG_RECORDFILE "/tmp/neg.log"

#define BATCH_CAP 10
#define SYNC_CAP 200
#define SYNC_TIME_CAP 600
#define MAGIC_NUMBER -123456789

extern llvm::cl::opt<bool> ForCPP;

llvm::cl::opt<bool> CondExt("cond-ext", llvm::cl::init(false),
                            llvm::cl::desc("Enable condition extension"));
// XXX: OK, this is really shitty code for statistics
llvm::cl::opt<std::string> DumpPassedCandidate("dump-passed-candidate", llvm::cl::init(""),
        llvm::cl::desc("Dump the passed candidate, in order to collect experiments data!"));
size_t pass_global_cnt = 0;
size_t patch_explored = 0;

unsigned long tot_synthesis_run = 0;
unsigned long tot_concrete_conds = 0;
unsigned long tot_explored_templates = 0;

static unsigned long long the_timeout_limit = 0;

void dumpCandidate(SourceContextManager &M, const RepairCandidate &rc, Expr* v, double score) {
    std::ostringstream sout;
    sout << DumpPassedCandidate.getValue() << "-" << pass_global_cnt << ".txt";
    std::string fname = sout.str();
    std::ofstream fout(fname.c_str(), std::ofstream::out);
    fout << "Score: " << score << "\n";
    ASTContext *ctxt = M.getSourceContext(rc.actions[0].loc.filename);
    fout << "Var: " << stmtToString(*ctxt, v) << "\n";
    fout << rc.toString(M) << "\n";
    fout.close();
    pass_global_cnt ++;
}


static inline size_t locate_next(const std::string &s, size_t idx, int direction, std::string sub) {
    long long cur = idx;
    int cnt = 0;
    while (cur >= 0 && cur < (long long)s.size() && cnt >= 0) {
        cur = cur + direction;
        if (cur < 0) return std::string::npos;
        if (cur > (long long)s.size()) return std::string::npos;
        if (cnt == 0)
            if (s.substr(cur, sub.size()) == sub)
                return cur;
        if (s[cur] == '(')
            cnt += direction;
        if (s[cur] == ')')
            cnt -= direction;
    }
    return std::string::npos;
}

static inline size_t leftPos(size_t a, size_t b) {
    if (a != std::string::npos && ((a < b) || (b == std::string::npos)))
        return a;
    else
        return b;
}

static inline size_t rightPos(size_t a, size_t b) {
    if (a != std::string::npos && ((a > b) || (b == std::string::npos)))
        return a;
    else
        return b;
}
long checkV(const std::map<unsigned long, std::vector< std::vector< long long> > > &caseVMap,
    const std::set<unsigned long> &negative_cases, const std::set<unsigned long> &positive_cases,
    const std::map<unsigned long, std::vector<size_t> > &negative_records,
    size_t idx, long long v, int flag) {

    long ret = 0;

    if (false)
    for (std::set<unsigned long>::iterator it = positive_cases.begin();
            it != positive_cases.end(); ++it) {
        std::map<unsigned long, std::vector< std::vector< long long> > >::const_iterator cit;
        cit = caseVMap.find(*it);
        assert( cit != caseVMap.end());
        for (size_t i = 0; i < cit->second.size(); i++) {
            assert(cit->second[i][idx] != MAGIC_NUMBER && "sanity check!");
            int flag_mask = flag & 3;
            int flag_bit = flag & 4;
            long long rv = v;
            if (flag_bit != 0)
                rv = cit->second[i][v];
            bool cond;
            switch (flag_mask) {
                case 0:
                    cond = (cit->second[i][idx] != rv);
                    break;
                case 1:
                    cond = (cit->second[i][idx] == rv);
                    break;
                case 2:
                    cond = (cit->second[i][idx] < rv);
                    break;
                case 3:
                    cond = (cit->second[i][idx] > rv);
                    break;
                default:
                    break;
            }
            if (cond) {
                ret += 2;
                break;
            }
        }
    }

    bool valid = false;
    for (std::set<unsigned long>::iterator it = negative_cases.begin();
            it != negative_cases.end(); ++it) {
        std::map<unsigned long, std::vector< std::vector< long long> > >::const_iterator cit;
        cit = caseVMap.find(*it);
        // assert( cit != caseVMap.end());
        if (cit == caseVMap.end()) continue;
        std::map<unsigned long, std::vector<unsigned long> >::const_iterator nit;
        nit = negative_records.find(*it);
        if (cit->second.size() != nit->second.size()) {
            outlog_printf(1, "Seems some non-determinism! %lu v.s. %lu Length mismatch!\n",
                    (unsigned long)cit->second.size(), (unsigned long)nit->second.size());
            return -1;
        }
        for (size_t i = 0; i < cit->second.size(); i++) {
            assert(cit->second[i][idx] != MAGIC_NUMBER && "sanity check!");

            int flag_mask = flag & 3;
            int flag_bit = flag & 4;
            long long rv = v;
            if (flag_bit != 0) {
                rv = cit->second[i][v];
            }
            bool cond;
            switch (flag_mask) {
                case 0:
                    cond = (cit->second[i][idx] != rv);
                    break;
                case 1:
                    cond = (cit->second[i][idx] == rv);
                    break;
                case 2:
                    cond = (cit->second[i][idx] < rv);
                    break;
                case 3:
                    cond = (cit->second[i][idx] > rv);
                    break;
                default:
                    break;
            }

            if (cond)
                valid = true;
            if ((nit->second[i] == 0) && (cond)) {
                ret++;
                break;
            }
            if ((nit->second[i] == 1) && (!cond)) {
                ret++;
                break;
            }
        }
    }
    if (valid)
        return ret;
    else
        return -1;
}

struct SynResTy {
    size_t idx;
    long long v;
    size_t flag;
    SynResTy(): idx(), v(), flag() {}

    SynResTy(size_t idx, long long v, size_t flag):
        idx(idx), v(v), flag(flag) {}

    bool operator < (const SynResTy &a) const {
        if (flag != a.flag)
            return flag > a.flag;
        else if (idx != a.idx)
            return idx < a.idx;
        else
            return v < a.v;
    }
};

static int compareTypeCheck(Expr* E1, Expr* E2) {
    QualType T1 = E1->getType();
    QualType T2 = E2->getType();
    if (T1->isIntegerType() == T2->isIntegerType())
        return 0;
    if (T1->isPointerType() == T2->isPointerType())
        return 1;
    return -1;
}

std::vector<Expr*> synthesizeResult(ExprListTy exprs,
        const std::map<unsigned long, std::vector<unsigned long> > &negative_records,
        const std::map<unsigned long, std::vector< std::vector< long long> > > &caseVMap,
        const std::set<unsigned long> &negative_cases,
        const std::set<unsigned long> &positive_cases, ASTContext *ctxt) {
    std::vector<std::pair<size_t, SynResTy> > res;
    res.clear();
    // If just remove work, then we remove it
    /*bool no_pos_exec = true;
    for (std::set<unsigned long>::const_iterator it = positive_cases.begin();
            it != positive_cases.end(); ++it) {
        std::map<unsigned long, std::vector< std::vector<long long> > > ::const_iterator find_it;
        find_it = caseVMap.find(*it);
        if (find_it->second.size() != 0) {
            no_pos_exec = false;
            break;
        }
    }
    if (no_pos_exec)
        res.push_back(std::make_pair(0, SynResTy(0, 1, 2)));*/
    // Collect over all possible values for a variable
    for (size_t vid = 0; vid < exprs.size(); vid++) {
        std::set<long long> vals;
        vals.clear();
        bool give_up = false;
        for (std::map<unsigned long, std::vector< std::vector< long long> > >::const_iterator
                it = caseVMap.begin(); it != caseVMap.end(); ++it) {
            for (size_t i = 0; i < it->second.size(); i++) {
                if (it->second[i][vid] == MAGIC_NUMBER)
                    give_up = true;
                // This is to avoid catch those pointer values, they are not useful at all
                if ((it->second[i][vid] < 1000) && (it->second[i][vid] > -1000))
                    vals.insert(it->second[i][vid]);
            }
        }
        if (give_up) continue;
        for (std::set<long long>::iterator it = vals.begin(); it != vals.end(); it++) {
            size_t flag_e = 2;
            if (CondExt.getValue())
                if (exprs[vid]->getType()->isIntegerType())
                    flag_e = 4;
            for (size_t flag_v = 0; flag_v < flag_e; flag_v ++) {
                long long the_v = *it;
                if (flag_v == 2)
                    the_v ++;
                else if (flag_v == 3)
                    the_v --;
                long vio = checkV(caseVMap, negative_cases, positive_cases,
                        negative_records, vid, the_v, flag_v);
                if (vio >= 0)
                    res.push_back(std::make_pair((size_t)vio, SynResTy(vid, the_v, flag_v)));
            }
        }
        if (CondExt.getValue()) {
            for (size_t vid2 = 0; vid2 < exprs.size(); vid2 ++) {
                int checkType = compareTypeCheck(exprs[vid], exprs[vid2]);
                if (checkType == -1)
                    continue;
                bool give_up = false;
                for (std::map<unsigned long, std::vector< std::vector< long long> > >::const_iterator
                    it = caseVMap.begin(); it != caseVMap.end(); ++it) {
                    for (size_t i = 0; i < it->second.size(); i++) {
                        if (it->second[i][vid2] == MAGIC_NUMBER) {
                            give_up = true;
                            break;
                        }
                    }
                    if (give_up) break;
                }
                if (give_up)
                    continue;
                size_t flag_e = 8;
                if (checkType == 1)
                    flag_e = 6;
                for (size_t flag_v = 4; flag_v < flag_e; flag_v ++) {
                    long vio = checkV(caseVMap, negative_cases, positive_cases,
                            negative_records, vid, vid2, flag_v);
                    if (vio >= 0)
                        res.push_back(std::make_pair((size_t)vio, SynResTy(vid, vid2, flag_v)));
                }
            }
        }
    }

    tot_synthesis_run ++;
    tot_concrete_conds += res.size() + 1;

    std::sort(res.begin(), res.end());
    std::vector<Expr*> ret;
    ret.clear();
    ret.push_back(getNewIntegerLiteral(ctxt, 1));
    for (size_t i = 0; i < res.size(); i++)
        if (i < 20 || res[i].first == 0)
        {
            SynResTy &tmp = res[i].second;
            Expr *E;
            Expr *RHS;
            int flag_mask = (tmp.flag & 3);
            int flag_bit = (tmp.flag & 4);
            if (flag_bit == 0)
                RHS = getNewIntegerLiteral(ctxt, tmp.v);
            else
                RHS = exprs[tmp.v];
            BinaryOperatorKind bkind;
            switch (flag_mask) {
                case 0:
                    bkind = BO_NE;
                    break;
                case 1:
                    bkind = BO_EQ;
                    break;
                case 2:
                    bkind = BO_LE;
                    break;
                case 3:
                    bkind = BO_GT;
                    break;
                default:
                    assert(0);
            }
            E = BinaryOperator::Create(*ctxt,
                exprs[tmp.idx], RHS, bkind, ctxt->IntTy, VK_RValue,
                OK_Ordinary, SourceLocation(), FPOptionsOverride());
            if (bkind == BO_EQ)
                ret.push_back(getParenExpr(ctxt, E));
            else
                ret.push_back(E);
        }
        else
            break;

    return ret;
}

std::vector<Expr*> synthesizeResultSPR(ExprListTy exprs,
        const std::map<unsigned long, std::vector<size_t> > &negative_records,
        const std::map<unsigned long, std::vector< std::vector< long long> > > &caseVMap,
        const std::set<unsigned long> &negative_cases,
        const std::set<unsigned long> &positive_cases, ASTContext *ctxt) {
    std::vector<std::pair<size_t, SynResTy> > res;
    res.clear();
    // Collect over all possible values for a variable
    for (size_t vid = 0; vid < exprs.size(); vid++) {
        std::set<long long> vals;
        vals.clear();
        bool give_up = false;
        for (std::map<unsigned long, std::vector< std::vector< long long> > >::const_iterator
                it = caseVMap.begin(); it != caseVMap.end(); ++it) {
            for (size_t i = 0; i < it->second.size(); i++) {
                if (it->second[i][vid] == MAGIC_NUMBER)
                    give_up = true;
                // This is to avoid catch those pointer values, they are not useful at all
                if ((it->second[i][vid] < 1000) && (it->second[i][vid] > -1000))
                    vals.insert(it->second[i][vid]);
            }
        }
        if (give_up) continue;
        for (std::set<long long>::iterator it = vals.begin(); it != vals.end(); it++) {
            size_t flag_e = 2;
            if (CondExt.getValue())
                if (exprs[vid]->getType()->isIntegerType())
                    flag_e = 4;
            for (size_t flag_v = 0; flag_v < flag_e; flag_v ++) {
                long long the_v = *it;
                if (flag_v == 2)
                    the_v ++;
                else if (flag_v == 3)
                    the_v --;
                long vio = checkV(caseVMap, negative_cases, positive_cases,
                        negative_records, vid, the_v, flag_v);
                if (vio >= 0)
                    res.push_back(std::make_pair((size_t)vio, SynResTy(vid, the_v, flag_v)));
            }
        }
        if (CondExt.getValue()) {
            for (size_t vid2 = 0; vid2 < exprs.size(); vid2 ++) {
                int checkType = compareTypeCheck(exprs[vid], exprs[vid2]);
                if (checkType == -1)
                    continue;
                bool give_up = false;
                for (std::map<unsigned long, std::vector< std::vector< long long> > >::const_iterator
                    it = caseVMap.begin(); it != caseVMap.end(); ++it) {
                    for (size_t i = 0; i < it->second.size(); i++) {
                        if (it->second[i][vid2] == MAGIC_NUMBER) {
                            give_up = true;
                            break;
                        }
                    }
                    if (give_up) break;
                }
                if (give_up)
                    continue;
                size_t flag_e = 8;
                if (checkType == 1)
                    flag_e = 6;
                for (size_t flag_v = 4; flag_v < flag_e; flag_v ++) {
                    long vio = checkV(caseVMap, negative_cases, positive_cases,
                            negative_records, vid, vid2, flag_v);
                    if (vio >= 0)
                        res.push_back(std::make_pair((size_t)vio, SynResTy(vid, vid2, flag_v)));
                }
            }
        }
    }

    tot_synthesis_run ++;
    tot_concrete_conds += res.size() + 1;

    std::sort(res.begin(), res.end());
    std::vector<Expr*> ret;
    ret.clear();
    ret.push_back(getNewIntegerLiteral(ctxt, 1));
    for (size_t i = 0; i < res.size(); i++)
        if (i < 20 || res[i].first == 0)
        {
            SynResTy &tmp = res[i].second;
            Expr *E;
            if (tmp.v == 0 && (tmp.flag == 0 || tmp.flag == 1)) {
                Expr *RHS;
                int flag_mask = tmp.flag & 3;
                int flag_bit = tmp.flag & 4;
                if (flag_bit == 0)
                    RHS = getNewIntegerLiteral(ctxt, tmp.v);
                else
                    RHS = exprs[tmp.v];
                BinaryOperatorKind bkind;
                switch (flag_mask) {
                    case 0:
                        bkind = BO_NE;
                        break;
                    case 1:
                        bkind = BO_EQ;
                        break;
                    case 2:
                        bkind = BO_LE;
                        break;
                    case 3:
                        bkind = BO_GT;
                        break;
                    default:
                        assert(0);
                }
                E = BinaryOperator::Create(*ctxt,
                    exprs[tmp.idx], RHS, bkind, ctxt->IntTy, VK_RValue,
                    OK_Ordinary, SourceLocation(), FPOptionsOverride());
                ret.push_back(E);
            }
        }
        else
            break;
    for (size_t i = 0; i < res.size(); i++)
        if (i < 20 || res[i].first == 0)
        {
            SynResTy &tmp = res[i].second;
            Expr *E;
            if (tmp.v != 0 || (tmp.flag != 0 && tmp.flag != 1)) {
                Expr *RHS;
                int flag_mask = tmp.flag & 3;
                int flag_bit = tmp.flag & 4;
                if (flag_bit == 0)
                    RHS = getNewIntegerLiteral(ctxt, tmp.v);
                else
                    RHS = exprs[tmp.v];
                BinaryOperatorKind bkind;
                switch (flag_mask) {
                    case 0:
                        bkind = BO_NE;
                        break;
                    case 1:
                        bkind = BO_EQ;
                        break;
                    case 2:
                        bkind = BO_LE;
                        break;
                    case 3:
                        bkind = BO_GT;
                        break;
                    default:
                        assert(0);
                }
                E = BinaryOperator::Create(*ctxt,
                    exprs[tmp.idx], RHS, bkind, ctxt->IntTy, VK_RValue,
                    OK_Ordinary, SourceLocation(), FPOptionsOverride());
                ret.push_back(E);
            }
        }
        else
            break;

    return ret;
}


bool isZeroConstantExpr(Expr *E) {
    BinaryOperator *BO = llvm::dyn_cast<BinaryOperator>(E);
    if (!BO) return false;
    IntegerLiteral *IL = llvm::dyn_cast<IntegerLiteral>(BO->getRHS());
    if (!IL) return false;
    return (IL->getValue() == 0);
}


std::set<std::string> replaceIsNegWithClause(const std::string &code) {
    size_t idx = code.find("\n");
    idx = code.find(IS_NEG_HANDLER, idx);
    assert( idx != std::string::npos );

    size_t cnt = 1;
    size_t idx1 = idx + 9;
    while (cnt != 0) {
        if (code[idx1] == '(') cnt ++;
        if (code[idx1] == ')') cnt --;
        idx1 ++;
    }

    std::set<std::string> ret;
    ret.clear();

    // We always try 0 and 1
    ret.insert(code.substr(0, idx) + "(1)" + code.substr(idx1));
    ret.insert(code.substr(0, idx) + "(0)" + code.substr(idx1));

    // We are going to locate the left and right hand
    size_t left = locate_next(code, idx, -1, "(");
    left = locate_next(code, left, -1, "(");
    size_t right = locate_next(code, idx, 1, ")");
    right = locate_next(code, right, 1, ")");
    assert( left != std::string::npos);
    assert( right != std::string::npos);
    assert( right >= left);
    std::string s = code.substr(left, right - left + 1);
    //llvm::errs() << "The big clause: " << s << "\n";
    size_t cur_pos = 0;
    while ((s.find("&&", cur_pos) != std::string::npos) || (s.find("||", cur_pos) != std::string::npos)) {
        size_t pos1 = s.find("&&", cur_pos);
        size_t pos2 = s.find("||", cur_pos);
        if ((pos1 != std::string::npos) && ((pos1 < pos2) || (pos2 == std::string::npos)))
            cur_pos = pos1;
        else
            cur_pos = pos2;
        //llvm::errs() << s << " " << cur_pos << " " << s[cur_pos] << "\n";
        size_t sleft = locate_next(s, cur_pos, -1, "(");
        //llvm::errs() << sleft << " " << std::string::npos << "\n";
        sleft = rightPos(sleft, locate_next(s, cur_pos, -1, "&&"));
        sleft = rightPos(sleft, locate_next(s, cur_pos, -1, "||"));
        assert(sleft != std::string::npos);
        assert(sleft <= cur_pos);
        std::string subs;
        if (s[sleft] == '(')
            subs = s.substr(sleft + 1, cur_pos - sleft - 1);
        else
            subs = s.substr(sleft + 2, cur_pos - sleft - 2);
        std::string res = "";

        if (subs.find(IS_NEG_HANDLER) == std::string::npos) {
            //llvm::errs() << "Found a clause: " << subs << "\n";
            res = code.substr(0, idx) + "(" + subs + ")" + code.substr(idx1);
            //llvm::errs() << res.substr(idx - 200, idx1 - idx + 300) << "\n";
            ret.insert(res);
            res = code.substr(0, idx) + "(!" + "(" + subs + "))" + code.substr(idx1);
            //llvm::errs() << res.substr(idx - 200, idx1 - idx + 300) << "\n";
            ret.insert(res);
        }

        size_t sright = locate_next(s, cur_pos, 1, ")");
        sright = leftPos(sright, locate_next(s, cur_pos, 1, "&&"));
        sright = leftPos(sright, locate_next(s, cur_pos, 1, "||"));
        assert(sright != std::string::npos);
        assert(sright >= cur_pos);

        subs = s.substr(cur_pos + 2, sright - cur_pos - 2);

        if (subs.find(IS_NEG_HANDLER) == std::string::npos) {
            //llvm::errs() << "Found a clause: " << subs << "\n";
            res = code.substr(0, idx) + "(" + subs + ")" + code.substr(idx1);
            //llvm::errs() << res.substr(idx - 200, idx1 - idx + 300) << "\n";
            ret.insert(res);
            res = code.substr(0, idx) + "(!" + "(" + subs + "))" + code.substr(idx1);
            //llvm::errs() << res.substr(idx - 200, idx1 - idx + 300) << "\n";
            ret.insert(res);
        }
        cur_pos++;
    }
    return ret;
}

namespace {

double found_score = 0;

/*LocalAnalyzer* getLocalAnalyzer(SourceContextManager &M, const RepairCandidate &rc, size_t idx) {
    std::map<Stmt*, size_t> tmp_map;
    tmp_map.clear();
    for (size_t i = 0; i < idx; i++)
        if (rc.actions[i].kind != RepairAction::ExprMutationKind)
            tmp_map[(Stmt*)rc.actions[i].ast_node] = i;
    ASTLocTy cur_loc = rc.actions[idx].loc;
    // There is a corner case where the loc inserts the statement
    // as thee same as the location indication stmt
    Stmt* last_stmt = NULL;
    while (tmp_map.count(cur_loc.stmt) != 0) {
        cur_loc = rc.actions[tmp_map[cur_loc.stmt]].loc;
        if (cur_loc.stmt == last_stmt) break;
        last_stmt = cur_loc.stmt;
    }
    return M.getLocalAnalyzer(cur_loc);
}*/
Expr *createNullPointerLiteral(ASTContext *ctxt){
    IntegerLiteral *literal=IntegerLiteral::Create(*ctxt,llvm::APInt(32,0),ctxt->IntTy,SourceLocation());
    ImplicitCastExpr *cast=ImplicitCastExpr::Create(*ctxt,ctxt->getPointerType(ctxt->IntTy),
            CastKind::CK_NullToPointer,literal,nullptr,VK_LValue);
    return cast;
}
Expr* createConditionVarList(ASTContext *ctxt,std::vector<Expr *> exprs,QualType type){
    InitListExpr *list=new(*ctxt) InitListExpr(*ctxt,SourceLocation(),llvm::ArrayRef<Expr *>(exprs),SourceLocation());
    QualType cast_type=type;
    if (ForCPP){
        cast_type.addConst();
        cast_type=ctxt->getIncompleteArrayType(cast_type,ArrayType::ArraySizeModifier::Normal,3);
    }
    else{
        cast_type=ctxt->getPointerType(cast_type);
    }

    CompoundLiteralExpr *compound=new(*ctxt) CompoundLiteralExpr(SourceLocation(),ctxt->getTrivialTypeSourceInfo(cast_type),
            cast_type,VK_LValue,list,false);

    ImplicitCastExpr *cast=ImplicitCastExpr::Create(*ctxt,cast_type,CastKind::CK_ArrayToPointerDecay,
            compound,nullptr,VK_LValue);
    
    return cast;
}
Expr* createConditionVarNameList(ASTContext *ctxt,std::vector<std::string> names){
    std::vector<Expr *> exprs;
    exprs.clear();
    for (size_t i=0;i<names.size();i++){
        StringLiteral *str = StringLiteral::Create(*ctxt, names[i], StringLiteral::Ascii,
                false, ctxt->getConstantArrayType(ctxt->CharTy, llvm::APInt(32, names[i].size() + 1), nullptr,ArrayType::Normal, 0),
                SourceLocation());
        ImplicitCastExpr *ICE1 = ImplicitCastExpr::Create(*ctxt, ctxt->getPointerType(ctxt->CharTy),
                CK_ArrayToPointerDecay, str, 0, VK_RValue);
        exprs.push_back(ICE1);
    }

    return createConditionVarList(ctxt,exprs,ctxt->getPointerType(ctxt->CharTy));
}


Expr* createAbstractConditionExpr(SourceContextManager &M, const RepairAction &action) {
    ASTContext *ctxt = M.getSourceContext(action.loc.filename);
    std::vector<Expr*> tmp_argv;
    const ExprListTy &exprs = action.candidate_atoms;
    CallExpr *placeholder=(CallExpr*)action.ast_node;
    tmp_argv.clear();
    // tmp_argv.push_back(placeholder->getArgs()[0]);
    // tmp_argv.push_back(getNewIntegerLiteral(ctxt, exprs.size()));
    std::vector<std::string> intVarName;
    std::vector<std::string> charVarName;
    std::vector<std::string> pointerVarName;
    std::vector<std::string> doubleVarName;
    std::vector<Expr *> intVar;
    std::vector<Expr *> charVar;
    std::vector<Expr *> pointerVar;
    std::vector<Expr *> doubleVar;
    std::string location;
    intVarName.clear();
    charVarName.clear();
    pointerVarName.clear();
    doubleVarName.clear();
    intVar.clear();
    charVar.clear();
    pointerVar.clear();
    doubleVar.clear();
    location="";

    for (size_t i = 0; i < exprs.size(); ++i) {
        // ParenExpr *ParenE1 = new (*ctxt) ParenExpr(SourceLocation(), SourceLocation(), exprs[i]);
        // UnaryOperator *AddrsOf = 
        //     UnaryOperator::Create(*ctxt,ParenE1, UO_AddrOf, ctxt->getPointerType(exprs[i]->getType()),
        //             VK_RValue, OK_Ordinary, SourceLocation(),false,FPOptionsOverride());
        // tmp_argv.push_back(AddrsOf);
        // ParenExpr *ParenE2 = new (*ctxt) ParenExpr(SourceLocation(), SourceLocation(), exprs[i]);
        // UnaryExprOrTypeTraitExpr *SizeofE = new (*ctxt) UnaryExprOrTypeTraitExpr(
        //     UETT_SizeOf, ParenE2, ctxt->UnsignedLongTy, SourceLocation(), SourceLocation());
        // tmp_argv.push_back(SizeofE);

        DeclRefExpr *decl=llvm::dyn_cast<DeclRefExpr>(exprs[i]);
        if (decl){
            VarDecl *var=llvm::dyn_cast<VarDecl>(decl->getDecl());
            if (var){
                std::string name=var->getDeclName().getAsString();
                // varName.push_back(name);
                QualType type=var->getType();
                if (ctxt->hasSameType(type,ctxt->IntTy)){
                    intVar.push_back(exprs[i]);
                    intVarName.push_back(name);
                }
                else if (ctxt->hasSameType(type,ctxt->CharTy)){
                    charVar.push_back(exprs[i]);
                    charVarName.push_back(name);
                }
                else if (type->isPointerType()){
                    pointerVar.push_back(exprs[i]);
                    pointerVarName.push_back(name);
                }
                else if (ctxt->hasSameType(type,ctxt->DoubleTy)){
                    doubleVar.push_back(exprs[i]);
                    doubleVarName.push_back(name);
                }
            }
        }
    }

    StringLiteral *str = StringLiteral::Create(*ctxt, location, StringLiteral::Ascii,
            false, ctxt->getConstantArrayType(ctxt->CharTy, llvm::APInt(32, location.size() + 1), nullptr,ArrayType::Normal, 0),
            SourceLocation());
    tmp_argv.push_back(str);

    IntegerLiteral *size;
    size=IntegerLiteral::Create(*ctxt,llvm::APInt(32,intVar.size()),ctxt->IntTy,SourceLocation());
    tmp_argv.push_back(size);
    if (intVar.size()==0) tmp_argv.push_back(createNullPointerLiteral(ctxt));
    else tmp_argv.push_back(createConditionVarList(ctxt,intVar,ctxt->IntTy));

    size=IntegerLiteral::Create(*ctxt,llvm::APInt(32,charVar.size()),ctxt->IntTy,SourceLocation());
    tmp_argv.push_back(size);
    if (charVar.size()==0) tmp_argv.push_back(createNullPointerLiteral(ctxt));
    else tmp_argv.push_back(createConditionVarList(ctxt,charVar,ctxt->CharTy));

    size=IntegerLiteral::Create(*ctxt,llvm::APInt(32,pointerVar.size()),ctxt->IntTy,SourceLocation());
    tmp_argv.push_back(size);
    if (pointerVar.size()==0) tmp_argv.push_back(createNullPointerLiteral(ctxt));
    else tmp_argv.push_back(createConditionVarList(ctxt,pointerVar,ctxt->VoidPtrTy));

    size=IntegerLiteral::Create(*ctxt,llvm::APInt(32,doubleVar.size()),ctxt->IntTy,SourceLocation());
    tmp_argv.push_back(size);
    if (doubleVar.size()==0) tmp_argv.push_back(createNullPointerLiteral(ctxt));
    else tmp_argv.push_back(createConditionVarList(ctxt,doubleVar,ctxt->DoubleTy));

    std::vector<std::string> varName;
    varName.clear();

    varName.insert(varName.end(),intVarName.begin(),intVarName.end());
    varName.insert(varName.end(),charVarName.begin(),charVarName.end());
    varName.insert(varName.end(),pointerVarName.begin(),pointerVarName.end());
    varName.insert(varName.end(),doubleVarName.begin(),doubleVarName.end());
    
    for (size_t i=0;i<varName.size();i++){
        StringLiteral *str = StringLiteral::Create(*ctxt, varName[i], StringLiteral::Ascii,
                false, ctxt->getConstantArrayType(ctxt->CharTy, llvm::APInt(32, varName[i].size() + 1), nullptr,ArrayType::Normal, 0),
                SourceLocation());
        tmp_argv.push_back(str);
    }

    Expr *abstract_cond = M.getInternalHandlerInfo(ctxt).abstract_cond;
    CallExpr *CE = CallExpr::Create(*ctxt, abstract_cond, tmp_argv,
            ctxt->IntTy, VK_RValue, SourceLocation());

    return CE;
}

Expr* createAsciiStringLiteral(ASTContext *ctxt, const std::string &str_val) {
    StringLiteral *SL = StringLiteral::Create(*ctxt, str_val, StringLiteral::Ascii,
            false, ctxt->getConstantArrayType(ctxt->CharTy, llvm::APInt(32, str_val.size() + 1), nullptr,ArrayType::Normal, 0),
            SourceLocation());
    ImplicitCastExpr *ICE1 = ImplicitCastExpr::Create(*ctxt, ctxt->getPointerType(ctxt->CharTy),
            CK_ArrayToPointerDecay, SL, 0, VK_RValue);
    ImplicitCastExpr *ICE2 = ImplicitCastExpr::Create(*ctxt, ctxt->getPointerType(ctxt->getConstType(ctxt->CharTy)),
            CK_BitCast, ICE1, 0, VK_RValue);
    return ICE2;
}

#define UNIQUE_TOKEN "##uni*##"

Expr* createUniqueStringToken(SourceContextManager &M, const RepairAction &action, Expr *oldRExpr) {
    ASTContext *ctxt = M.getSourceContext(action.loc.filename);
    StringLiteral *SL = llvm::dyn_cast<StringLiteral>(stripParenAndCast(oldRExpr));
    assert( SL );
    std::string string_val = SL->getString().str();
    // If it has %s or %n, we are not going to replace all of them
    size_t idx = string_val.rfind("%");
    if (idx == std::string::npos)
        string_val = UNIQUE_TOKEN;
    else {
        size_t j = idx + 1;
        while ((j < string_val.size() - 1) && ((string_val[j] > 'a') && (string_val[j] < 'z')))
            j++;
        string_val = string_val.substr(0, j) + UNIQUE_TOKEN;
    }
    return createAsciiStringLiteral(ctxt, string_val);
}

void enumerateExprImpl(SourceContextManager &M, const RepairCandidate &rc, long long mutate_idx, size_t i,
        std::set<ExprFillInfo> *ret, ExprFillInfo &cur_info) {
    if (i == rc.actions.size()) {
        ret->insert(cur_info);
        return;
    }
    if (rc.actions[i].kind != RepairAction::ExprMutationKind)
        enumerateExprImpl(M, rc, mutate_idx, i+1, ret, cur_info);
    else {
        //LocalAnalyzer *L = getLocalAnalyzer(M, rc, i);
        // If it is the condition synthesizer target, we are going to
        // create a special expr to replace it
        // if (i == (size_t)mutate_idx) {
            if (rc.actions[i].tag == RepairAction::CondTag) {
                cur_info.insert(std::make_pair(i, createAbstractConditionExpr(M, rc.actions[i])));
                enumerateExprImpl(M, rc, mutate_idx, i + 1, ret, cur_info);
                cur_info.erase(i);
                return;
            }
            else {
                cur_info.insert(std::make_pair(i, createUniqueStringToken(M, rc.actions[i], rc.oldRExpr)));
                enumerateExprImpl(M, rc, mutate_idx, i + 1, ret, cur_info);
                cur_info.erase(i);
                return;
            }
        // }
        // else
        //     ret=nullptr;
        /*
        Expr* E = (Expr*)rc.actions[i].ast_node;
        RepairAction::ExprTagTy tag = rc.actions[i].tag;
        if (tag == RepairAction::EnumTag) {
            // If it is an enum constant
            DeclRefExpr *DRE = llvm::dyn_cast<DeclRefExpr>(E);
            assert( DRE && (rc.actions[i].candidate_atoms.size() == 0));
            ValueDecl *VD = DRE->getDecl();
            // If this is a Enumerate, we are going to replace with other type
            if (llvm::isa<EnumConstantDecl>(VD)) {
                EnumConstantDecl *ECD = llvm::dyn_cast<EnumConstantDecl>(VD);
                ExprListTy exprs = L->getCandidateEnumConstant(ECD);
                for (size_t j = 0; j < exprs.size(); j++) {
                    cur_info.insert(std::make_pair(i, exprs[j]));
                    enumerateExprImpl(M, rc, cond_idx, i+1, ret, cur_info);
                    cur_info.erase(i);
                }
                return;
            }
        }
        else if (tag == RepairAction::ReturnTag || tag == RepairAction::ConstantTag) {
            assert( rc.actions[i].candidate_atoms.size() == 0);
            ExprListTy exprs;
            // If it is a return expr, newly created potentially
            if (tag == RepairAction::ReturnTag)
                exprs = L->getCandidateReturnExpr();
            else
                exprs = L->getCandidateConstantInType(E->getType());
            for (size_t j = 0; j < exprs.size(); ++j) {
                cur_info.insert(std::make_pair(i, exprs[j]));
                enumerateExprImpl(M, rc, cond_idx, i+1, ret, cur_info);
                cur_info.erase(i);
            }
            return;
        }
        else if (tag == RepairAction::VarTag || tag == RepairAction::CondTag) {
            // The normal variable replace
            for (size_t j = 0; j < rc.actions[i].candidate_atoms.size(); j++) {
                cur_info.insert(std::make_pair(i, rc.actions[i].candidate_atoms[j]));
                enumerateExprImpl(M, rc, cond_idx, i+1, ret, cur_info);
                cur_info.erase(i);
            }
        }
        */
    }
}

inline void out_codes(const CodeSegTy &a_code, const CodeSegTy &a_patch) {
    for (CodeSegTy::const_iterator it = a_code.begin(); it != a_code.end(); ++it) {
        outlog_printf(3, "//src_file:%s\n", it->first.c_str());
        CodeSegTy::const_iterator pit = a_patch.find(it->first);
        assert( pit != a_patch.end());
        const std::vector<std::string> &c_vec = it->second;
        const std::vector<std::string> &p_vec = pit->second;
        outlog_printf(3, "//Total frags: %lu\n", p_vec.size());
        outlog_printf(4, "//CodeSeg 0:\n%s\n", c_vec[0].c_str());
        for (size_t i = 0; i < p_vec.size(); i++) {
            outlog_printf(3, "//Patch %lu:\n%s\n", i, p_vec[i].c_str());
            outlog_printf(4, "//CodeSeg %lu:\n%s\n", i + 1, c_vec[i + 1].c_str());
        }
    }
}

std::set<ExprFillInfo>* enumerateExprBindings(SourceContextManager &M,
        const RepairCandidate &rc, long long mutate_idx) {
    std::set<ExprFillInfo> *ret=new std::set<ExprFillInfo>;
    ExprFillInfo tmp;
    ret->clear();
    enumerateExprImpl(M, rc, mutate_idx, 0, ret, tmp);
    return ret;
}

double computeFinalScore(bool learning, SourceContextManager &M,
        const RepairCandidate &candidate, size_t id, Expr *E) {
    if (learning) {
        assert(candidate.actions.size() > 0);
        ASTContext *ast = M.getSourceContext(candidate.actions[0].loc.filename);
        Expr *E1 = NULL;
        if (E) {
            E1 = stripParenAndCast(E);
            if (isIntegerConstant(E1))
                E1 = NULL;
            if (E1) {
                BinaryOperator *BO = llvm::dyn_cast<BinaryOperator>(E1);
                if (BO)
                    E1 = BO->getLHS();
            }
        }
        for (std::map<Expr*, double>::const_iterator it = candidate.scoreMap.begin();
                it != candidate.scoreMap.end(); ++it)
            if (stmtToString(*ast, it->first) ==
                    stmtToString(*ast, E1))
                return it->second;
        assert(0);
    }
    else
        return candidate.score * 1000 - id;
}


typedef std::map<std::string, std::vector<std::string> > CodeSegTy;
typedef std::map<std::string, std::vector<std::string> > PatchListTy;
typedef std::map<std::string, std::string> NewCodeMapTy;

typedef BenchProgram::TestCaseSetTy TestCaseSetTy;

class BasicTester {
protected:
    BenchProgram &P;
    bool learning;
    SourceContextManager &M;
    TestCaseSetTy negative_cases, positive_cases;
    std::vector<RepairCandidate> candidates;
    CodeSegTy codes;
    PatchListTy patches;
    bool naive;
    long long total_macro;
    int count;
    std::map<int,std::map<int,std::string>> idAndCase;
    std::vector<std::list<size_t>> switchCluster;
    // std::map<int,std::list<std::list<int>>> caseCluster;
    std::map<std::string,std::map<FunctionDecl*,std::pair<unsigned,unsigned>>> &functionLoc;
    std::map<std::string,std::map<std::string,std::map<size_t,std::string>>> mutationInfo;
    std::map<long long,std::string> macroCode;
    std::map<size_t,std::map<size_t,std::vector<double>>> patchScores;
    std::vector<FunctionReplaceInfo> functionReplaceInfo;

    std::map<std::pair<std::string,size_t>,std::pair<size_t,size_t>> &scores;
    std::map<size_t,std::pair<std::pair<size_t,size_t>,std::pair<size_t,size_t>>> originalLoc;

    bool testOneCase(const BenchProgram::EnvMapTy &env, unsigned long t_id) {
        return P.test(std::string("src"), t_id, env, idAndCase.size(),P.getSwitch().first,P.getSwitch().second);
    }

    bool testNegativeCases(const BenchProgram::EnvMapTy &env) {
        bool result=true;
        for (TestCaseSetTy::iterator it = negative_cases.begin();
                it != negative_cases.end(); it++)
            if (!testOneCase(env, *it)){
                outlog_printf(2,"Failed negative case %lu\n",*it);
                result=false;
            }
        outlog_printf(2, "Passed Negative Cases\n");
        return result;
    }

    bool testPositiveCases(const BenchProgram::EnvMapTy &env) {
        bool result=true;
        // for (TestCaseSetTy::iterator it = failed_cases.begin();
        //         it != failed_cases.end(); it++) {
        //     if (!testOneCase(env, *it)) {
        //         outlog_printf(2, "Failed positive case %lu\n", *it);
        //         result=false;
        //     }
        // }
        for (TestCaseSetTy::iterator it = positive_cases.begin();
                it != positive_cases.end(); it++) {
            // if (failed_cases.count(*it) != 0)
            //     continue;
            if (!testOneCase(env, *it)) {
                outlog_printf(2, "Failed positive case %lu\n", *it);
                // failed_cases.insert(*it);
                result=false;
            }
        }
        outlog_printf(2, "Passed Positive Cases\n");
        return result;
    }

    NewCodeMapTy cleanUpCode(const NewCodeMapTy &code) {
        NewCodeMapTy ret = code;
        for (NewCodeMapTy::iterator it = ret.begin(); it != ret.end(); ++it)
            it->second = M.cleanUpCode(it->second);
        return ret;
    }

    std::map<NewCodeMapTy, double> singleResult(const NewCodeMapTy &code, double score) {
        std::map<NewCodeMapTy, double> ret;
        ret.clear();
        ret.insert(std::make_pair(code, score));
        return ret;
    }
    template<typename K,typename V> bool hasKey(std::map<K,V> target,K key){
        for (typename std::map<K,V>::iterator it=target.begin();it!=target.end();it++){
            if (it->first==key) return true;
        }
        return false;
    }
    std::map<void*,std::vector<double>> getCandidateScores(){
        std::map<void*,std::vector<double>> result;
        for (size_t i=0;i<candidates.size();i++){
            std::vector<double> scores;
            scores.clear();
            if (candidates[i].actions.size()>1 && candidates[i].actions[1].kind==RepairAction::ExprMutationKind){
                ExprListTy atoms=candidates[i].actions[1].candidate_atoms;
                for (size_t j=0;j<atoms.size();j++){
                    scores.push_back(computeFinalScore(learning,M,candidates[i],i,atoms[j]));
                }
            }
            else
                scores.push_back(computeFinalScore(learning,M,candidates[i],i,NULL));
            result[candidates[i].actions[0].ast_node]=scores;
        }
        return result;
    }
    void savePatchInfo(std::vector<File> &infos){
        // Add case number of each switch
        std::map<size_t,size_t> switchCase;
        int i=0;
        for (std::map<int,std::map<int,std::string>>::iterator it=idAndCase.begin();it!=idAndCase.end();it++){
            switchCase[it->first]=idAndCase[it->first].size();
        }
        P.getSwitchInfo().caseNum=switchCase;

        P.getSwitchInfo().switchCluster=switchCluster;
        // P.getSwitchInfo().caseCluster=caseCluster;

        P.getSwitchInfo().scoreInfo=scores;
        P.getSwitchInfo().funcReplaceInfos=functionReplaceInfo;
        P.getSwitchInfo().mutationInfo=mutationInfo;
        P.getSwitchInfo().originalLoc=originalLoc;

        P.getSwitchInfo().infos=infos;
        P.getSwitchInfo().patchScores=patchScores;
        P.getSwitchInfo().save();
    }

    bool fuzzTest(size_t timeout){
        outlog_printf(2,"Running AFL!\n");

        std::string cmd = "msv-afl";
        if (timeout>0) cmd+=" -t "+std::to_string(timeout);
        cmd+=" -w "+P.getWorkdir();
        bool result=system(cmd.c_str());
        return result;
    }

public:
    ASTContext *tempCtxt;
    std::map<std::string,std::vector<long long>> macroFile;
    BasicTester(BenchProgram &P, bool learning, SourceContextManager &M, bool naive,std::map<std::string,std::map<FunctionDecl*,std::pair<unsigned,unsigned>>> &functionLoc,
            std::map<std::pair<std::string,size_t>,std::pair<size_t,size_t>> &scores):
    P(P), learning(learning), M(M), scores(scores),
    negative_cases(P.getNegativeCaseSet()),
    positive_cases(P.getPositiveCaseSet()),
    candidates(),
    functionLoc(functionLoc),tempCtxt(NULL),
    naive(naive) {}

    virtual ~BasicTester() { }

    virtual bool canHandle(const RepairCandidate &candidate) {
        return true;
    }

    void setMutationInfo(std::map<std::string,std::map<std::string,std::map<size_t,std::string>>> &info){
        mutationInfo=info;
    }

    virtual CodeSegTy getCodeSegs() {
        return codes;
    }

    virtual CodeSegTy getPatches() {
        return patches;
    }
    std::map<long long,std::string> getMacroCode(){
        return macroCode;
    }

    virtual long long getMacroCount(){
        return total_macro;
    }
    size_t getSwitchCount(){
        return idAndCase.size();
    }
    BenchProgram::EnvMapTy initEnv(const BenchProgram::EnvMapTy &env){
        BenchProgram::EnvMapTy testEnv=env;
        for (int i=0;i<count;i++){
            testEnv["__ID"+std::to_string(i)]="0";
        }
        return testEnv;
    }

    virtual std::vector<unsigned long> preprocess(const std::vector<RepairCandidate> &candidate) {
        std::vector<std::set<ExprFillInfo> *> infos;
        infos.clear();

        reset_timer();

        for (size_t i=0;i<candidate.size();i++){
            // We are going to create a set of binding ExprFillInfos
            infos.push_back(enumerateExprBindings(M, candidate[i], -1));
        }

        std::map<void*,std::vector<double>> candidateScores=getCandidateScores();

        std::vector<unsigned long> res;
        res.clear();
        CodeRewriter R(M, candidate, &infos,functionLoc,P.getWorkdir(),candidateScores);
        CodeSegTy a_code = R.getCodeSegments();
        CodeSegTy a_patch = R.getPatches();
        idAndCase=R.getIdAndCase();
        count=R.getIdCount();
        total_macro=R.index;
        switchCluster=R.getSwitchCluster();
        macroCode=R.getMacroCode();
        macroFile=R.getMacroFile();
        patchScores=R.patchScores;
        functionReplaceInfo=R.funcReplace;
        originalLoc=R.originalLoc;
        P.getSwitchInfo().varSizes=R.getVarSizes();
        P.getSwitchInfo().funcLocations=R.funcLocation;
        P.getSwitchInfo().patchCodes=R.patchCodes;
        // caseCluster=R.getCaseCluster();

        // Create rules
        std::vector<File> rules=R.rules;        

        savePatchInfo(rules);
        {
            outlog_printf(0, "Meta-program preprocessed in %llus!\n",get_timer());
            out_codes(a_code, a_patch);
        }
        for (size_t i=0;i<candidate.size();i++)
            candidates.push_back(candidate[i]);
        codes=a_code;
        patches=a_patch;
        res.push_back((unsigned long)codes.size());

        return res;
    }

    virtual clang::Expr* getFillExpr(size_t id) {
        return NULL;
    }

    virtual bool test(BenchProgram::EnvMapTy &env, unsigned long id,bool isFuzz) {
        {
            out_codes(codes, patches);
        }
        
        if (isFuzz){
            return fuzzTest(10000000);
        }
        else{
            bool ret;
            outlog_printf(2, "Testing negative cases!\n");
            if (!testNegativeCases(env)) {
                codes.clear();
                patches.clear();
                outlog_printf(2,"Negative Case fail\n");
                // return false;
            }
            outlog_printf(2, "Testing positive cases!\n");
            ret = testPositiveCases(env);
            if (ret)
                outlog_printf(2, "Passed in %llus!\n", get_timer());
            else {
                // We are going to clear out stuff tested, to avoid memory usage.
                outlog_printf(2,"Fail to test with success patch\n");
                codes.clear();
                patches.clear();
            }
            return ret;
        }
    }

    virtual std::map<NewCodeMapTy, double> getResults(unsigned long id) {
        assert( candidates.size() > id);
        double score = computeFinalScore(learning, M, candidates[id], id, NULL);
        if (found_score < score) {
            found_score = score;
            outlog_printf(2, "Update best score %lf\n", found_score);
        }

        if (DumpPassedCandidate.getValue() != "")
            dumpCandidate(M, candidates[id], NULL, score);
        return singleResult(cleanUpCode(combineCode(codes, patches)), score);
    }
};

class StringConstTester : public BasicTester {
    std::map<unsigned long, std::set<std::string> > candidate_strs;
    std::vector<ExprFillInfo> infos;
    std::vector<std::set<ExprFillInfo> *> infos_set;

    bool parseFile(std::vector<std::string> &res, const std::string &fname) {
        res.clear();
        std::ifstream fin(fname.c_str(), std::ifstream::in);
        if (!fin.is_open())
            return false;
        std::string line;
        while (fin) {
            getline(fin, line);
            res.push_back(line);
        }
        return true;
    }

    bool compareStr(const std::vector<std::string> &out_lines,
            const std::vector<std::string> &exp_lines, std::string &replace_str) {
        replace_str = "";
        // FIXME: I hate this code, but I cannot find a better way to do it
        if (out_lines.size() != exp_lines.size())
            return false;
        for (size_t i = 0; i < out_lines.size(); i++) {
            unsigned long token_idx = out_lines[i].find(UNIQUE_TOKEN);
            if (token_idx != std::string::npos) {
                // We first going to identify the longest substr in exp as the
                // match up point. It can be done with O(N+M) with extended KMP
                // but I am lazy. Just a line, bah
                size_t maximum_cnt = 0;
                size_t match_idx = 0;
                if (token_idx > 0) {
                    for (size_t j = 0; j < exp_lines[i].size(); j++) {
                        size_t i1 = token_idx - 1;
                        size_t j1 = j;
                        size_t cnt = 0;
                        while ((i1 > 0) && (j1 > 0)) {
                            if (out_lines[i][i1] == exp_lines[i][j1]) {
                                i1 --;
                                j1 --;
                                cnt ++;
                            }
                            else
                                break;
                        }
                        if (out_lines[i][i1] == exp_lines[i][j1])
                            cnt ++;
                        if (cnt > maximum_cnt) {
                            maximum_cnt = cnt;
                            match_idx = j + 1;
                        }
                    }
                    // This is weird, not likely to be correct.
                    if (maximum_cnt == 0)
                        return false;
                }

                // Now we are going to find identify the longest substr in
                // exp that can match substr starts from token_idx + UNIQUE_TOKEN in out
                maximum_cnt = 0;
                size_t match2_idx = 0;
                for (size_t j = match_idx; j <= exp_lines[i].size(); j++) {
                    size_t i1 = token_idx + std::string(UNIQUE_TOKEN).size();
                    size_t j1 = j;
                    size_t cnt = 0;
                    while ((i1 < out_lines[i].size()) && (j1 < exp_lines[i].size())) {
                        if (out_lines[i][i1] == exp_lines[i][j1]) {
                            i1 ++;
                            j1 ++;
                            cnt ++;
                        }
                        else
                            break;
                    }
                    if (i1 == out_lines[i].size() && j1 == exp_lines[i].size())
                        cnt ++;
                    if (cnt > maximum_cnt) {
                        maximum_cnt = cnt;
                        match2_idx = j;
                    }
                }
                // Now the replace_str should equal to stuff between
                // match_idx and match2_idx
                std::string new_replace = exp_lines[i].substr(match_idx, match2_idx - match_idx);
                if ((replace_str != "") && (new_replace != replace_str))
                    return false;
                replace_str = new_replace;
            }
        }
        return true;
        /*// This does not work because of %s and %d in stupid php exp files
        unsigned long cnt = 0;
        unsigned long i = 0;
        while ((i = out_str.find(UNIQUE_TOKEN, i)) != std::string::npos) {
            cnt ++;
            i ++;
        }
        unsigned long token_size = std::string(UNIQUE_TOKEN).size();
        unsigned long fixed_size = out_str.size() - token_size * cnt;
        unsigned long missing = exp_str.size() - fixed_size;
        if (missing % cnt != 0)
            return false;
        unsigned long each_len = missing / cnt;
        replace_str = "";
        unsigned long j = 0;
        for (size_t i = 0; i < out_str.size(); i++) {
            if (out_str.substr(i, token_size) == UNIQUE_TOKEN) {
                std::string tmp = exp_str.substr(j, each_len);
                if ((replace_str == tmp) || (replace_str == ""))
                    replace_str = tmp;
                else
                    return false;
                i += token_size - 1;
                j += each_len;
            }
            else if (out_str[i] != exp_str[j])
                return false;
            else
                j ++;
        }
        return true;*/
    }

    Expr* createNewStringExpr(ASTContext *ctxt, Expr *oldExpr, const std::string &replace_str) {
        StringLiteral *SL = llvm::dyn_cast<StringLiteral>(stripParenAndCast(oldExpr));
        assert (SL);
        std::string old_str = SL->getString().str();
        unsigned long idx = old_str.find(UNIQUE_TOKEN);
        assert( idx != std::string::npos);
        std::string new_str = old_str.substr(0, idx) + replace_str + old_str.substr(idx + std::string(UNIQUE_TOKEN).size());
        return createAsciiStringLiteral(ctxt, new_str);
    }

public:
    StringConstTester(BenchProgram &P, bool learning, SourceContextManager &M, bool naive,std::map<std::string,std::map<FunctionDecl*,std::pair<unsigned,unsigned>>> functionLoc,
            std::map<std::pair<std::string,size_t>,std::pair<size_t,size_t>> &scores):
        BasicTester(P, learning, M, naive,functionLoc,scores), candidate_strs(), infos(),infos_set() { }

    virtual ~StringConstTester() { }

    int getMutateId(const RepairCandidate &rc) {
        for (int i = 0; i < (int)rc.actions.size(); ++i)
            if (rc.actions[i].kind == RepairAction::ExprMutationKind)
                if (rc.actions[i].tag == RepairAction::StringConstantTag)
                    return i;
        return -1;
    }

    virtual bool canHandle(const RepairCandidate &candidate) {
        return getMutateId(candidate) != -1;
    }

    virtual CodeSegTy getCodeSegs() {
        return codes;
    }

    virtual CodeSegTy getPatches() {
        return patches;
    }

    virtual std::vector<unsigned long> preprocess(const std::vector<RepairCandidate> &candidate) {
        std::vector<std::set<ExprFillInfo> *> the_infos;
        the_infos.clear();
        outlog_printf(2, "[%llu] Preprocess the following candidate with StringConstTester:\n%d\n", get_timer(),
            candidate.size());
        for (size_t i=0;i<candidate.size();i++){
            // We are going to create a set of binding ExprFillInfos
            the_infos.push_back(enumerateExprBindings(M, candidate[i], getMutateId(candidate[i])));
        }
        std::vector<unsigned long> res;
        res.clear();
        CodeRewriter R(M, candidate, &the_infos,functionLoc,P.getWorkdir());
        CodeSegTy a_code = R.getCodeSegments();
        CodeSegTy a_patch = R.getPatches();
        idAndCase=R.getIdAndCase();
        count=R.getIdCount();
        total_macro=R.index;
        {
            outlog_printf(2, "[%llu] StringConstTester, a patch instance with id %lu:\n", get_timer(),
                    codes.size());
            out_codes(a_code, a_patch);
        }
        res.push_back((unsigned long)codes.size());
        for (size_t i=0;i<candidate.size();i++)
            candidates.push_back(candidate[i]);
        codes=a_code;
        patches=a_patch;
        for (size_t i=0;i<the_infos.size();i++){
            infos_set.push_back(the_infos[i]);
            for (std::set<ExprFillInfo>::iterator it=the_infos[i]->begin();it!=the_infos[i]->end();it++)
                infos.push_back(*it);
        }
        // OK, this is hacky, we are going to propagate this change to other place,
        // only here
        //if (!naive) {
        if (false){
            DuplicateDetector D(M, a_code, a_patch);
            if (D.hasDuplicateCodeToPatch()) {
                CodeSegTy a_new_code = D.getNewCodeSegments();
                CodeSegTy a_new_patch = D.getNewPatches();
                {
                    outlog_printf(2, "[%llu] StringConstTester, a patch instance with id %lu (duplicate):\n", get_timer(),
                            codes.size());
                    out_codes(a_new_code, a_new_patch);
                }
                res.push_back(codes.size());
                for (size_t i=0;i<candidate.size();i++)
                    candidates.push_back(candidate[i]);
                codes=a_new_code;
                patches=a_new_patch;
                for (size_t i=0;i<the_infos.size();i++){
                    infos_set.push_back(the_infos[i]);
                    for (std::set<ExprFillInfo>::iterator it=the_infos[i]->begin();it!=the_infos[i]->end();it++)
                        infos.push_back(*it);
                }
            }
        }
        return res;
    }

    virtual clang::Expr* getFillExpr(size_t id) {
        return NULL;
    }

    virtual bool test(const BenchProgram::EnvMapTy &env, unsigned long id) {
        {
            outlog_printf(2, "[%llu] StringConstTester, Testing instance id %lu:\n", get_timer(), id);
            out_codes(codes, patches);
        }
        BenchProgram::EnvMapTy testEnv=initEnv(env);
        outlog_printf(3, "Testing negative cases!\n");
        candidate_strs[id].clear();
        for (TestCaseSetTy::iterator it = negative_cases.begin();
                it != negative_cases.end(); it++) {
            std::string tmp_out = "/tmp/__out.str";
            std::string tmp_exp = "/tmp/__exp.str";
            BenchProgram::EnvMapTy new_env = testEnv;
            new_env["OUTIFFAIL"] = tmp_out;
            new_env["EXPIFFAIL"] = tmp_exp;
            {
                std::string cmd = "rm -rf " + tmp_out;
                int ret = system(cmd.c_str());
                assert( ret == 0);
                cmd = "rm -rf " + tmp_exp;
                ret = system(cmd.c_str());
                assert( ret == 0);
            }
            if (!testOneCase(new_env, *it)) {
                bool may_pass = false;
                std::vector<std::string> outstr, expstr;
                if (parseFile(outstr, tmp_out))
                    if (parseFile(expstr, tmp_exp)) {
                        std::string replace_str;
                        if (compareStr(outstr, expstr, replace_str)) {
                            // If it may pass with some substitution
                            candidate_strs[id].insert(replace_str);
                            may_pass = true;
                        }
                    }
                if (!may_pass || candidate_strs[id].size() > 1) {
                    codes.clear();
                    candidate_strs[id].clear();
                    patches.clear();
                    {
                        std::string cmd = "rm -rf " + tmp_out;
                        int ret = system(cmd.c_str());
                        assert( ret == 0);
                        cmd = "rm -rf " + tmp_exp;
                        ret = system(cmd.c_str());
                        assert( ret == 0);
                    }
                    return false;
                }
            }
            else {
                // XXX: OK, althogh it makes it passes, it got to be an incorrect patch, we just skip it
                return false;
            }
            {
                std::string cmd = "rm -rf " + tmp_out;
                int ret = system(cmd.c_str());
                assert( ret == 0);
                cmd = "rm -rf " + tmp_exp;
                ret = system(cmd.c_str());
                assert( ret == 0);
            }
        }


        // We just skip test positive right now, we are going to check them
        // at postprocess anyway
        /*outlog_printf(3, "Testing positive cases!\n");
        bool ret = testPositiveCases(env);
        if (ret)
            outlog_printf(2, "Passed!\n");
        else {
            // We are going to clear out stuff tested, to avoid memory usage.
            codes[id].clear();
            patches[id].clear();
            candidate_strs[id].clear();
            assert(0);
        }
        return ret;*/
        outlog_printf(2, "[%llu] May pass, will check when postprocessing, with:\n%s\n", get_timer(), candidate_strs[id].begin()->c_str());
        return true;
    }

    virtual std::map<NewCodeMapTy, double> getResults(unsigned long id) {
        outlog_printf(2, "[%llu] StringConstTester postprocessing! \n", get_timer());
        RepairCandidate candidate = candidates[id];
        unsigned long mutate_id = getMutateId(candidate);
        assert( candidate_strs[id].size() == 1);
        ExprFillInfo efi = infos[id];
        ASTContext *ctxt = M.getSourceContext(candidate.actions[mutate_id].loc.filename);
        efi[mutate_id] = createNewStringExpr(ctxt, efi[mutate_id], *candidate_strs[id].begin());

        CodeRewriter R(M, candidates, &infos_set,functionLoc,P.getWorkdir());
        NewCodeMapTy code = R.getCodes();
        BenchProgram::EnvMapTy buildEnv;
        buildEnv.clear();
        if (ForCPP.getValue())
            buildEnv["COMPILE_CMD"] = "clang++";
        else
            buildEnv["COMPILE_CMD"] = "clang";
        std::vector<long long> build_succ = P.buildWithRepairedCode(TEST_CC, buildEnv, code,macroCode,macroFile);
        if (build_succ.size()==0) {
            outlog_printf(2, "Build failed!\n");
            return std::map<NewCodeMapTy, double>();
        }

        bool passed = testNegativeCases(BenchProgram::EnvMapTy());
        if (!passed) {
            outlog_printf(2, "Failed negative cases!\n");
            return std::map<NewCodeMapTy, double>();
        }
        passed = testPositiveCases(BenchProgram::EnvMapTy());
        if (!passed) {
            outlog_printf(2, "Failed positive cases!\n");
            return std::map<NewCodeMapTy, double>();
        }

        double score = computeFinalScore(learning, M, candidates[id], id, NULL);
        if (found_score < score) {
            found_score = score;
            outlog_printf(2, "[%llu] Update best score %lf\n", get_timer(), found_score);
        }

        if (DumpPassedCandidate.getValue() != "")
            dumpCandidate(M, candidate, NULL, score);

        return singleResult(cleanUpCode(code), score);
    }
};

class ConstantCondVisitor : public RecursiveASTVisitor<ConstantCondVisitor> {
    std::set<std::string> res;
    ASTContext *ast;

    void addTo(Expr *E) {
        Expr *E1 = stripParenAndCast(E);
        if (llvm::isa<DeclRefExpr>(E1) || llvm::isa<MemberExpr>(E1))
            res.insert(stmtToString(*ast, E1));
    }

public:
    ConstantCondVisitor(ASTContext *ast): res(), ast(ast) {
        res.clear();
    }

    virtual bool VisitParenExpr(ParenExpr *PE) {
        addTo(PE->getSubExpr());
        return true;
    }

    virtual bool VisitBinLT(BinaryOperator *BO) {
        addTo(BO->getLHS());
        addTo(BO->getRHS());
        return true;
    }

    virtual bool VisitBinGT(BinaryOperator *BO) {
        addTo(BO->getLHS());
        addTo(BO->getRHS());
        return true;
    }

    virtual bool VisitBinLE(BinaryOperator *BO) {
        addTo(BO->getLHS());
        addTo(BO->getRHS());
        return true;
    }

    virtual bool VisitBinGE(BinaryOperator *BO) {
        addTo(BO->getLHS());
        addTo(BO->getRHS());
        return true;
    }

    /*
    virtual bool VisitBinEQ(BinaryOperator *BO) {
        addTo(BO->getLHS());
        addTo(BO->getRHS());
        return true;
    }

    virtual bool VisitBinNE(BinaryOperator *BO) {
        addTo(BO->getLHS());
        addTo(BO->getRHS());
        return true;
    }
    */

    std::set<std::string> getResult() {
        return res;
    }
};

std::set<std::string> getPassedConstantHelper(ASTContext *ast, Stmt *S) {
    IfStmt *IFS = llvm::dyn_cast<IfStmt>(S);
    if (!IFS)
        return std::set<std::string>();
    ConstantCondVisitor V(ast);
    V.TraverseStmt(IFS->getCond());
    return V.getResult();
}
template<typename T>
size_t getElementCount(std::vector<T> vector,T target){
    size_t count=0;
    for (typename std::vector<T>::iterator it=vector.begin();it!=vector.end();it++){
        if (*it==target) count++;
    }
    return count;
}

    // virtual std::map<NewCodeMapTy, double> getResults(BenchProgram::EnvMapTy id) {
    //     post_cnt ++;
    //     {
    //         outlog_printf(2, "CondTester, Postprocessing instance id %lu:\n", id);
    //         out_codes(codes, patches);
    //     }
    //     BranchRecordTy &negative_records = branchRecords[id];
    //     ValueRecordTy &caseVMap = valueRecords[id];
    //     RepairCandidate candidate = candidates[id];
    //     long long condition_idx = getConditionIndex(candidate);
    //     ASTContext *ast = M.getSourceContext(candidate.actions[condition_idx].loc.filename);
    //     ExprListTy &exps = candidates[id].actions[condition_idx].candidate_atoms;
    //     // We are going to do the synthesize, and we recompile everything we tried to recompile
    //     // FIXME: This is some really shitty code
    //     std::vector<Expr*> candidateExprs;
    //     if (learning)
    //         candidateExprs = synthesizeResult(exps, negative_records, caseVMap,
    //             negative_cases, positive_cases, ast);
    //     else
    //         candidateExprs = synthesizeResultSPR(exps, negative_records, caseVMap,
    //             negative_cases, positive_cases, ast);
    //     std::map<NewCodeMapTy, double> res_set;
    //     res_set.clear();
    //     bool passed_constant = false;
    //     double passed_constant_score = -1e20;
    //     std::set<std::string> helper;
    //     helper.clear();
    //     int passed_cnt = 0;
    //     patch_explored += candidateExprs.size();
    //     for (size_t i = 0; i < candidateExprs.size(); i++) {
    //         if (the_timeout_limit != 0)
    //             if (get_timer() > the_timeout_limit) {
    //                 outlog_printf(1, "[%llu] Timeout! Limit is %llu\n", get_timer(), the_timeout_limit);
    //                 break;
    //             }
    //         Expr* new_expr = candidateExprs[i];
    //         ExprFillInfo cur_info = infos[id];
    //         cur_info[condition_idx] = new_expr;
    //         double score = computeFinalScore(learning, M, candidate, id, new_expr);
    //         if (learning) {
    //             if (!isZeroConstantExpr(stripParenAndCast(new_expr)) && (stmtToString(*ast, new_expr).find("len") != std::string::npos))
    //                 score -= 0.7;
    //             if (passed_constant) {
    //                 bool found_it = false;
    //                 std::string stmtStr = stmtToString(*ast, stripParenAndCast(new_expr));
    //                 for (std::set<std::string>::iterator it = helper.begin(); it != helper.end(); it++)
    //                     if (stmtStr.find(*it) == 0) {
    //                         size_t idx = it->size();
    //                         if (stmtStr.size() > idx + 2)
    //                             if ((stmtStr[idx] == ' ') && (stmtStr[idx + 2] == '=')) {
    //                                 found_it = true;
    //                                 break;
    //                             }
    //                     }
    //                 if (found_it) {
    //                     outlog_printf(2, "Going to try %s even with passed constant!\n", stmtStr.c_str());
    //                 }
    //                 else {
    //                     outlog_printf(2, "Rejected %s because of passed constant!\n", stmtStr.c_str());
    //                     if (passed_constant_score - 0.1 + 1e-4 * score < score)
    //                         score = passed_constant_score - 0.1 + 1e-4 * score;
    //                 }
    //             }
    //         }
    //         if (!full_synthesis) {
    //             if (learning) {
    //                 if (found_score >= score && !isIntegerConstant(new_expr))
    //                     continue;
    //             }
    //             else if (found_score >= score)
    //                 continue;
    //         }
    //         CodeRewriter R(M, candidates, &infos_set);
    //         NewCodeMapTy code = R.getCodes();
    //         BenchProgram::EnvMapTy buildEnv;
    //         buildEnv.clear();
    //         if (ForCPP.getValue())
    //             buildEnv["COMPILE_CMD"] = "clang++";
    //         else
    //             buildEnv["COMPILE_CMD"] = "clang";
    //         outlog_printf(2, "Trying a synthesis expr %s\n", stmtToString(*ast, new_expr).c_str());
    //         bool build_succ = P.buildWithRepairedCode(TEST_CC, buildEnv, code,0);
    //         if (!build_succ) {
    //             outlog_printf(3, "Build failed when synthesizing!\n");
    //             continue;
    //         }
    //         TestCaseSetTy passed;
    //         outlog_printf(3, "Verifing Negative cases!\n");
    //         passed = P.testSet("src", negative_cases, std::map<std::string, std::string>());
    //         if (passed != negative_cases) {
    //             outlog_printf(3, "Not passed!\n");
    //             continue;
    //         }
    //         outlog_printf(3, "Verifying positive cases\n");
    //         bool passed_pos = testPositiveCases(std::map<std::string, std::string>());
    //         //passed = P.testSet("src", positive_cases, std::map<std::string, std::string>());
    //         if (!passed_pos) {
    //             outlog_printf(3, "Not passed!\n");
    //             continue;
    //         }
    //         outlog_printf(2, "[%llu] Passed!\n", get_timer());
    //         passed_cnt ++;
    //         if (full_synthesis && !learning)
    //             // OK, i don't want to explain this. It is just a hacky fix to make the experiment checking
    //             // easier. The first passed will come out first if possible.
    //             res_set.insert(std::make_pair(cleanUpCode(code), score - 1e-3 * passed_cnt));
    //         else
    //             res_set.insert(std::make_pair(cleanUpCode(code), score));
    //         if (DumpPassedCandidate.getValue() != "") {
    //             assert(candidate.actions.size() > 0);
    //             Expr *E1 = NULL;
    //             if (new_expr) {
    //                 E1 = stripParenAndCast(new_expr);
    //                 if (isIntegerConstant(E1))
    //                     E1 = NULL;
    //                 if (E1) {
    //                     BinaryOperator *BO = llvm::dyn_cast<BinaryOperator>(E1);
    //                     if (BO)
    //                         E1 = BO->getLHS();
    //                 }
    //             }
    //             dumpCandidate(M, candidates[id], E1, score);
    //         }
    //         if (learning)
    //             if (isIntegerConstant(new_expr)) {
    //                 passed_constant = true;
    //                 passed_constant_score = score;
    //                 helper = getPassedConstantHelper(ast, candidate.actions[condition_idx].loc.stmt);
    //                 outlog_printf(2, "Passed constant expr!\n");
    //                 for (std::set<std::string>::iterator it = helper.begin(); it != helper.end(); ++it)
    //                     outlog_printf(2, "Still allow %s.\n", it->c_str());
    //             }
    //         if (found_score < score) {
    //             found_score = score;
    //             outlog_printf(2, "Passed with updated best score %lf\n", found_score);
    //         }
    //     }
    //     if (res_set.size() != 0) {
    //         valueRecords.erase(id);
    //         branchRecords.erase(id);
    //         return res_set;
    //     }
    //     // FIXME: This is too hacky, I hate this!
    //     double score = computeFinalScore(learning, M, candidate, id, NULL);
    //     if (found_score < score) {
    //         CodeSegTy codeSegs = codes;
    //         PatchListTy patch = patches;
    //         NewCodeMapTy new_code = combineCode(codeSegs, patch);
    //         std::string src_file = candidate.actions[condition_idx].loc.src_file;
    //         outlog_printf(3, "Initial synthesize failed, final attempt\n");
    //         // We are going to loose / tight the condition with existing
    //         // clause as the final attempt
    //         assert( new_code.count(src_file) != 0);
    //         std::set<std::string> codes = replaceIsNegWithClause(new_code[src_file]);
    //         unsigned long cnt = 0;
    //         patch_explored += codes.size();
    //         for (std::set<std::string>::iterator sit = codes.begin();
    //                 sit != codes.end(); ++sit) {
    //             cnt ++;
    //             outlog_printf(3, "Final attempt %lu/%lu with expr %s\n", cnt, codes.size(),
    //                     sit->c_str());
    //             new_code[src_file] = *sit;
    //             TestCaseSetTy passed;
    //             BenchProgram::EnvMapTy buildEnv;
    //             buildEnv.clear();
    //             if (ForCPP.getValue())
    //                 buildEnv["COMPILE_CMD"] = "clang++";
    //             else
    //                 buildEnv["COMPILE_CMD"] = "clang";
    //             bool build_succ = P.buildWithRepairedCode(TEST_CC, buildEnv, new_code,0);
    //             if (!build_succ) {
    //                 outlog_printf(3, "Build failed\n");
    //                 continue;
    //             }
    //             outlog_printf(3, "Trying Negative cases!\n");
    //             passed = P.testSet("src", negative_cases, std::map<std::string, std::string>());
    //             if (passed == negative_cases) {
    //                 outlog_printf(3, "Trying Positive cases!\n");
    //                 passed = P.testSet("src", positive_cases, std::map<std::string, std::string>());
    //                 if (passed == positive_cases) {
    //                     outlog_printf(2, "[%llu] Passed!\n", get_timer());
    //                     if (DumpPassedCandidate.getValue() != "")
    //                         dumpCandidate(M, candidate, NULL, score);
    //                     valueRecords.erase(id);
    //                     branchRecords.erase(id);
    //                     if (found_score < score) {
    //                         found_score = score;
    //                         outlog_printf(2, "Updated best score %lf\n", found_score);
    //                     }
    //                     return singleResult(cleanUpCode(new_code), score);
    //                 }
    //             }
    //         }
    //     }
    //     outlog_printf(2, "Postprocessing failed!\n");
    //     valueRecords.erase(id);
    //     branchRecords.erase(id);
    //     return std::map<NewCodeMapTy, double>();
    // }

class TestBatcher {
    BenchProgram &P;
    SourceContextManager &manager;
    bool naive;
    bool learning;
    FeatureParameter *FP;
    std::string fixedFile;

    struct CandidateEntry {
        RepairCandidate rc;
        BasicTester *T;
        unsigned long id;

        CandidateEntry(const RepairCandidate &rc, BasicTester *T, unsigned long id):
            rc(rc), T(T), id(id) {}
    };

    typedef std::map<CodeSegTy, std::vector<CandidateEntry> > CandidateMapTy;
    std::map<NewCodeMapTy, double> res;
    std::vector<RepairCandidate> succCandidates;
    size_t cur_size;
    unsigned long total_cnt;


    std::map<NewCodeMapTy, double> singleTest(const CodeSegTy &codeSegs, const CodeSegTy &patches,
            BasicTester *T) {
        long long macros=T->getMacroCount();
        // macros=20;

        BenchProgram::EnvMapTy buildEnv;
        buildEnv.clear();
        if (ForCPP.getValue()) {
            buildEnv["CXX"] = TEST_CC;
            buildEnv["COMPILE_CMD"] = "clang++";
        } else {
            buildEnv["CC"] = TEST_CC;
            buildEnv["COMPILE_CMD"] = "clang";
        }
        buildEnv=T->initEnv(buildEnv);
        // for (size_t i=0;i<T->getSwitchCount();i++)
        //     buildEnv["__SWITCH"+std::to_string(i)]="0";
        
        buildEnv["LD_LIBRARY_PATH"]=P.getProphetSrc()+"/.libs:"+std::string(getenv("LD_LIBRARY_PATH"));
        buildEnv["LIBRARY_PATH"]=P.getProphetSrc()+"/.libs:"+std::string(getenv("LIBRARY_PATH"));
        std::map<std::string, std::string> combined=combineCode(codeSegs, patches);

        // Create source file with fix
        // This should success
        // P.saveFixedFiles(combined,fixedFile);
        
        BenchProgram::EnvMapTy testEnv;
        P.applyRepairedCode(combined, buildEnv);
        if (P.skip_profile){
            for (std::map<std::string, std::string>::iterator it=combined.begin();it!=combined.end();it++){
                size_t filePos=it->first.rfind("/");
                std::string bak_file=P.getWorkdir()+"/"+fixedFile+it->first.substr(filePos+1);
                std::ofstream bak_fo(bak_file.c_str());
                bak_fo << it->second;
                bak_fo.close();
                outlog_printf(2,"Saved fixed file at: %s\n",bak_file.c_str());
            }
        }

        std::vector<long long> succ_macros;
        succ_macros.clear();
        if (!P.skip_build){
            succ_macros=P.buildWithRepairedCode(TEST_CC, buildEnv, combined, T->getMacroCode(), T->macroFile, fixedFile);
            outlog_printf(0,"Meta-program generated in %llus!\n",get_timer());
        }

        if (!P.skip_profile){
            outlog_printf(2,"Adding profile writers...\n");
            std::map<long long,std::string> macroCode;
            macroCode.clear();
            reset_timer();
            std::map<std::string,std::vector<long long>> macroFile=addProfileWriter(P,combined,succ_macros,macroCode,fixedFile);
            outlog_printf(0,"Profile writer added in %llus!\n",get_timer());
            outlog_printf(2,"Trying build...\n");
            reset_timer();
            buildEnv["TMP_FILE"]="tmp.log";
            std::vector<long long> succ_macros2=P.buildWithRepairedCode(TEST_CC, buildEnv, combined, macroCode, macroFile, fixedFile, succ_macros);
            if (succ_macros.size()>0) printf("Pass to build final program\n");
            else{
                printf("\033[0;31m");
                printf("\nFail to build with profile writer, check build.log!\n");
                printf("\033[0m");
            }
            outlog_printf(0,"Final build finished in %llus!\n",get_timer());
        }
        P.rollbackOriginalCode(combined, buildEnv);

        // if (P.getSwitch().first==0 && P.getSwitch().second==0)
        //     result_init=T->test(testEnv,0,true);
        // else{
        //     BenchProgram::EnvMapTy tempEnv=testEnv;
        //     result_init=T->test(tempEnv,0,false);
        // }

        // std::string rollbackCmd="rm -f /usr/local/lib/libtest_runtime.so.0.0.0";
        // system(rollbackCmd.c_str());
        // std::string copyCmd="mv /usr/local/lib/libtest_runtime_bak.so.0.0.0 /usr/local/lib/libtest_runtime.so.0.0.0";
        // system(copyCmd.c_str());

        std::map<NewCodeMapTy, double> newCode;
        newCode.clear();
        newCode[combined]=(double)0;
        return newCode;
    }

public:
    TestBatcher(BenchProgram &P, bool naive,
            bool learning, FeatureParameter *FP,std::string fixedFile,SourceContextManager &M):
        P(P), naive(naive), learning(learning && !naive), FP(FP), fixedFile(fixedFile),res(), succCandidates(),cur_size(0),
    total_cnt(0),manager(M) { }

    // This is a lazy test routine, we are only going to decode it without
    // actually doing the test in the most of the time
    bool test(const std::vector<RepairCandidate> &candidate, BasicTester* T) {
        T->preprocess(candidate);
        outlog_printf(2, "Begin building test\n");
        CodeSegTy codeSegs = T->getCodeSegs();
        PatchListTy patches = T->getPatches();
        outlog_printf(2,"Total macros: %d\n",T->getMacroCount());
        // for (int j=0;j<candidate.size();j++){
        //     tot_explored_templates += candidate[j].getCandidateAtoms().size();
        //     patch_explored += candidate[j].getCandidateAtoms().size();
        // }
        std::map<NewCodeMapTy, double> code_set = singleTest(codeSegs, patches, T);
        for (std::map<NewCodeMapTy, double>::iterator it = code_set.begin();
                it != code_set.end(); it++) {
            NewCodeMapTy code = it->first;
            double res_score = it->second;
            if (res.count(code) == 0)
                res.insert(std::make_pair(code, res_score));
            else if (res[code] < res_score)
                res[code] = res_score;
            for (size_t j=0;j<candidate.size();j++)
                succCandidates.push_back(candidate[j]);
        }
        return res.size()!=0;
    }

    bool hasResult() {
        return res.size() != 0;
    }

    std::vector<std::pair<NewCodeMapTy, double> > getResults() {
        std::vector<std::pair<NewCodeMapTy, double> > tmp;
        tmp.clear();
        for (std::map<NewCodeMapTy, double>::iterator it = res.begin(); it != res.end(); it++)
            tmp.push_back(std::make_pair(it->first, it->second));
        if (tmp.size() > 0) {
            for (size_t i = 0; i < tmp.size() - 1; i++)
                for (size_t j = i + 1; j < tmp.size(); j++)
                    if (tmp[i].second < tmp[j].second)
                        std::swap(tmp[i], tmp[j]);
        }
        return tmp;
    }

    size_t size() {
        return cur_size;
    }

    std::vector<RepairCandidate> getSuccCandidates() {
        return succCandidates;
    }
};

}

bool ExprSynthesizer::workUntil(size_t candidate_limit, size_t time_limit,
        ExprSynthesizerResultTy &res, bool full_synthesis, bool quit_with_any) {
    the_timeout_limit = this->timeout_limit;
    TestBatcher TB(P, naive, learning, FP,fixedFile,M);
    std::vector<BasicTester*> testers;
    testers.clear();
    // testers.push_back(new ConditionSynthesisTester(P, learning, M, full_synthesis,functionLoc,scores));
    testers.push_back(new StringConstTester(P, learning, M, naive,functionLoc,scores));
    testers.push_back(new BasicTester(P, learning, M, naive,functionLoc,scores));
    // outlog_printf(2, "BasicTester pointer: %p\n", testers[2]);
    // outlog_printf(2, "StringConstTester pointer: %p\n", testers[1]);
    // outlog_printf(2, "CondTester pointer: %p\n", testers[0]);
    TestCache *cache = P.getTestCache();
    size_t cnt = 0;
    ExecutionTimer Timer;
    time_t last_sync = Timer.getSeconds();

    std::vector<std::pair<NewCodeMapTy, double> > collected_res;
    std::map<std::string, size_t> candidate_to_id;
    candidate_to_id.clear();
    collected_res.clear();
    unsigned long generate_min_id = 100000000;
    found_score = -1e20;

    std::vector<RepairCandidate> candidate;
    candidate.clear();
    outlog_printf(2,"Patch Candidates Generated!\nCandidates size: %d\n",q.size());
    while(q.size()>0){
        RepairCandidateWithScore candidate_a_score = q.top();
        candidate.push_back(candidate_a_score.first);
        q.pop();
    }

    bool result;
    outlog_printf(2,"Generating meta-program...\n");
    // for (int i=0;i<testers.size();i++)
    // result= TB.test(candidate, testers[0]);
    testers[1]->setMutationInfo(mutationInfo);
    result= TB.test(candidate, testers[1]);

    outlog_printf(0, "The total number of explored concrete patches: %lu\n", patch_explored);
    for (size_t i = 0; i < testers.size(); i++)
        delete testers[i];
    return result;
}
