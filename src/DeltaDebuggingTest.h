#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include "BenchProgram.h"

using namespace clang;

class DeltaDebuggingTest{
    const std::map<std::string, std::string> source;
    BenchProgram &program;
    std::vector<long long> fullMacros;
    std::vector<std::vector<long long>> succ_case;
    std::vector<std::vector<long long>> fail_case;
    bool isFinish;

    BenchProgram::EnvMapTy buildEnv;

    bool testCase(std::vector<long long> cases);
    std::pair<std::vector<long long>,std::vector<long long>> slice(std::vector<long long> target);
    std::vector<long long> removeAll(std::vector<long long> target,std::vector<long long> operand);
    bool includeAll(std::vector<long long> target,std::vector<long long> operand);
    bool successAlready(std::vector<long long> operand);
    void updateToBigger(std::vector<long long> target);
public:
    DeltaDebuggingTest(const std::map<std::string, std::string> source,BenchProgram &program,BenchProgram::EnvMapTy buildEnv,std::vector<long long> fullMacros);
    void test(std::vector<long long> macros);
    void run();
    std::vector<std::vector<long long>> getResult();
};