#pragma once
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>

#include "Utils.h"
#include "dg/llvm/PointerAnalysis/PointerAnalysis.h"
#include "dg/llvm/PointerAnalysis/LLVMPointerAnalysisOptions.h"

namespace dg{

class PointerAnalysis{
    std::string file;
    DGLLVMPointerAnalysis pta;
protected:
    PointerAnalysis(std::string file,llvm::Module *mod,LLVMPointerAnalysisOptions opts): file(file),pta(mod,opts){}
public:
    static PointerAnalysis createPointerAnalysis(std::string file);
    void run();
    DGLLVMPointerAnalysis &getPTA(){
        return pta;
    }
};
}