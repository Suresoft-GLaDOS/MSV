#ifndef CONTROLFLOWGRAPH_H
#define CONTROLFLOWGRAPH_H

#include <memory>
#include <set>

#include <dg/util/SilenceLLVMWarnings.h>
SILENCE_LLVM_WARNINGS_PUSH
#include <llvm/IR/Instructions.h>
SILENCE_LLVM_WARNINGS_POP

namespace dg {
    class DGLLVMPointerAnalysis;
}

namespace llvm {
    class Function;
}

class ThreadRegion;
class GraphBuilder;
class ThreadRegionsBuilder;
class CriticalSectionsBuilder;

class ControlFlowGraph
{
private:
    std::unique_ptr<GraphBuilder>               graphBuilder;
    std::unique_ptr<ThreadRegionsBuilder>       threadRegionsBuilder;
    std::unique_ptr<CriticalSectionsBuilder>    criticalSectionsBuilder;

public:
    ControlFlowGraph(dg::DGLLVMPointerAnalysis * pointsToAnalysis);

    ~ControlFlowGraph();

    std::set<const llvm::CallInst *> getJoins() const;

    std::set<const llvm::CallInst *> getCorrespondingForks(const llvm::CallInst * callInst) const;

    std::set<const llvm::CallInst *> getLocks() const;

    std::set<const llvm::CallInst *> getCorrespongingUnlocks(const llvm::CallInst * callInst) const;

    std::set<const llvm::Instruction *> getCorrespondingCriticalSection(const llvm::CallInst * callInst) const;

    void buildFunction(const llvm::Function *function);

    void printWithRegions(std::ostream & ostream) const;

    void printWithoutRegions(std::ostream & ostream) const;

    std::set<ThreadRegion *> threadRegions();
};

#endif // CONTROLFLOWGRAPH_H
