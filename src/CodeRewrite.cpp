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
            // 원래 __is_neg를 efi로 넘어온 Stmt로 변경
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
static std::pair<size_t,size_t> getStartEndLine(SourceContextManager &M, const ASTLocTy &loc){
    ASTContext *Context = M.getSourceContext(loc.filename);
    SourceManager &manager = Context->getSourceManager();
    unsigned start_idx = manager.getExpansionLineNumber(loc.stmt->getBeginLoc());
    unsigned end_idx = manager.getExpansionLineNumber(loc.stmt->getEndLoc());
    return std::make_pair(start_idx-1,end_idx-1);
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
    for (size_t i=0;i<code.size()-1;i++){
        if (code[i]=='/' && code[i+1]=='/'){
            size_t j=i+2;
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
    for (size_t i=0;i<code.size();i++){
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
std::string addLocationInIsNeg(std::string code,int id,int case_num){
    size_t position=code.find("__is_neg");

    if (position!=std::string::npos){
        position=code.find("(",position);
        code=code.erase(position+1,2);

        std::string location;
        location="\""+std::to_string(id)+"-"+std::to_string(case_num)+"\"";
        code=code.insert(position+1,location);
    }
    return code;
}
RepairCandidate::CandidateKind getCandidateKind(std::string code){
    size_t position=code.find("__is_neg");
    if (code[position-1]=='(') return RepairCandidate::CandidateKind::IfExitKind;
    else {
        if (code.find("&&",position-7)!=std::string::npos) return RepairCandidate::CandidateKind::TightenConditionKind;
        else if (code.find("||",position-7)!=std::string::npos) return RepairCandidate::CandidateKind::LoosenConditionKind;
        else return RepairCandidate::CandidateKind::GuardKind;
    }
}
size_t CodeRewriter::addIsNeg(int id,int case_num,std::string code){
    size_t position=code.find("__is_neg");
    size_t count=0;

    std::pair<int,int> location(id,case_num);
    isNegLocation[location].clear();

    while (position!=std::string::npos){
        count++;
        position=code.find("(",position);
        std::pair<int,int> location(id,case_num);
        IsNegInformation info(id,case_num,count,getCandidateKind(code.substr(position-10)));
        isNegLocation[location].push_back(info);

        position=code.find("__is_neg",position);
    }
    return count;
}
std::pair<size_t,size_t> getConditionLocation(std::string ifCode){
    size_t nextLine=0;
    if (ifCode.substr(0,2)=="//"){
        nextLine=ifCode.find("\n");
        ifCode=ifCode.substr(nextLine+1);
    }

    size_t start=ifCode.find("(");
    size_t count=0;
    size_t end=start;
    for (size_t i=start+1;i<ifCode.size();i++){
        if (ifCode[i]=='(') count++;
        else if (ifCode[i]==')'){
            if (count>0) count--;
            else {
                end=i;
                break;
            }
        }
    }
    return std::pair<size_t,size_t>(start+nextLine,end+nextLine+1);
}

std::map<ASTLocTy, std::vector<std::map<std::string, RepairCandidate::CandidateKind>> > CodeRewriter::eliminateAllNewLoc(SourceContextManager &M,
        const std::vector<RepairCandidate> &rc,std::map<ASTLocTy,std::string> &original_str) {
    original_str.clear();
    // We first construct the list of stmt close to each original ASTLocTy
    // and we store it to tmp_map1
    std::map<ASTLocTy, std::vector<std::map<std::string,RepairCandidate::CandidateKind>>> ret;
    ret.clear();
    for (size_t j=0;j<rc.size();j++){
        if (DeclStmt::classof(rc[j].original)) continue;

        const ASTLocTy rootLoc = rc[j].actions[0].loc;
        std::map<std::string,RepairCandidate::CandidateKind> replaceCodes;
        std::map<std::string,RepairCandidate::CandidateKind> conditionCodes;
        std::map<std::string,RepairCandidate::CandidateKind> insertCodes;
        std::map<std::string,RepairCandidate::CandidateKind> insertAfterCodes;
        replaceCodes.clear();
        conditionCodes.clear();
        insertCodes.clear();
        insertAfterCodes.clear();
        for (size_t i = 0; i < rc[j].actions.size(); i++) {
            ASTContext *ctxt = M.getSourceContext(rootLoc.filename);
            if (DeclStmt::classof(rootLoc.stmt)) continue;

            std::pair<size_t, size_t> offset_pair = getStartEndOffset(M, rootLoc.filename, rootLoc);
            std::string code=M.getSourceCode(rootLoc.filename);
            std::string original=code.substr(offset_pair.first,offset_pair.second-offset_pair.first);

            if (original_str[rootLoc].size()==0){
                original_str[rootLoc].clear();
                original_str[rootLoc]=original;
            }
            std::vector<std::string> tmp_vector;
            tmp_vector.clear();
            Stmt* S = (Stmt*) rc[j].actions[i].ast_node;
            
            // FIXME: Remove terrible Stmt starts with __is_neg, remove it at patch generation!
            if (stmtToString(*ctxt,S).substr(0,8)=="__is_neg") continue;

            if (rc[j].actions[i].kind == RepairAction::ReplaceMutationKind){
                std::string newStmt="//"+toString(rc[j])+"\n";
                if (rc[j].kind==RepairCandidate::TightenConditionKind || rc[j].kind==RepairCandidate::LoosenConditionKind){
                    newStmt+=stmtToString(*ctxt,S);
                    if (newStmt[newStmt.size() - 1]  != '\n' && newStmt[newStmt.size() - 1] != ';')
                        newStmt += ";\n";
                    conditionCodes.insert(std::pair<std::string,RepairCandidate::CandidateKind>(newStmt,rc[j].kind));
                }
                else{
                    newStmt+=stmtToString(*ctxt,S);
                    if (newStmt[newStmt.size() - 1]  != '\n' && newStmt[newStmt.size() - 1] != ';')
                        newStmt += ";\n";
                    replaceCodes.insert(std::pair<std::string,RepairCandidate::CandidateKind>(newStmt,rc[j].kind));
                }
            }
            else if (rc[j].actions[i].kind == RepairAction::InsertMutationKind){
                std::string newStmt="//"+toString(rc[j])+"\n"+stmtToString(*ctxt,S);
                if (newStmt[newStmt.size() - 1]  != '\n' && newStmt[newStmt.size() - 1] != ';')
                    newStmt += ";\n";
                insertCodes.insert(std::pair<std::string,RepairCandidate::CandidateKind>(newStmt,rc[j].kind));
            }
            else if (rc[j].actions[i].kind == RepairAction::InsertAfterMutationKind){
                std::string newStmt="//"+toString(rc[j])+"\n"+stmtToString(*ctxt,S);
                if (newStmt[newStmt.size() - 1]  != '\n' && newStmt[newStmt.size() - 1] != ';')
                    newStmt += ";\n";
                insertAfterCodes.insert(std::pair<std::string,RepairCandidate::CandidateKind>(newStmt,rc[j].kind));
            }
        }
        if (ret[rootLoc].size()==0){
            ret[rootLoc].push_back(replaceCodes);
            ret[rootLoc].push_back(conditionCodes);
            ret[rootLoc].push_back(insertCodes);
            ret[rootLoc].push_back(insertAfterCodes);
        }
        else{
            assert(ret[rootLoc].size()==4);
            ret[rootLoc][0].insert(replaceCodes.begin(),replaceCodes.end());
            ret[rootLoc][1].insert(conditionCodes.begin(),conditionCodes.end());
            ret[rootLoc][2].insert(insertCodes.begin(),insertCodes.end());
            ret[rootLoc][3].insert(insertAfterCodes.begin(),insertAfterCodes.end());
        }
    }
    return ret;
}

std::string CodeRewriter::applyPatch(size_t &currentIndex,std::vector<std::pair<size_t,size_t>> &currentLocation,std::vector<ASTLocTy> &currentCandidate,
        std::map<ASTLocTy, std::vector<std::map<std::string, RepairCandidate::CandidateKind>>> &res1,std::map<ASTLocTy,std::pair<size_t,size_t>> &line,const std::string code){
    // if (currentLocation[currentIndex].second==0) return "";
    size_t start = currentLocation[currentIndex].first;
    size_t end = currentLocation[currentIndex].second;
    ASTLocTy loc=currentCandidate[currentIndex];
    // outlog_printf(2,"Location: %d %d\n",start,end);
    std::vector<size_t> currentSwitches;
    currentSwitches.clear();

    int case_count=0;
    // assert( end >= last_end);
    std::map<int,std::string> casePatch;
    casePatch.clear();
    std::map<RepairCandidate::CandidateKind,std::list<int>> caseKind;
    caseKind.clear();
    std::list<int> switchGroup;
    switchGroup.clear();
    std::string body="{\n";

    // Insert before
    if(res1[currentCandidate[currentIndex]][2].size()>0){
        currentSwitches.push_back(counter);
        body+="switch(__choose(\""+workDir+"/switch.txt\","+std::to_string(counter)+"))\n{\n";
        body+="case "+std::to_string(case_count++)+": \n";
        body+="break;\n";

        for (std::map<std::string,RepairCandidate::CandidateKind>::iterator patch_it=res1[currentCandidate[currentIndex]][2].begin();
                patch_it!=res1[currentCandidate[currentIndex]][2].end();patch_it++){
            // size_t isNegCount=getIsNegCount(patch_it->first);
            // if (isNegCount>=2) continue;
            // isNegCount=addIsNeg(counter,case_count,patch_it->first);

            body+="#ifdef COMPILE_"+std::to_string(index)+"\n";
            body+="case "+std::to_string(case_count)+": {\n";
            std::string currentBody=addLocationInIsNeg(patch_it->first,counter,case_count);
            body+=currentBody;
            casePatch[case_count]=currentBody;
            body+="\nbreak;\n}\n";
            body+="#endif\n";

            macroMap.insert(std::pair<long long,std::pair<int,int>>(index,std::pair<int,int>(counter,case_count)));
            macroCode[index]=currentBody;
            index++;
            caseKind[patch_it->second].push_back(case_count);

            case_count++;
        }
        body+="}\n";

        idAndCase[counter]=casePatch;
        for(std::map<RepairCandidate::CandidateKind,std::list<int>>::iterator kindIt=caseKind.begin();
                kindIt!=caseKind.end();kindIt++){
            caseCluster[counter].push_back(kindIt->second);
        }

        switchGroup.push_back(counter);
        counter++;
    }

    size_t conditionCounter=counter;
    ASTContext *ctxt = sourceManager.getSourceContext(loc.filename);
    // Condition Synthesize
    if(res1[currentCandidate[currentIndex]][1].size()>0){
        case_count=0;
        currentSwitches.push_back(counter);
        std::string subPatch=stmtToString(*ctxt,loc.stmt);
        std::pair<size_t,size_t> conditionLoc=getConditionLocation(subPatch);
        // body+="{\n"+conditionTypes[currentCandidate[currentIndex]].getAsString()+" __temp"+std::to_string(counter)+"="+subPatch.substr(conditionLoc.first,conditionLoc.second-conditionLoc.first+1)+";\n";
        body+="{\nlong long __temp"+std::to_string(counter)+"="+subPatch.substr(conditionLoc.first,conditionLoc.second-conditionLoc.first+1)+";\n";
        body+="switch(__choose(\""+workDir+"/switch.txt\","+std::to_string(counter)+"))\n{\n";
        body+="case "+std::to_string(case_count++)+": {\n";
        // body+="__temp"+std::to_string(counter)+"="+subPatch.substr(conditionLoc.first,conditionLoc.second-conditionLoc.first+1)+";\n";
        body+="\nbreak;\n}\n";

        for (std::map<std::string,RepairCandidate::CandidateKind>::iterator patch_it=res1[currentCandidate[currentIndex]][1].begin();
                patch_it!=res1[currentCandidate[currentIndex]][1].end();patch_it++){
            // outlog_printf(2,"%s\n",patch_it->first.c_str());
            // size_t isNegCount=getIsNegCount(patch_it->first);
            // if (isNegCount>=2) continue;
            // isNegCount=addIsNeg(counter,case_count,patch_it->first);

            body+="#ifdef COMPILE_"+std::to_string(index)+"\n";
            body+="case "+std::to_string(case_count)+": {\n";
            std::string currentBody=addLocationInIsNeg(patch_it->first,counter,case_count);
            std::pair<size_t,size_t> conditionLoc=getConditionLocation(currentBody);
            body+="__temp"+std::to_string(counter)+"="+currentBody.substr(conditionLoc.first,conditionLoc.second-conditionLoc.first+1)+";\n";
            // outlog_printf(2,"%s\n\n",currentBody.substr(conditionLoc.first,conditionLoc.second-conditionLoc.first+1).c_str());
            casePatch[case_count]=currentBody;
            body+="\nbreak;\n}\n";
            body+="#endif\n";

            macroMap.insert(std::pair<long long,std::pair<int,int>>(index,std::pair<int,int>(counter,case_count)));
            macroCode[index]=currentBody;
            index++;
            caseKind[patch_it->second].push_back(case_count);

            case_count++;
        }
        body+="}\n";

        idAndCase[counter]=casePatch;
        for(std::map<RepairCandidate::CandidateKind,std::list<int>>::iterator kindIt=caseKind.begin();
                kindIt!=caseKind.end();kindIt++){
            caseCluster[counter].push_back(kindIt->second);
        }

        switchGroup.push_back(counter);
        counter++;
    }

    // Apply sub-patches to original body
    std::string origBody="";
    size_t indexBackup=currentIndex;
    std::vector<std::string> bodyCodes;
    bodyCodes.clear();
    if (res1[currentCandidate[indexBackup]][1].size()>0 && code.substr(start,2)!="if")
        bodyCodes.push_back(stmtToString(*ctxt,loc.stmt));
    else
        bodyCodes.push_back(code.substr(start,end-start));
    size_t beforeEnd=start;
    while (currentIndex+1<currentLocation.size() && currentLocation[currentIndex+1].second<=end){
        currentIndex++;
        size_t beforeIndex=currentIndex;
        std::string subPatch=applyPatch(currentIndex,currentLocation,currentCandidate,res1,line,code);

        std::string last=bodyCodes[bodyCodes.size()-1];
        bodyCodes.pop_back();
        bodyCodes.push_back(last.substr(0,currentLocation[beforeIndex].first-beforeEnd));
        bodyCodes.push_back(subPatch);
        bodyCodes.push_back(last.substr(currentLocation[beforeIndex].second-beforeEnd));
        beforeEnd=currentLocation[beforeIndex].second;
    }
    for (size_t i=0;i<bodyCodes.size();i++){
        origBody+=bodyCodes[i];
    }
    if(res1[currentCandidate[indexBackup]][1].size()>0){
        std::pair<size_t,size_t> conditionLoc=getConditionLocation(origBody);
        origBody=origBody.replace(conditionLoc.first+1,conditionLoc.second-conditionLoc.first-2,"__temp"+std::to_string(conditionCounter));
    }

    // Normal replace
    if(res1[currentCandidate[currentIndex]][0].size()>0 && origBody!="break;\n"){
        case_count=0;
        currentSwitches.push_back(counter);
        body+="switch(__choose(\""+workDir+"/switch.txt\","+std::to_string(counter)+"))\n{\n";
        body+="case "+std::to_string(case_count++)+": {\n";
        body+=origBody;
        body+="\nbreak;\n}\n";

        for (std::map<std::string,RepairCandidate::CandidateKind>::iterator patch_it=res1[currentCandidate[currentIndex]][0].begin();
                patch_it!=res1[currentCandidate[currentIndex]][0].end();patch_it++){
            // size_t isNegCount=getIsNegCount(patch_it->first);
            // if (isNegCount>=2) continue;
            // isNegCount=addIsNeg(counter,case_count,patch_it->first);

            body+="#ifdef COMPILE_"+std::to_string(index)+"\n";
            body+="case "+std::to_string(case_count)+": {\n";
            std::string currentBody=addLocationInIsNeg(patch_it->first,counter,case_count);
            body+=currentBody;
            casePatch[case_count]=currentBody;
            body+="\nbreak;\n}\n";
            body+="#endif\n";

            macroMap.insert(std::pair<long long,std::pair<int,int>>(index,std::pair<int,int>(counter,case_count)));
            macroCode[index]=currentBody;
            index++;
            caseKind[patch_it->second].push_back(case_count);

            case_count++;
        }
        body+="}\n";

        idAndCase[counter]=casePatch;
        for(std::map<RepairCandidate::CandidateKind,std::list<int>>::iterator kindIt=caseKind.begin();
                kindIt!=caseKind.end();kindIt++){
            caseCluster[counter].push_back(kindIt->second);
        }

        switchGroup.push_back(counter);
        counter++;
    }
    else{
        body+=origBody;
    }
    if(res1[currentCandidate[indexBackup]][1].size()>0){
        body+="}\n";
    }


    // Insert after
    if(res1[currentCandidate[currentIndex]][3].size()>0){
        case_count=0;
        currentSwitches.push_back(counter);
        body+="switch(__choose(\""+workDir+"/switch.txt\","+std::to_string(counter)+"))\n{\n";
        body+="case "+std::to_string(case_count++)+": {\n";
        body+="\nbreak;\n}\n";

        for (std::map<std::string,RepairCandidate::CandidateKind>::iterator patch_it=res1[currentCandidate[currentIndex]][3].begin();
                patch_it!=res1[currentCandidate[currentIndex]][3].end();patch_it++){
            // size_t isNegCount=getIsNegCount(patch_it->first);
            // if (isNegCount>=2) continue;
            // isNegCount=addIsNeg(counter,case_count,patch_it->first);

            body+="#ifdef COMPILE_"+std::to_string(index)+"\n";
            body+="case "+std::to_string(case_count)+": {\n";
            std::string currentBody=addLocationInIsNeg(patch_it->first,counter,case_count);
            body+=currentBody;
            casePatch[case_count]=currentBody;
            body+="\nbreak;\n}\n";
            body+="#endif\n";

            macroMap.insert(std::pair<long long,std::pair<int,int>>(index,std::pair<int,int>(counter,case_count)));
            macroCode[index]=currentBody;
            index++;
            caseKind[patch_it->second].push_back(case_count);

            case_count++;
        }
        body+="}\n";

        idAndCase[counter]=casePatch;
        for(std::map<RepairCandidate::CandidateKind,std::list<int>>::iterator kindIt=caseKind.begin();
                kindIt!=caseKind.end();kindIt++){
            caseCluster[counter].push_back(kindIt->second);
        }

        switchGroup.push_back(counter);
        counter++;
    }
    
    for (size_t i=0;i<currentSwitches.size();i++)
        switchLineMap[loc.filename][line[loc]].push_back(currentSwitches[i]);

    return body+"}\n";
}

CodeRewriter::CodeRewriter(SourceContextManager &M, const std::vector<RepairCandidate> &rc, std::vector<std::set<ExprFillInfo> *> *pefi,std::map<std::string,std::map<FunctionDecl*,std::pair<unsigned,unsigned>>> functionLoc,std::string work_dir):sourceManager(M) {
    workDir=work_dir;
    std::map<ASTLocTy,std::string> original_str;
    std::vector<RepairCandidate> rc1;
    rc1.clear();
    for (size_t i=0;i<rc.size();i++){

        std::vector<ExprFillInfo> temp((*pefi)[i]->begin(),(*pefi)[i]->end());
        for(size_t k=0;k<temp.size();k++){
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
            // rc1.push_back(replaceExprInCandidate(M, rc[i], efi));
            //rc1.dump();
            // We then eliminate ASTLocTy with new statements, and replace them with
            // strings
            //std::map<ASTLocTy, std::pair<std::string, bool> > tmp=eliminateAllNewLoc(M, rc[i]);
            //res1.insert(tmp.begin(),tmp.end());
        }
        rc1.push_back(rc[i]);
    }

    // Create string with whole candidate vector 
    std::map<ASTLocTy, std::vector<std::map<std::string, RepairCandidate::CandidateKind>>> res1=eliminateAllNewLoc(M, rc1,original_str);
    // We then categorize location based on the src_file and their offset
    std::map<std::string, std::map<std::pair<size_t, size_t>, ASTLocTy> > res2;
    std::map<std::string, std::map<std::pair<size_t, size_t>, ASTLocTy> > tmp_loc;
    std::map<std::string, std::map<ASTLocTy,std::pair<size_t,size_t>>> line;
    res2.clear();
    tmp_loc.clear();
    line.clear();
    for (std::map<ASTLocTy, std::vector<std::map<std::string, RepairCandidate::CandidateKind>>>::iterator it = res1.begin();
            it != res1.end(); ++it) {
        //llvm::errs() << "Location: " << it->first.toString(M) << "\n";
        //llvm::errs() << "patch: " << it->second.first << "\n";
        //llvm::errs() << "braket: " << it->second.second << "\n";
        ASTLocTy loc = it->first;
        std::string src_file = loc.src_file;
        if (tmp_loc.count(src_file) == 0)
            tmp_loc[src_file].clear();
        std::pair<size_t, size_t> offset_pair = getStartEndOffset(M, src_file, loc);
        // llvm::errs() << "offset: " << offset_pair.first << " " << offset_pair.second << "\n";
        // We reverse the start and end for sorting purpose
        tmp_loc[src_file].insert(std::make_pair(std::make_pair(offset_pair.first,
                        offset_pair.second), loc));
        line[src_file][loc]=getStartEndLine(M,loc);
    }

    // Sorting res2 with start location
    std::map<std::string,std::vector<std::pair<size_t,size_t>>> location;
    std::map<std::string,std::vector<ASTLocTy>> candidate;
    location.clear();
    candidate.clear();
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

            for (size_t i=1;i<locationVec.size();i++) {
                std::pair<size_t,size_t> temp=locationVec[i];
                ASTLocTy temp2=valueVec[i];
                int j;
                for (j=i;j>=1 && locationVec[j-1].first>=temp.first;j--){
                    if (locationVec[j-1].first==temp.first)
                        if (locationVec[j-1].second<temp.second)
                            continue;
                    locationVec[j]=locationVec[j-1];
                    valueVec[j]=valueVec[j-1];
                }
                locationVec[j]=temp;
                valueVec[j]=temp2;
            }

            location[it->first]=locationVec;
            candidate[it->first]=valueVec;
        }
    }
    outlog_printf(2,"Generating patches...\n");
    // Then we handle each source file saperately
    resCodeSegs.clear();
    resPatches.clear();
    macroMap.clear();
    idAndCase.clear();
    switchCluster.clear();
    switchLineMap.clear();
    index=0;
    counter=0;
    for (std::map<std::string,std::vector<std::pair<size_t,size_t>>>::iterator
            it = location.begin(); it != location.end(); ++it) {
    // for (std::map<std::string, std::map<std::pair<size_t, size_t>, ASTLocTy> >::iterator
    //         it = tmp_loc.begin(); it != tmp_loc.end(); ++it) {
        std::string src_file = it->first;
        resCodeSegs[src_file].clear();
        resPatches[src_file].clear();
        std::string code = M.getSourceCode(src_file);
        // outlog_printf(2,"Code size: %d\n",code.size());
        resCodeSegs[src_file].push_back(code);
        std::vector<std::pair<size_t,size_t>> currentLocation=location[src_file];
        std::vector<ASTLocTy> currentCandidate=candidate[src_file];
        long long cur_start = -1;
        long long cur_end = -1;
        long long last_end = 0;
        for (size_t i=0;i<it->second.size();i++) {
        // for (std::map<std::pair<size_t, size_t>, ASTLocTy>::iterator it2=it->second.begin();
        //         it2!=it->second.end();it2++) {
            // if (it2->first.second==0) continue;
            if (currentLocation[i].second==0) continue;
            // long long start = it2->first.second;
            // long long end = it2->first.first;
            size_t start = currentLocation[i].first;
            size_t end = currentLocation[i].second;
            // outlog_printf(2,"Location: %d %d\n",start,end);

            int case_count=0;
            // assert( end >= last_end);
            std::map<int,std::string> casePatch;
            casePatch.clear();
            std::map<RepairCandidate::CandidateKind,std::list<int>> caseKind;
            caseKind.clear();
            std::list<int> switchGroup;
            switchGroup.clear();
            
            std::string last_code=resCodeSegs[src_file][resCodeSegs[src_file].size()-1];
            resCodeSegs[src_file].pop_back();
            assert(code.find(last_code)!=std::string::npos);
            size_t seg_start=code.find(last_code);

            resCodeSegs[src_file].push_back(code.substr(seg_start,start-seg_start));

            std::string body=applyPatch(i,currentLocation,currentCandidate,res1,line[src_file],code);

            // Add patch code to code
            resPatches[src_file].push_back(body);
            resCodeSegs[src_file].push_back(code.substr(end));
            switchCluster.push_back(switchGroup);
        }
    }
}
