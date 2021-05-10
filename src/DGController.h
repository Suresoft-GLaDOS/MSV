#pragma once
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>

#include "Utils.h"
#include "dg/llvm/PointerAnalysis/PointerAnalysis.h"
#include "dg/llvm/PointerAnalysis/LLVMPointerAnalysisOptions.h"

#include "dg/llvm/LLVMSlicer.h"
#include "dg/llvm/LLVMDependenceGraph.h"
#include "dg/llvm/LLVMDependenceGraphBuilder.h"
#include "dg/llvm/LLVMNode.h"

namespace clang{
llvm::Module* createModule(std::string file);
dg::LLVMDependenceGraph* createDG(llvm::Module* module);

class Slicer{
    std::string file;
    dg::llvmdg::LLVMSlicer slicer;
    dg::LLVMDependenceGraph *dependenceGraph;
    std::vector<dg::LLVMNode*> criteriaList;
public:
    Slicer(std::string file,dg::LLVMDependenceGraph *graph): file(file),dependenceGraph(graph), slicer(),criteriaList(){}
    void addCriteria(dg::LLVMNode *criteria){
        criteriaList.push_back(criteria);
    }
    void run();
};
} // fin namespace dg