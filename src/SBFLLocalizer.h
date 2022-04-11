#include "ErrorLocalizer.h"
#include "ProfileErrorLocalizer.h"
#include "BenchProgram.h"

class SBFLLocalizer: public ErrorLocalizer{
public:
    class ResRecordTy {
    public:
        long long primeScore;
        long long secondScore;
        SourcePositionTy loc;
        std::string pid;
    };

    std::string SBFLFile;
    std::vector<SourcePositionTy> result;
    BenchProgram *program;
    std::vector<ResRecordTy> candidateResults;

public:

    virtual std::vector<SourcePositionTy> getCandidateLocations();

    virtual void printResult(const std::string &outfile);

    SBFLLocalizer(std::string fileName,BenchProgram *program): SBFLFile(fileName),program(program),result() {}
    virtual ~SBFLLocalizer() { };
};