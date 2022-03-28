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
#pragma once
#include "ExprSynthesizer.h"
#include "BenchProgram.h"
#include <string>

class SourceContextManager;

typedef std::map<size_t, clang::Expr*> ExprFillInfo;
typedef std::map<std::string, std::vector<std::string> > CodeSegTy;
typedef std::map<std::string, std::string> NewCodeMapTy;

NewCodeMapTy mergeCode(const CodeSegTy &codeSegs, const std::vector<CodeSegTy> &patches);

bool canMerge(const CodeSegTy &codeSegs, const CodeSegTy &patches);

NewCodeMapTy combineCode(const CodeSegTy &codeSegs, const CodeSegTy &patch);

class CodeRewriter {
private:
    typedef enum{
        NormalReplace=0,
        ConditionSynthesize,
        NormalInsert,
        AfterInsert,
        VarMutation,
    } ActionType;
    CodeSegTy resCodeSegs, resPatches;
    int counter;
    size_t varMutationCounter=0;
    SourceContextManager &sourceManager;

    std::map<long long,std::pair<int,int>> macroMap;
    std::map<long long,std::string> macroCode;
    std::map<std::string,std::vector<long long>> macroFile;
    std::vector<std::pair<size_t,size_t>> isNegLocation;
    std::map<int,std::map<int,std::string>> idAndCase;
    std::map<size_t,std::vector<clang::Expr *>> switchAtoms;
    std::map<ASTLocTy,std::vector<size_t>> switchLoc;
    // std::map<int,std::list<std::list<int>>> caseCluster;
    std::vector<std::list<size_t>> switchCluster;
    std::string workDir;
    std::map<void*,std::vector<double>> candidateScores;

    std::map<size_t,size_t> switchLine;
    std::map<std::pair<size_t,size_t>,std::string> patchTypes;

    std::map<std::string,std::map<std::pair<size_t,size_t>,std::vector<size_t>>> patchLines;

    std::vector<Line> lines;
    void addIsNeg(size_t id,size_t case_num,std::string code);

    // For mutation
    std::map<std::string,std::map<clang::FunctionDecl *,std::pair<unsigned,unsigned>>> functionLoc; // function locations
    std::map<std::pair<size_t,size_t>,size_t> varSizes;
public:
    int index;
    std::vector<File> rules;
    std::map<size_t,std::map<size_t,std::vector<double>>> patchScores;
    std::map<std::string,std::map<std::string,std::pair<size_t,size_t>>> funcLocation;
    std::vector<FunctionReplaceInfo> funcReplace;
    CodeRewriter(SourceContextManager &M, const std::vector<RepairCandidate> &rc, std::vector<std::set<ExprFillInfo> *> *pefi,
            std::map<std::string,std::map<clang::FunctionDecl*,std::pair<unsigned,unsigned>>> functionLoc=std::map<std::string,std::map<clang::FunctionDecl*,std::pair<unsigned,unsigned>>>(),std::string work_dir="",
            std::map<void*,std::vector<double>> candScores=std::map<void*,std::vector<double>>());

    std::map<ASTLocTy, std::map<CodeRewriter::ActionType,std::map<std::string, RepairCandidate>>> eliminateAllNewLoc(SourceContextManager &M,
        const std::vector<RepairCandidate> &rc,std::map<ASTLocTy,std::string> &original_str);

    std::string applyPatch(size_t &currentIndex,std::vector<std::pair<size_t,size_t>> &currentLocation,std::vector<ASTLocTy> &currentCandidate,
        std::map<ASTLocTy, std::map<CodeRewriter::ActionType,std::map<std::string, RepairCandidate>>> &res1,std::map<ASTLocTy,std::pair<size_t,size_t>> &line,const std::string code);


    CodeSegTy getCodeSegments() {
        return resCodeSegs;
    }

    CodeSegTy getPatches() {
        return resPatches;
    }

    NewCodeMapTy getCodes() {
        return combineCode(resCodeSegs, resPatches);
    }
    std::map<long long,std::pair<int,int>> getMacroMap(){
        return macroMap;
    }
    std::map<int,std::map<int,std::string>> getIdAndCase(){
        return idAndCase;
    }
    int getIdCount(){
        return counter;
    }
    std::vector<std::list<size_t>> getSwitchCluster(){
        return switchCluster;
    }
    // std::map<int,std::list<std::list<int>>> getCaseCluster(){
    //     return caseCluster;
    // }
    std::vector<std::pair<size_t,size_t>> getIsNegLocation(){
        return isNegLocation;
    }
    std::map<long long,std::string> getMacroCode(){
        return macroCode;
    }
    std::map<std::string,std::map<std::pair<size_t,size_t>,std::vector<size_t>>> getPatchLines(){
        return patchLines;
    }
    std::map<size_t,std::vector<clang::Expr *>> getSwitchAtoms(){
        return switchAtoms;
    }
    std::map<ASTLocTy,std::vector<size_t>> getSwitchLoc(){
        return switchLoc;
    }
    std::map<size_t,size_t> getSwitchLine(){
        return switchLine;
    }
    std::map<std::pair<size_t,size_t>,std::string> getPatchTypes(){
        return patchTypes;
    }
    std::map<std::string,std::vector<long long>> getMacroFile(){
        return macroFile;
    }
    std::map<std::pair<size_t,size_t>,size_t> getVarSizes(){
        return varSizes;
    }
};

