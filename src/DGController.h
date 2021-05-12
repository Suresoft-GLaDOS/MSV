#pragma once

#include <ctime>
#include <fstream>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_os_ostream.h>

#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Bitcode/BitcodeWriter.h>

#include <clang/Basic/SourceLocation.h>

#include "TimeMeasure.h"
#include "Utils.h"

#include "dg/llvm/PointerAnalysis/PointerAnalysis.h"
#include "dg/llvm/PointerAnalysis/LLVMPointerAnalysisOptions.h"

#include "dg/llvm/LLVMSlicer.h"
#include "dg/llvm/LLVMDependenceGraph.h"
#include "dg/llvm/LLVMDependenceGraphBuilder.h"
#include "dg/llvm/LLVMNode.h"

bool getSlicingCriteriaNodes(dg::LLVMDependenceGraph& dg,
                        const std::string& slicingCriteria,
                        std::set<dg::LLVMNode *>& criteria_nodes,
                        bool criteria_are_next_instr);

namespace clang{

llvm::Module* createModule(std::string file,std::vector<std::string> dependencies);

class Slicer {
    llvm::Module *M{};

    dg::llvmdg::LLVMDependenceGraphBuilder _builder;
    std::unique_ptr<dg::LLVMDependenceGraph> _dg{};

    dg::llvmdg::LLVMSlicer slicer;
    uint32_t slice_id = 0;
    bool _computed_deps{false};

    llvm::StringRef entryFunction;
public:
    Slicer(llvm::Module *mod,llvm::StringRef entryFunction)
    : M(mod),entryFunction(entryFunction),
      _builder(mod) { assert(mod && "Need module"); }

    const dg::LLVMDependenceGraph& getDG() const { return *_dg.get(); }
    dg::LLVMDependenceGraph& getDG() { return *_dg.get(); }
    llvm::Module* getModule(){return M;}

    std::set<dg::LLVMNode *> getCriteria(std::set<unsigned> slicingCriteria);

    // Mirror LLVM to nodes of dependence graph,
    // No dependence edges are added here unless the
    // 'compute_deps' parameter is set to true.
    // Otherwise, dependencies must be computed later
    // using computeDependencies().
    bool buildDG();

    // Explicitely compute dependencies after building the graph.
    // This method can be used to compute dependencies without
    // calling mark() afterwards (mark() calls this function).
    // It must not be called before calling mark() in the future.
    void computeDependencies();

    // Mark the nodes from the slice.
    // This method calls computeDependencies(),
    // but buildDG() must be called before.
    bool mark(std::set<dg::LLVMNode *>& criteria_nodes);

    bool slice();

    ///
    // Create new empty main in the module. If 'call_entry' is set to true,
    // then call the entry function from the new main (if entry is not main),
    // otherwise the main is going to be empty
    bool createEmptyMain(bool call_entry = false);
};

} // fin namespace clang