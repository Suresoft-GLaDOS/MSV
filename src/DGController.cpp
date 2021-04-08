#include "DGController.h"

using namespace dg;

PointerAnalysis PointerAnalysis::createPointerAnalysis(std::string file){
    llvm::LLVMContext context;
    llvm::SMDiagnostic SMD;

    std::unique_ptr<llvm::Module> M = llvm::parseIRFile(file, SMD, context);
    llvm::Module *mod=M.get();

    LLVMPointerAnalysisOptions opts;

    return PointerAnalysis(file,mod,opts);
}

void PointerAnalysis::run(){
    outlog_printf(2,"test1\n");
    pta.initialize();
    outlog_printf(2,"test2\n");

    pta.run();
    outlog_printf(2,"test3\n");
}