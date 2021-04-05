#ifndef DG_LLVM_DOD_H_
#define DG_LLVM_DOD_H_

#include <llvm/IR/Module.h>

#include "dg/llvm/ControlDependence/ControlDependence.h"
#include "GraphBuilder.h"
#include "IGraphBuilder.h"

#include "ControlDependence/DOD.h"
#include "ControlDependence/DODNTSCD.h"

#include <set>
#include <map>
#include <unordered_map>


namespace llvm {
class Function;
}

namespace dg {
namespace llvmdg {

class DOD : public LLVMControlDependenceAnalysisImpl {
    CDGraphBuilder graphBuilder{};

    // although DOD is a ternary relation, we treat it as binary relation
    // by forgetting the connection between dependant nodes. That is,
    // for each p -> {a, b}, we have (p, a) and (p, b).
    // This has no effect on slicing. If we will need that in the future,
    // we can change this.
    using CDResultT =  std::map<CDNode *, std::set<CDNode *>>;

    struct Info {
        CDGraph graph;

        // forward edges (from branchings to dependent blocks)
        CDResultT controlDependence{};
        // reverse edges (from dependent blocks to branchings)
        CDResultT revControlDependence{};

        Info(CDGraph&& graph) : graph(std::move(graph)) {}
    };

    std::unordered_map<const llvm::Function *, Info> _graphs;

public:
    using ValVec = LLVMControlDependenceAnalysis::ValVec;

    DOD(const llvm::Module *module,
        const LLVMControlDependenceAnalysisOptions& opts = {})
        : LLVMControlDependenceAnalysisImpl(module, opts) {
        _graphs.reserve(module->size());
    }

    /// Getters of dependencies for a value
    ValVec getDependencies(const llvm::Instruction *I) override {
        if (!getOptions().nodePerInstruction()) {
            return {};
        }

        // XXX: this could be computed on-demand per one node (block)
        // (in contrary to getDependent())
        auto *f = I->getParent()->getParent();
        if (_getGraph(f) == nullptr) {
            /// FIXME: get rid of the const cast
            computeOnDemand(const_cast<llvm::Function*>(f));
        }

        assert(_getGraph(f) != nullptr);

        auto *node = graphBuilder.getNode(I);
        if (!node) {
            return {};
        }
        auto *info = _getFunInfo(f);
        assert(info && "Did not compute CD");

        auto dit = info->controlDependence.find(node);
        if (dit == info->controlDependence.end())
            return {};

        std::set<llvm::Value *> ret;
        for (auto *dep : dit->second) {
            auto *val = graphBuilder.getValue(dep);
            assert(val && "Invalid value");
            ret.insert(const_cast<llvm::Value*>(val));
        }

        return ValVec{ret.begin(), ret.end()};
    }

    ValVec getDependent(const llvm::Instruction *) override { return {}; }

    /// Getters of dependencies for a basic block
    ValVec getDependencies(const llvm::BasicBlock *b) override {
        if (getOptions().nodePerInstruction()) {
            return {};
        }

        if (_getGraph(b->getParent()) == nullptr) {
            /// FIXME: get rid of the const cast
            computeOnDemand(const_cast<llvm::Function*>(b->getParent()));
        }
        assert(_getGraph(b->getParent()) != nullptr);

        auto *block = graphBuilder.getNode(b);
        if (!block) {
            return {};
        }
        auto *info = _getFunInfo(b->getParent());
        assert(info && "Did not compute CD");

        auto dit = info->controlDependence.find(block);
        if (dit == info->controlDependence.end())
            return {};

        std::set<llvm::Value *> ret;
        for (auto *dep : dit->second) {
            auto *val = graphBuilder.getValue(dep);
            assert(val && "Invalid value");
            ret.insert(const_cast<llvm::Value*>(val));
        }

        return ValVec{ret.begin(), ret.end()};
    }

    ValVec getDependent(const llvm::BasicBlock *) override {
        assert(false && "Not supported");
        abort();
    }

    // We run on demand but this method can trigger the computation
    void compute(const llvm::Function *F = nullptr) override {
        DBG(cda, "Triggering computation of all dependencies");
        if (F && !F->isDeclaration() && (_getGraph(F) == nullptr)) {
            computeOnDemand(const_cast<llvm::Function*>(F));
        } else {
            for (auto& f : *getModule()) {
                if (!f.isDeclaration() && (_getGraph(&f) == nullptr)) {
                    computeOnDemand(const_cast<llvm::Function*>(&f));
                }
            }
        }
    }

    CDGraph *getGraph(const llvm::Function *f) override { return _getGraph(f); }
    const CDGraph *getGraph(const llvm::Function *f) const override { return _getGraph(f); }

    // make this one public so that we can dump it in llvm-cda-dump
    // (keep the _ prefix so that we can see that it should not be normally used...)
    const Info *_getFunInfo(const llvm::Function *f) const {
        auto it = _graphs.find(f);
        return it == _graphs.end() ? nullptr : &it->second;
    }

    Info *_getFunInfo(const llvm::Function *f) {
        auto it = _graphs.find(f);
        return it == _graphs.end() ? nullptr : &it->second;
    }

private:
    const CDGraph *_getGraph(const llvm::Function *f) const {
        auto it = _graphs.find(f);
        return it == _graphs.end() ? nullptr : &it->second.graph;
    }

    CDGraph *_getGraph(const llvm::Function *f) {
        auto it = _graphs.find(f);
        return it == _graphs.end() ? nullptr : &it->second.graph;
    }

   void computeOnDemand(llvm::Function *F) {
        DBG(cda, "Triggering on-demand computation for " << F->getName().str());
        assert(_getGraph(F) == nullptr
               && "Already have the graph");

        auto tmpgraph = graphBuilder.build(F, getOptions().nodePerInstruction());
        // FIXME: we can actually just forget the graph if we do not want to dump
        // it to the user
        auto it = _graphs.emplace(F, std::move(tmpgraph));

        auto& info = it.first->second;

        if (getOptions().dodRanganathCD()) {
            dg::DODRanganath dod;
            auto result = dod.compute(info.graph);
            info.controlDependence = std::move(result.first);
            info.revControlDependence = std::move(result.second);
        } else if (getOptions().dodCD()) {
            dg::DOD dod;
            auto result = dod.compute(info.graph);
            info.controlDependence = std::move(result.first);
            info.revControlDependence = std::move(result.second);
        } else if (getOptions().dodntscdCD()) {
            dg::DODNTSCD dodntscd;
            auto result = dodntscd.compute(info.graph);
            info.controlDependence = std::move(result.first);
            info.revControlDependence = std::move(result.second);
        } else {
            assert(false && "Wrong analysis type");
            abort();
        }
   }
};


// FIXME: this basically copies InterproceduralNTSCD, create
// a shared superclass...
class InterproceduralDOD : public LLVMControlDependenceAnalysisImpl {
    ICDGraphBuilder igraphBuilder{};
    CDGraph graph;

    using CDResultT =  std::map<CDNode *, std::set<CDNode *>>;
    // forward edges (from branchings to dependent blocks)
    CDResultT controlDependence{};
    // reverse edges (from dependent blocks to branchings)
    CDResultT revControlDependence{};
    bool _computed{false};

public:
    using ValVec = LLVMControlDependenceAnalysis::ValVec;

    ///
    // Note: use this only when you know what you want.
    // Computing intraprocedural CD + interprocedural CD
    // separately is more efficient.
    InterproceduralDOD(const llvm::Module *module,
                       const LLVMControlDependenceAnalysisOptions& opts = {},
                       LLVMPointerAnalysis *pta = nullptr,
                       CallGraph *cg = nullptr)
        : LLVMControlDependenceAnalysisImpl(module, opts), igraphBuilder(pta, cg) {}

    /// Getters of dependencies for a value
    ValVec getDependencies(const llvm::Instruction *I) override {
        if (!getOptions().nodePerInstruction()) {
            return {};
        }

        _compute();

        auto *node = igraphBuilder.getNode(I);
        if (!node) {
            return {};
        }

        assert(_computed && "CD is not computed");
        auto dit = controlDependence.find(node);
        if (dit == controlDependence.end())
            return {};

        std::set<llvm::Value *> ret;
        for (auto *dep : dit->second) {
            auto *val = igraphBuilder.getValue(dep);
            assert(val && "Invalid value");
            ret.insert(const_cast<llvm::Value*>(val));
        }

        return ValVec{ret.begin(), ret.end()};
    }

    ValVec getDependent(const llvm::Instruction *) override { return {}; }

    /// Getters of dependencies for a basic block
    ValVec getDependencies(const llvm::BasicBlock *b) override {
        if (getOptions().nodePerInstruction()) {
            return {};
        }

        _compute();

        auto *block = igraphBuilder.getNode(b);
        if (!block) {
            return {};
        }

        assert(_computed && "Did not compute CD");
        auto dit = controlDependence.find(block);
        if (dit == controlDependence.end())
            return {};

        std::set<llvm::Value *> ret;
        for (auto *dep : dit->second) {
            auto *val = igraphBuilder.getValue(dep);
            assert(val && "Invalid value");
            ret.insert(const_cast<llvm::Value*>(val));
        }

        return ValVec{ret.begin(), ret.end()};
    }

    ValVec getDependent(const llvm::BasicBlock *) override {
        assert(false && "Not supported");
        abort();
    }

    // We run on demand but this method can trigger the computation
    void compute(const llvm::Function *) override {
        _compute();
    }

    /// Getter for noreturn nodes in function (for interprocedural analysis)
    ValVec getNoReturns(const llvm::Function *) override {
        assert(false && "Unsupported"); abort();
    }

    CDGraph *getGraph(const llvm::Function *) override { return &graph; }
    const CDGraph *getGraph(const llvm::Function *) const override { return &graph; }

private:

   void _compute() {
       if (_computed)
           return;

        DBG(cda, "Triggering computation of interprocedural NTSCD");

        graph = igraphBuilder.build(getModule(),
                                    getOptions().nodePerInstruction());

        if (getOptions().dodRanganathCD()) {
            dg::DODRanganath dod;
            auto result = dod.compute(graph);
            controlDependence = std::move(result.first);
            revControlDependence = std::move(result.second);
        } else if (getOptions().dodCD()) {
            dg::DOD dod;
            auto result = dod.compute(graph);
            controlDependence = std::move(result.first);
            revControlDependence = std::move(result.second);
        } else if (getOptions().dodntscdCD()) {
            dg::DODNTSCD dodntscd;
            auto result = dodntscd.compute(graph);
            controlDependence = std::move(result.first);
            revControlDependence = std::move(result.second);
        } else {
            assert(false && "Wrong analysis type");
            abort();
        }

        _computed = true;
   }
};


} // namespace llvmdg
} // namespace dg


#endif
