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
#include "RepairCandidateGenerator.h"
#include <queue>
#include <map>
#include <algorithm>

namespace clang {
    class Expr;
    class Stmt;
    class FunctionDecl;
}

class FeatureParameter;

typedef std::map<size_t, clang::Expr*> ExprFillInfo;
typedef std::map<std::string, std::string> NewCodeMapTy;
typedef std::vector<std::pair<NewCodeMapTy, double> > ExprSynthesizerResultTy;

class ExprSynthesizer {
    BenchProgram &P;
    SourceContextManager &M;
    size_t tested_cnt;
    RepairCandidateQueue &q;
    bool naive;
    bool learning;
    FeatureParameter *FP;
    unsigned long long timeout_limit;
    std::string fixedFile;
    std::map<std::string,std::map<clang::FunctionDecl*,std::pair<unsigned,unsigned>>> functionLoc;
    std::map<std::string,std::map<std::string,std::map<size_t,std::string>>> mutationInfo;
    std::map<std::pair<std::string,size_t>,size_t> &scores;
public:
    ExprSynthesizer(BenchProgram &P, SourceContextManager &M,
            RepairCandidateQueue &q, std::string fixedFile,std::map<std::string,std::map<clang::FunctionDecl*,std::pair<unsigned,unsigned>>> functionLoc,
            std::map<std::pair<std::string,size_t>,size_t> &scores,
            bool naive, bool learning, FeatureParameter *FP):
        P(P), M(M), tested_cnt(0), q(q), fixedFile(fixedFile),naive(naive), learning(learning && !naive), FP(FP),functionLoc(functionLoc),
        timeout_limit(0),scores(scores) { }
    
    void setMutationInfo(std::map<std::string,std::map<std::string,std::map<size_t,std::string>>> &info){
        mutationInfo=info;
    }

    bool workUntil(size_t candidate_limit,
            size_t time_limit, ExprSynthesizerResultTy &res,
            bool full_synthesis, bool quit_with_any);

    size_t getTestedCandidateNumber() {
        return tested_cnt;
    }

    void setTimeoutLimit(unsigned long long limit) {
        timeout_limit = limit;
    }
};