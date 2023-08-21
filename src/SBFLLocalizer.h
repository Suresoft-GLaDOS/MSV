#include "ErrorLocalizer.h"
#include "ProfileErrorLocalizer.h"
#include "BenchProgram.h"

class SBFLLocalizer: public ErrorLocalizer{
public:
    std::string SBFLFile;
    std::vector<std::pair<double,SourcePositionTy>> result;
    BenchProgram *program;
    std::vector<ProfileErrorLocalizer::ResRecordTy> candidateResults;

public:

    virtual std::vector<SourcePositionTy> getCandidateLocations();
    std::vector<ProfileErrorLocalizer::ResRecordTy> getCandidates();

    virtual void printResult(const std::string &outfile,const std::string backupFile);
    virtual void printResult(const std::string &outfile);

    SBFLLocalizer(std::string fileName,BenchProgram *program);
    virtual ~SBFLLocalizer() { };
};