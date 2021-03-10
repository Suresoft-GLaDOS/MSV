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
#include "CodeRewrite.h"
#include "ExprSynthesizer.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ASTContext.h"
#include "llvm/Support/raw_ostream.h"
#include <string>

using namespace clang;

static std::string getNextToken(const std::string &str, size_t idx, size_t &next_idx) {
    size_t i = idx;
    while ((str[i] == '\n') || (str[i] == ' ') || (str[i] == '\t'))
        i++;
    std::string ret = "";
    while ((str[i] != '\n') && (str[i] != ' ') && (str[i] != '\t')) {
        ret.push_back(str[i]);
        i++;
    }
    next_idx = i;
    return ret;
}

bool canMerge(const CodeSegTy &codeSegs, const CodeSegTy &patches) {
    // So far we cannot merge the patch code that contains label
    // identifier, because we use nasty string manipulation.
    for (CodeSegTy::const_iterator it = patches.begin(); it != patches.end(); ++it) {
        assert(codeSegs.count(it->first) != 0);
        const std::vector<std::string> &tmp_vec = it->second;
        for (size_t i = 0; i < tmp_vec.size(); i++) {
            std::string patch = tmp_vec[i];
            size_t idx = 0;
            while (idx < patch.size()) {
                idx = patch.find(":", idx);
                if (idx != std::string::npos) {
                    if (idx + 1 <= patch.size())
                        if (patch[idx + 1] == ':') {
                            // Double "::", ignore
                            idx += 2;
                            continue;
                        }
                    size_t j = idx;
                    while (j > 0)
                        if (patch[j] != ';')
                            j --;
                        else
                            break;
                    size_t next_idx;
                    std::string token = getNextToken(patch, j, next_idx);
                    if (token != "case") {
                        bool multi_token = false;
                        for (size_t k = next_idx; k < idx; k++) {
                            if (patch[k] != '\n' && patch[k] != ' ' && patch[k] != '\t') {
                                multi_token = true;
                                break;
                            }
                        }
                        if (multi_token == false)
                            return false;
                    }
                    idx += 2;
                }
                else
                    break;
            }
            // We also cannot handle the case of declare statement
            // FIXME: this is just a heristic to get rid of this
            idx = 0;
            size_t idx3 = patch.find("{", 0);
            if (idx3 == std::string::npos) idx3 = patch.size();
            while (idx < patch.size() && (idx < idx3)) {
                idx = patch.find("\n", idx);
                if (idx == std::string::npos) break;
                size_t idx2 = idx + 1;
                while (idx2 < patch.size()) {
                    if ((patch[idx2] == ' ') || (patch[idx2] == '\t') || (patch[idx2] == '\n'))
                        idx2 ++;
                    else
                        break;
                }
                std::string token = "";
                while (idx2 < patch.size()) {
                    if ((patch[idx2] != ' ') && (patch[idx2] != '\t') && (patch[idx2] != '\n')) {
                        token.push_back(patch[idx2]);
                        idx2 ++;
                    }
                    else
                        break;
                }
                if (idx2 < idx3) {
                    if (token.size() > 0) {
                        if ((token == "unsigned") || (token == "int") || (token == "long") || (token == "char") ||
                            (token[token.size() - 1] == '*'))
                            return false;
                    }
                }
                idx = idx2;
            }
        }
    }
    return true;
}

bool getTrailingIndent(const std::string &str, std::string &indent_str) {
    size_t idx = str.size() - 1;
    while ((idx > 0) && (str[idx] == ' ' || str[idx] == '\t'))
        idx --;
    if (str[idx] != '\n')
        return false;
    else {
        indent_str = str.substr(idx+1);
        return true;
    }
}

std::string indentPatch(const std::string &patch, const std::string &indent_str) {
    std::vector<std::string> lines;
    lines.size();
    std::string tmp = "";
    for (size_t i = 0; i < patch.size(); i++) {
        tmp.push_back(patch[i]);
        if (patch[i] == '\n') {
            lines.push_back(tmp);
            tmp = "";
        }
    }
    if (tmp != "") lines.push_back(tmp);
    std::string ret = "";
    for (size_t i = 0; i < lines.size(); i++) {
        ret += lines[i];
        if (i != lines.size() - 1)
            ret += indent_str;
    }
    return ret;
}

NewCodeMapTy combineCode(const CodeSegTy &codeSegs, const CodeSegTy &patch) {
    assert( codeSegs.size() == patch.size() );
    NewCodeMapTy ret;
    ret.clear();
    for (CodeSegTy::const_iterator it = patch.begin(); it != patch.end(); ++it) {
        std::string src_file = it->first;
        CodeSegTy::const_iterator code_it = codeSegs.find(src_file);
        assert( code_it != codeSegs.end());
        const std::vector<std::string> &seg_vec = code_it->second;
        const std::vector<std::string> &patch_vec = it->second;
        assert( seg_vec.size() == patch_vec.size() + 1);
        ret[src_file] = "";
        ret[src_file] += seg_vec[0];
        std::string indent_str = "";
        for (size_t i = 0; i < patch_vec.size(); i++) {
            getTrailingIndent(seg_vec[i], indent_str);
            //ret[src_file] += std::string("bool count=true;\nif (count==true){\n");
            ret[src_file] += indentPatch(std::string("//prophet generated patch\n") +
                    patch_vec[i], indent_str) /* +std::string("}\n")*/ +seg_vec[i+1];
        }
    }
    return ret;
}

NewCodeMapTy mergeCode(const CodeSegTy &codeSegs, const std::vector<CodeSegTy> &patches) {
    CodeSegTy mergedPatch;
    mergedPatch.clear();
    for (CodeSegTy::const_iterator it = codeSegs.begin(); it != codeSegs.end(); ++it) {
        std::string src_file = it->first;
        mergedPatch[src_file].clear();
        CodeSegTy::const_iterator p_it = patches[0].find(src_file);
        assert( p_it != patches[0].end() );
        size_t n = p_it->second.size();
        assert( n == it->second.size() - 1);
        for (size_t j = 0; j < n; j ++) {
            std::string tmp_str = "";
            for (size_t i = 0; i < patches.size(); i++) {
                p_it = patches[i].find(src_file);
                assert( p_it != patches[i].end());
                assert(p_it->second.size() > j);
                std::ostringstream sout;
                if (i == 0)
                    sout << "if ";
                else
                    sout << "else if ";
                sout << "(__get_mutant()==" << i << ") {\n";
                sout << p_it->second[j];
                sout << "}\n";
                tmp_str += sout.str();
            }
            mergedPatch[src_file].push_back(tmp_str);
        }
    }
    return combineCode(codeSegs, mergedPatch);
}

static RepairCandidate replaceExprInCandidate(SourceContextManager &M,
        const RepairCandidate &rc, const ExprFillInfo &efi) {
    RepairCandidate ret = rc;
    ret.actions.clear();
    std::map<Stmt*, size_t> tmp_map1;
    std::map<Stmt*, std::map<Expr*, Expr*> > tmp_map2;
    tmp_map1.clear();
    tmp_map2.clear();
    for (size_t i = 0; i < rc.actions.size(); i++) {
        if (rc.actions[i].kind != RepairAction::ExprMutationKind) {
            tmp_map1[(Stmt*)rc.actions[i].ast_node] = ret.actions.size();
            ret.actions.push_back(rc.actions[i]);
        }
        else {
            Stmt *S = rc.actions[i].loc.stmt;
            if (tmp_map2.count(S) == 0)
                tmp_map2[S].clear();
            ExprFillInfo::const_iterator e_it = efi.find(i);
            assert( e_it != efi.end());
            tmp_map2[S].insert(std::make_pair((Expr*)rc.actions[i].ast_node,
                        e_it->second));
            if (tmp_map1.count(S) == 0) {
                tmp_map1[S] = ret.actions.size();
                // This is a old statement at all, we are going to add a replacemutation
                ret.actions.push_back(RepairAction(rc.actions[i].loc,
                            RepairAction::ReplaceMutationKind, S));
            }
        }
    }
    for (std::map<Stmt*, std::map<Expr*, Expr*> >::iterator it = tmp_map2.begin();
            it != tmp_map2.end(); ++it) {
        assert( tmp_map1.count(it->first) != 0);
        size_t idx = tmp_map1[it->first];
        ASTContext *ctxt = M.getSourceContext(ret.actions[idx].loc.filename);
        StmtReplacer R(ctxt, it->first);
        for (std::map<Expr*, Expr*>::iterator it2 = it->second.begin();
                it2 != it->second.end(); ++it2) {
            // ParenExpr *E = new(*ctxt) ParenExpr(SourceLocation(), SourceLocation(), it2->second);
            // E->dump();
            R.addRule(it2->first, it2->second);
        }
        ret.actions[tmp_map1[it->first]].ast_node = R.getResult();
    }
    return ret;
}

static std::pair<size_t, size_t> getStartEndOffset(SourceContextManager &M,
        const std::string &src_file, const ASTLocTy &loc) {
    ASTContext *Context = M.getSourceContext(loc.filename);
    SourceManager &manager = Context->getSourceManager();
    unsigned start_idx = manager.getFileOffset(manager.getExpansionLoc(loc.stmt->getBeginLoc()));
    unsigned end_idx = manager.getFileOffset(manager.getExpansionLoc(loc.stmt->getEndLoc()));
    std::string code = M.getSourceCode(loc.src_file);
    while (end_idx < code.size()) {
        if (code[end_idx] == '\n') {
            end_idx ++;
            break;
        }
        if (code[end_idx] == ';') {
            end_idx ++;
            if (end_idx < code.size() && code[end_idx] == '\n')
                end_idx ++;
            break;
        }
        end_idx ++;
    }
    return std::make_pair(start_idx, end_idx);
}
bool hasUnknownIdent(ASTContext *ctxt,Stmt *s){
    std::string str=stmtToString(*ctxt,s);
    if (str.find("--this")!=std::string::npos ||
        str.find("++this")!=std::string::npos||
        str.find("_M_")!=std::string::npos ||
        str.find("_S_")!=std::string::npos) return true;
    else return false;
}
std::string toString(RepairAction action){
    switch(action.kind){
        case RepairAction::ReplaceMutationKind:
            return "ReplaceMutationKind";
        case RepairAction::InsertMutationKind:
            return "InsertMutationKind";
        case RepairAction::InsertAfterMutationKind:
            return "InsertAfterMutationKind";
        default:
            return "ExprMutationKind";
    }
}
std::string toString(RepairCandidate candidate){
    switch(candidate.kind){
        case RepairCandidate::TightenConditionKind:
            return "TightenConditionKind";
        case RepairCandidate::LoosenConditionKind:
            return "LoosenConditionKind";
        case RepairCandidate::GuardKind:
            return "GuardKind";
        case RepairCandidate::SpecialGuardKind:
            return "SpecialGuardKind";
        case RepairCandidate::IfExitKind:
            return "IfExitKind";
        case RepairCandidate::AddInitKind:
            return "AddInitKind";
        case RepairCandidate::ReplaceKind:
            return "ReplaceKind";
        case RepairCandidate::ReplaceStringKind:
            return "ReplaceStringKind";
        default:
            return "AddAndReplaceKind";
    }
}
std::string removeComment(std::string code){
    if (code.size()<2) return code;
    for (int i=0;i<code.size()-1;i++){
        if (code[i]=='/' && code[i+1]=='/'){
            int j=i+2;
            if (j<code.size())
                for (j=i+2;j<code.size();j++){
                    if (code[j]=='\n') break;
                }

            code=code.erase(i,j-i);
        }
        else if (code[i]=='/' && code[i+1]=='*'){
            int j=i+2;
            if (j<code.size()-1)
                for (j=i+2;j<code.size()-1;j++){
                    if (code[j]=='*' && code[j+1]=='/') break;
                }

            code=code.erase(i,j-i+1);
        }
    }
    return code;
}
std::string removeSpace(std::string code){
    code=removeComment(code);
    for (int i=0;i<code.size();i++){
        if (code[i]==' ' || code[i]=='\t' || code[i]=='\n'){
            code=code.erase(i,1);
            i--;
        }
    }
    return code;
}
size_t getIsNegCount(std::string code){
    size_t position=code.find("__is_neg");
    size_t count=0;

    while (position!=std::string::npos){
        count++;
        position=code.find("__is_neg",position+1);
    }
    return count;
}
size_t CodeRewriter::addIsNeg(int id,int case_num,std::string code){
    size_t position=code.find("__is_neg");
    size_t count=0;

    std::pair<int,int> location(id,case_num);
    isNegLocation[location].clear();

    while (position!=std::string::npos){
        count++;
        position=code.find("(",position);
        for (position;code[position]==' ';position++){}
        int isNegId=std::strtol(code.substr(position).c_str(),nullptr,0);
        std::pair<int,int> location(id,case_num);
        isNegLocation[location].push_back(isNegId);

        position=code.find("__is_neg",position);
    }
    return count;
}
std::map<ASTLocTy, std::map<std::string, bool> > CodeRewriter::eliminateAllNewLoc(SourceContextManager &M,
        const std::vector<RepairCandidate> &rc,std::map<ASTLocTy,std::string> &original_str) {
    original_str.clear();
    // We first construct the list of stmt close to each original ASTLocTy
    // and we store it to tmp_map1
    std::map<ASTLocTy, std::vector<std::vector<Stmt*>> > tmp_map1;
    tmp_map1.clear();
    // Change: Process with candidates vactor, not one candidate
    for (size_t j=0;j<rc.size();j++){
        if (DeclStmt::classof(rc[j].original)) continue;
        for (size_t i = 0; i < rc[j].actions.size(); i++) {
            const ASTLocTy &rootLoc = rc[j].actions[i].loc;
            if (DeclStmt::classof(rootLoc.stmt)) continue;
            if (tmp_map1[rootLoc].size()==0){
                std::vector<Stmt *> orig_vec;
                orig_vec.push_back(rc[j].original);
                tmp_map1[rootLoc].push_back(orig_vec);
            }

            // outlog_printf(2,"Candidate Type: %s\n",toString(rc[j]).c_str());
            // outlog_printf(2,"Action Type: %s\n",toString(rc[j].actions[i]).c_str());
            // ASTContext *ctxt = M.getSourceContext(rootLoc.filename);
            // std::string new_str=stmtToString(*ctxt,(Stmt*)rc[j].actions[i].ast_node);
            // outlog_printf(2,"new string: %s\n\n",new_str.c_str());

            std::vector<Stmt *> vec_create;
            vec_create.clear();
            tmp_map1[rootLoc].push_back(vec_create);
            std::vector<Stmt *> tmp_vector;
            tmp_vector.clear();
            Stmt* S = (Stmt*) rc[j].actions[i].ast_node;
            
            //if (hasUnknownIdent(M.getSourceContext(rc[j].actions[i].loc.filename),S)) continue;
            // This is a new original statement
            //if (tmp_map2.count(rc[j].actions[i].loc.stmt) == 0) {
            if (std::find(tmp_vector.begin(),tmp_vector.end(),
                rc[j].actions[i].loc.stmt)==tmp_vector.end()){
                //tmp_map2.insert(std::make_pair(rc[j].actions[i].loc.stmt, rc[j].actions[i].loc));
                tmp_vector.clear();
                tmp_vector.push_back(rc[j].actions[i].loc.stmt);
            }
            //tmp_map2[S] = rootLoc;
            std::vector<Stmt*>::iterator pos = std::find(tmp_vector.begin(),
                   tmp_vector.end(), rc[j].actions[i].loc.stmt);
            assert( pos != tmp_vector.end() );
            if (rc[j].actions[i].kind == RepairAction::ReplaceMutationKind){
                *pos=S;
            }
            else if (rc[j].actions[i].kind == RepairAction::InsertMutationKind)
                tmp_vector.insert(pos, S);
            else if (rc[j].actions[i].kind == RepairAction::InsertAfterMutationKind){
                assert(rc[j].actions[i].kind == RepairAction::InsertAfterMutationKind);
                pos ++;
                if (pos == tmp_vector.end())
                    tmp_vector.push_back(S);
                else
                    tmp_vector.insert(pos, S);
            }
            tmp_map1[rootLoc].push_back(tmp_vector);
        }
    }
    // We then get string from the list of stmt for each ASTLocTy
    std::map<ASTLocTy, std::map<std::string, bool> > ret;
    ret.clear();
    index=0;
    for (std::map<ASTLocTy, std::vector<std::vector<Stmt*>> >::iterator it = tmp_map1.begin();
            it != tmp_map1.end(); ++it) {
        std::map<std::string,bool> str_vec;
        str_vec.clear();

        ASTContext *ctxt = M.getSourceContext(it->first.filename);
        std::string original_case=stmtToString(*ctxt,it->first.stmt);
        if (original_case[original_case.size() - 1]  != '\n' && original_case[original_case.size() - 1] != ';')
            original_case += ";\n";
        original_str.insert(std::pair<ASTLocTy,std::string>(it->first,
                original_case));
        bool skip=false;

        for (std::vector<std::vector<Stmt *>>::iterator it2=it->second.begin();
                it2!=it->second.end();it2++){
            std::vector<Stmt*> &tmp_vec = *it2;
            std::pair<std::string,bool> str_pair;
            str_pair.first = "";
            str_pair.second = !llvm::isa<CompoundStmt>(it->first.parent_stmt);
            for (size_t i = 0; i < tmp_vec.size(); i++) {
                std::string tmp = stmtToString(*ctxt, tmp_vec[i]);
                str_pair.first += tmp;
                if (tmp[tmp.size() - 1]  != '\n' && tmp[tmp.size() - 1] != ';')
                    str_pair.first += ";\n";
            }

            if (str_pair.first=="") continue;
            if (original_case==str_pair.first){
                continue;
            }
            str_vec.insert(str_pair);
        }

        if (skip==true) continue;
        ret[it->first]=str_vec;
        
    }
    return ret;
}

CodeRewriter::CodeRewriter(SourceContextManager &M, const std::vector<RepairCandidate> &rc, std::vector<std::set<ExprFillInfo> *> *pefi) {
    std::map<ASTLocTy,std::string> original_str;
    std::vector<RepairCandidate> rc1;
    rc1.clear();
    for (int i=0;i<rc.size();i++){
        std::vector<ExprFillInfo> temp((*pefi)[i]->begin(),(*pefi)[i]->end());
        for(int k=0;k<temp.size();k++){
            ExprFillInfo efi;
            if (pefi !=nullptr){
                efi = temp[k];
            }
            else {
                efi.clear();
                // We are going to replace it with all unknown expressions
                for (size_t j = 0; j < rc[i].actions.size(); j++)
                    if (rc[i].actions[j].kind == RepairAction::ExprMutationKind) {
                        ASTContext *ctxt = M.getSourceContext(rc[i].actions[j].loc.filename);
                        efi[j] = M.getUnknownExpr(ctxt, rc[i].actions[j].candidate_atoms);
                    }
            }
            //rc.dump();
            // We first the rid of all ExprMutationKind in rc
            rc1.push_back(replaceExprInCandidate(M, rc[i], efi));
            //rc1.dump();
            // We then eliminate ASTLocTy with new statements, and replace them with
            // strings
            //std::map<ASTLocTy, std::pair<std::string, bool> > tmp=eliminateAllNewLoc(M, rc[i]);
            //res1.insert(tmp.begin(),tmp.end());
        }
    }
    // Create string with whole candidate vector 
    std::map<ASTLocTy, std::map<std::string, bool> > res1=eliminateAllNewLoc(M, rc1,original_str);
    // We then categorize location based on the src_file and their offset
    std::map<std::string, std::map<std::pair<size_t, size_t>, ASTLocTy> > res2;
    std::map<std::string, std::map<std::pair<size_t, size_t>, ASTLocTy> > tmp_loc;
    res2.clear();
    tmp_loc.clear();
    for (std::map<ASTLocTy, std::map<std::string, bool> >::iterator it = res1.begin();
            it != res1.end(); ++it) {
        //llvm::errs() << "Location: " << it->first.toString(M) << "\n";
        //llvm::errs() << "patch: " << it->second.first << "\n";
        //llvm::errs() << "braket: " << it->second.second << "\n";
        ASTLocTy loc = it->first;
        std::string src_file = loc.src_file;
        if (tmp_loc.count(src_file) == 0)
            tmp_loc[src_file].clear();
        std::pair<size_t, size_t> offset_pair = getStartEndOffset(M, src_file, loc);
        //llvm::errs() << "offset: " << offset_pair.first << " " << offset_pair.second << "\n";
        // We reverse the start and end for sorting purpose
        tmp_loc[src_file].insert(std::make_pair(std::make_pair(offset_pair.second,
                        offset_pair.first), loc));
    }

    // Sorting res2 with start location
    if (true){
        for (std::map<std::string, std::map<std::pair<size_t, size_t>, ASTLocTy> >::iterator        
                it = tmp_loc.begin(); it != tmp_loc.end(); ++it) {
            std::vector<std::pair<size_t,size_t>> locationVec;
            std::vector<ASTLocTy> valueVec;
            locationVec.clear();
            valueVec.clear();
            for (std::map<std::pair<size_t, size_t>, ASTLocTy>::iterator it2 = it->second.begin();
                    it2 != it->second.end(); ++it2) {
                locationVec.push_back(it2->first);
                valueVec.push_back(it2->second);
            }

            for (int i=0;i<locationVec.size();i++) {
                for (int j=i-1;j>=0;j--){
                    if (locationVec[j].second>locationVec[i].second){
                        std::pair<size_t,size_t> temp=locationVec[i];
                        locationVec[i]=locationVec[j];
                        locationVec[j]=temp;

                        ASTLocTy temp2=valueVec[i];
                        valueVec[i]=valueVec[j];
                        valueVec[j]=temp2;
                    }
                    else{
                        break;
                    }
                }
            }
                
            outlog_printf(2,"test\n");

            std::map<std::pair<size_t, size_t>, ASTLocTy> new_map;
            new_map.clear();
            for (int k=0;k<locationVec.size();k++){
                std::pair<size_t,size_t> temp=locationVec[k];
                ASTLocTy temp2=valueVec[k];
                new_map[temp]=temp2;
            }
            res2[it->first]=new_map;
        }
    }
    outlog_printf(2,"Generating patches...\n");
    // Then we handle each source file saperately
    resCodeSegs.clear();
    resPatches.clear();
    macroMap.clear();
    idAndCase.clear();
    int beforeId=0;
    counter=0;
    long long macro=0;
    for (std::map<std::string, std::map<std::pair<size_t, size_t>, ASTLocTy> >::iterator
            it = res2.begin(); it != res2.end(); ++it) {
        std::string src_file = it->first;
        resCodeSegs[src_file].clear();
        resPatches[src_file].clear();
        std::string code = M.getSourceCode(src_file);
        resCodeSegs[src_file].push_back(code);
        std::vector<std::string> cur_patch;
        long long cur_start = -1;
        long long cur_end = -1;
        long long last_end = 0;
        for (std::map<std::pair<size_t, size_t>, ASTLocTy>::iterator it2 = it->second.begin();
                it2 != it->second.end(); ++it2) {
            // NOTE: The start and the end are reversed
            if (it2->first.second==0) continue;
            long long start = it2->first.second;
            long long end = it2->first.first;
            int case_count=0;
            assert( start >= last_end);
            // if (start < last_end) continue;
            if (cur_start == -1) {
                cur_patch.clear();
                cur_start = start;
                cur_end = end;
                ASTContext *ctxt = M.getSourceContext(it2->second.filename);
                cur_patch.push_back(original_str[it2->second]);
                for (std::map<std::string,bool>::iterator patch_it=res1[it2->second].begin();
                        patch_it!=res1[it2->second].end();patch_it++){
                    std::string patch=patch_it->first;
                    if (patch_it->second)
                        patch = "    " + indentPatch(patch_it->first, "    ");
                    cur_patch.push_back(patch);
                }
            }
            else if (start<=cur_start && cur_end <= end) {
                // We need to merge these two, we first need to decide in the bigger one,
                // which part is not changed
                std::vector<std::string> before_patch=cur_patch;
                cur_patch.clear();
                std::string top_part = code.substr(start, cur_start - start);
                std::string mid_part = code.substr(cur_start, cur_end-cur_start);
                std::string bottom_part = code.substr(cur_end, end - cur_end);
                std::string big_patch=original_str[it2->second];
                // outlog_printf(2,"top: %s\n",top_part.c_str());
                // outlog_printf(2,"mid: %s\n",mid_part.c_str());
                // outlog_printf(2,"bottom: %s\n",bottom_part.c_str());
                // outlog_printf(2,"big: %s\n",big_patch.c_str());
                // outlog_printf(2,"before: %s\n",before_patch[0].c_str());
                for (int i=0;i<before_patch.size();i++){
                    std::string body=top_part+before_patch[i]+bottom_part;
                    cur_patch.push_back(body);
                }

                for (std::map<std::string,bool>::iterator patch_it=res1[it2->second].begin();
                        patch_it!=res1[it2->second].end();patch_it++){
                    big_patch = patch_it->first;
                    std::string body;
                    if (removeSpace(top_part) == removeSpace(big_patch.substr(0, cur_start - start))) {
                        for(int i=0;i<before_patch.size();i++){
                            body = top_part + before_patch[i] + big_patch.substr(cur_end - start);
                            if (patch_it->second)
                                body = "    " + indentPatch(body, "    ");
                            cur_patch.push_back(body);
                        }
                    }
                    else {
                        assert(removeSpace(big_patch).find(removeSpace(bottom_part))!=std::string::npos);
                        for(int i=0;i<before_patch.size();i++){
                            body = big_patch.substr(0, big_patch.find(mid_part))
                                + before_patch[i] + bottom_part;
                            if (patch_it->second)
                                body = "    " + indentPatch(body, "    ");
                            // outlog_printf(2,"big: %s\n",body.c_str());
                            cur_patch.push_back(body);
                        }
                    }
                }
                cur_start = start;
                cur_end = end;
            }
            else {
                assert(start >= cur_end);
                std::string last_code=resCodeSegs[src_file][resCodeSegs[src_file].size()-1];
                resCodeSegs[src_file].pop_back();
                assert(code.find(last_code)!=std::string::npos);
                size_t seg_start=code.find(last_code);


                resCodeSegs[src_file].push_back(code.substr(seg_start,cur_start-seg_start));

                std::string body="switch(__choose(\"__ID"+std::to_string(counter++)+"\"))\n{\n";
                body+="case "+std::to_string(case_count++)+": {\n";
                body+=cur_patch[0];
                int isNegCount=addIsNeg(counter-1,0,cur_patch[0]);
                body+="\nbreak;\n}\n";
                for(int i=1;i<cur_patch.size();i++){
                    isNegCount=getIsNegCount(cur_patch[i]);
                    if (isNegCount>=2) continue;
                    isNegCount=addIsNeg(counter-1,case_count,cur_patch[i]);

                    body+="#ifdef COMPILE_"+std::to_string(index)+"\n";
                    body+="case "+std::to_string(case_count)+": {\n";
                    body+=cur_patch[i];
                    body+="\nbreak;\n}\n";
                    body+="#endif\n";

                    macroMap.insert(std::pair<long long,std::pair<int,int>>(index++,std::pair<int,int>(counter-1,case_count)));
                    idAndCase.insert(std::pair<std::string,std::pair<int,int>>(cur_patch[i],std::pair<int,int>(counter-1,case_count++)));
                }
                body+="}\n";
                resPatches[src_file].push_back(body);
                resCodeSegs[src_file].push_back(code.substr(cur_end,code.size()-cur_end));

                //resCodeSegs[src_file].push_back(code.substr(last_end, cur_start - last_end));
                last_end = cur_end;
                cur_start = start;
                cur_end = end;
                case_count=0;
                cur_patch.clear();

                cur_patch.push_back(original_str[it2->second]);
                for (std::map<std::string,bool>::iterator patch_it=res1[it2->second].begin();
                        patch_it!=res1[it2->second].end();patch_it++){
                    std::string patch=patch_it->first;

                    if (patch_it->second)
                        patch = "    " + indentPatch(patch, "    ");
                    cur_patch.push_back(patch);
                }
            }
            //printf("current patch: %s\n",cur_patch.c_str());
        }
        std::string last_code=resCodeSegs[src_file][resCodeSegs[src_file].size()-1];
        resCodeSegs[src_file].pop_back();
        assert(code.find(last_code)!=std::string::npos);
        size_t seg_start=code.find(last_code);
        int case_count=0;

        resCodeSegs[src_file].push_back(code.substr(seg_start,cur_start-seg_start));

        std::string body="switch(__choose(\"__ID"+std::to_string(counter++)+"\"))\n{\n";
        body+="case "+std::to_string(case_count++)+": {\n";
        body+=cur_patch[0];
        int isNegCount=addIsNeg(counter-1,0,cur_patch[0]);
        body+="\nbreak;\n}\n";
        for(int i=1;i<cur_patch.size();i++){
            isNegCount=getIsNegCount(cur_patch[i]);
            if (isNegCount>=2) continue;
            isNegCount=addIsNeg(counter-1,case_count,cur_patch[i]);

            body+="#ifdef COMPILE_"+std::to_string(index)+"\n";
            body+="case "+std::to_string(case_count)+": {\n";
            body+=cur_patch[i];
            body+="\nbreak;\n}\n";
            body+="#endif\n";
            macroMap.insert(std::pair<long long,std::pair<int,int>>(index++,std::pair<int,int>(counter-1,case_count)));
            idAndCase.insert(std::pair<std::string,std::pair<int,int>>(cur_patch[i],std::pair<int,int>(counter-1,case_count++)));
        }
        body+="}\n";
        resPatches[src_file].push_back(body);
        resCodeSegs[src_file].push_back(code.substr(cur_end,code.size()-cur_end));


        // if (cur_start != -1) {
        //     resCodeSegs[src_file].push_back(code.substr(last_end, cur_start - last_end));
        //     resPatches[src_file].push_back(cur_patch);
        //     resCodeSegs[src_file].push_back(code.substr(cur_end));
        // }
    }
}
