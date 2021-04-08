#include "DGController.h"

using namespace clang;

llvm::Module *createModule(std::string file){
    llvm::LLVMContext context;
    llvm::SMDiagnostic SMD;

    std::unique_ptr<llvm::Module> M = llvm::parseIRFile(file, SMD, context);
    llvm::Module *mod=M.get();

    return mod;
}
dg::LLVMDependenceGraph* createDG(llvm::Module* module){
    dg::llvmdg::LLVMDependenceGraphBuilder builder(module);

    return builder.build().get();
}

void Slicer::run(){
    dg::LLVMNode *start=dependenceGraph->getEntry();
    uint32_t slice_id = 0xdead;
    slicer.slice(dependenceGraph,start,slice_id);
}