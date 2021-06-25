#pragma once
#include "BenchProgram.h"

#define MAGIC_NUMBER -123456789

namespace clang{

/**
 * Class for collect infos for condition synthesize.
 * For parallel running.
 */
class CollectCondition{
private:
    BenchProgram::EnvMapTy env;
    std::vector<std::pair<size_t,size_t>> &conditionLocation;
    std::map<std::pair<size_t,size_t>,std::map<size_t,std::vector<size_t>>> records;
    size_t switchNum;
    size_t totalProcess;
    BenchProgram &program;
    BenchProgram::TestCaseSetTy &negative_cases;
    BenchProgram::TestCaseSetTy &positive_cases;

    std::map<std::pair<size_t,size_t>,int> pids;
    std::map<std::pair<size_t,size_t>,int> readPipes;

    std::vector<unsigned long> parseBranchRecord(std::string tmpFile);
    void writeBranchRecordTerminator(std::string tmpFile);
    void writeBranchRecord(std::string tmpFile,const std::map<unsigned long,std::vector<unsigned long>> &negative_records,unsigned long case_id);
    void parseValueRecord(std::string recordFile,std::vector<std::vector<long long> > &vec);

    void record(std::pair<size_t,size_t> currentPatch,int outPipe);
    void collect(std::pair<size_t,size_t> currentPatch,std::map<size_t,std::vector<size_t>> currentRecord,unsigned long currentCase,int outPipe,bool isNegative);

public:
    CollectCondition(BenchProgram::EnvMapTy env,std::vector<std::pair<size_t,size_t>> &location, size_t switchNum,
        BenchProgram &program,BenchProgram::TestCaseSetTy &negative_cases,
        BenchProgram::TestCaseSetTy &positive_cases,size_t totalProcess=1): env(env),conditionLocation(location),records(),switchNum(switchNum),program(program),negative_cases(negative_cases),positive_cases(positive_cases),totalProcess(totalProcess),pids(),readPipes() { records.clear(); }
    void getConditionRecord();
    bool collectValues(std::map<std::pair<size_t,size_t>,std::map<unsigned long,std::vector<std::vector<long long>>>> &caseVMap);
    std::map<std::pair<size_t,size_t>,std::map<size_t,std::vector<size_t>>> getRecords(){
        return records;
    }
}; // fin class CollectCondition

} // fin namespace clang