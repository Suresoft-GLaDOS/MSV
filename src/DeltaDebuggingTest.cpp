
#include "DeltaDebuggingTest.h"

#define GCC_CMD "/usr/bin/gcc"
#define CLANG_TEST_WRAP "tclang.py"


std::pair<std::vector<long long>,std::vector<long long>> DeltaDebuggingTest::slice(std::vector<long long> target){
    std::vector<long long> first;
    first.clear();
    std::vector<long long> second;
    second.clear();

    for (int i=0;i<(target.size()/2);i++){
        first.push_back(target[i]);
    }
    for (int i=target.size()/2;i<target.size();i++){
        second.push_back(target[i]);
    }
    return std::pair<std::vector<long long>,std::vector<long long>>(first,second);
}
std::vector<long long> DeltaDebuggingTest::removeAll(std::vector<long long> target,std::vector<long long> operand){
    std::vector <long long> result;
    for (int i=0;i<target.size();i++){
        bool compare=false;
        for (int j=0;j<operand.size();j++){
            if (target[i]==operand[j]) {
                compare=true;
                break;
            }
        }
        if (!compare) result.push_back(target[i]);
    }
    return result;
}
bool DeltaDebuggingTest::includeAll(std::vector<long long> target,std::vector<long long> operand){
    size_t total=0;
    for (int i=0;i<target.size();i++){
        for (int j=0;j<operand.size();j++){
            if (target[i]==operand[j]) total++;
        }
    }
    if (total==operand.size()) return true;
    else return false;
}
bool DeltaDebuggingTest::successAlready(std::vector<long long> operand){
    for (int i=0;i<succ_case.size();i++){
        if (includeAll(succ_case[i],operand)) return true;
    }
    return false;
}
void DeltaDebuggingTest::updateToBigger(std::vector<long long> target){
    for (std::vector<std::vector<long long>>::iterator it=succ_case.begin();it!=succ_case.end();it++){
        if(includeAll(target,*it)) succ_case.erase(it);
    }
    succ_case.push_back(target);
}


DeltaDebuggingTest::DeltaDebuggingTest(const std::map<std::string, std::string> source,BenchProgram &program,BenchProgram::EnvMapTy buildEnv,std::vector<long long> fullMacros): 
        source(source),fullMacros(fullMacros),succ_case(),fail_case(),
        isFinish(false),program(program),buildEnv(buildEnv){}

bool DeltaDebuggingTest::testCase(std::vector<long long> cases){
    return program.buildWithRepairedCode(CLANG_TEST_WRAP, buildEnv,
        source,cases);
}

void DeltaDebuggingTest::test(std::vector<long long> macros){
    if (macros.size()<=1) return;
    // If success vectors include this vector, pass it
    if (!successAlready(macros)){
        bool result=testCase(macros);
        if (result) {
            // If success, add in success case
            updateToBigger(macros);
        }
        else{
            // If fail, run recursive with more slice
            std::pair<std::vector<long long>,std::vector<long long>> sliced=slice(macros);
            test(sliced.first);
            test(sliced.second);
        }
    }

    // Remove current vector from full vector
    std::vector<long long> removed=removeAll(fullMacros,macros);

    if (removed.size()<=1) return;
    if (!successAlready(removed)){
        bool result=testCase(removed);
        if (result) {
            updateToBigger(removed);
        }
        else{
            std::pair<std::vector<long long>,std::vector<long long>> sliced=slice(removed);
            test(sliced.first);
            test(sliced.second);
        }
    }
}

void DeltaDebuggingTest::run(){
    test(fullMacros);
}
std::vector<std::vector<long long>> DeltaDebuggingTest::getResult(){
    return succ_case;
}