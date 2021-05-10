#include "DGController.h"

namespace clang{

llvm::Module *createModule(std::string file){
    llvm::LLVMContext context;
    llvm::SMDiagnostic SMD;

    std::unique_ptr<llvm::Module> M = llvm::parseIRFile(file, SMD, context);
    llvm::Module *mod=M.get();

    return mod;
}
dg::LLVMDependenceGraph* createDG(llvm::Module* module){
    dg::llvmdg::LLVMDependenceGraphBuilder builder(module);
    std::unique_ptr<dg::LLVMDependenceGraph> graph=builder.constructCFGOnly();

    graph=builder.computeDependencies(std::move(graph));

    return graph.get();
}

void Slicer::run(){
    dg::LLVMNode *start=dependenceGraph->getEntry();
    for (dg::LLVMNode *criteria:criteriaList){
        slicer.mark(criteria);
    }
    slicer.slice(dependenceGraph,nullptr);
}
}; // fin namespace clang