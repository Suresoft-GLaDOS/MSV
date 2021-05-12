#include "DGController.h"

namespace clang{

llvm::Module *createModule(std::string file,std::vector<std::string> dependencies){
    // Create LLVM bitcode
    std::string bitcodeFile=replace_ext(file,".bc");
    std::string cmd="clang -emit-llvm -S ";
    for (std::string dependency: dependencies)
        cmd+="-I"+dependency+" ";
    cmd+=file+" -o "+bitcodeFile;
    bool result=system(cmd.c_str());
    if (result) return nullptr;

    // Get Module from generated bitcode file
    llvm::LLVMContext context;
    llvm::SMDiagnostic SMD;

    std::unique_ptr<llvm::Module> M = llvm::parseIRFile(bitcodeFile, SMD, context);
    llvm::Module *mod=M.get();
    return mod;
}
std::set<dg::LLVMNode *> Slicer::getCriteria(std::set<unsigned> slicingCriteria){
    std::string criteria="";
    for (unsigned lineNum:slicingCriteria){
        criteria+=std::to_string(lineNum)+";";
    }
    criteria.pop_back();
    outlog_printf(2,"criteria: %s\n",criteria.c_str());

    std::set<dg::LLVMNode *> result;
    getSlicingCriteriaNodes(*_dg.get(),criteria,result,false);
    return result;
}


bool Slicer::buildDG() {
    _dg = std::move(_builder.constructCFGOnly());

    if (!_dg) {
        llvm::errs() << "Building the dependence graph failed!\n";
        return false;
    }

    // if (compute_deps)
    //     computeDependencies();

    return true;
}

void Slicer::computeDependencies() {
    assert(!_computed_deps && "Already called computeDependencies()");
    // must call buildDG() before this function
    assert(_dg && "Must build dg before computing dependencies");

    _dg = _builder.computeDependencies(std::move(_dg));
    _computed_deps = true;

    const auto& stats = _builder.getStatistics();
    llvm::errs() << "[llvm-slicer] CPU time of pointer analysis: " << double(stats.ptaTime) / CLOCKS_PER_SEC << " s\n";
    llvm::errs() << "[llvm-slicer] CPU time of data dependence analysis: " << double(stats.rdaTime) / CLOCKS_PER_SEC << " s\n";
    llvm::errs() << "[llvm-slicer] CPU time of control dependence analysis: " << double(stats.cdaTime) / CLOCKS_PER_SEC << " s\n";
}

bool Slicer::mark(std::set<dg::LLVMNode *>& criteria_nodes)
{
    assert(_dg && "mark() called without the dependence graph built");
    assert(!criteria_nodes.empty() && "Do not have slicing criteria");

    dg::debug::TimeMeasure tm;

    // compute dependece edges
    computeDependencies();

    // unmark this set of nodes after marking the relevant ones.
    // Used to mimic the Weissers algorithm
    // std::set<dg::LLVMNode *> unmark;

    // if (_options.removeSlicingCriteria)
    //     unmark = criteria_nodes;

    // _dg->getCallSites(_options.additionalSlicingCriteria, &criteria_nodes);

    // for (auto& funcName : _options.preservedFunctions)
    //     slicer.keepFunctionUntouched(funcName.c_str());

    slice_id = 0xdead;

    tm.start();
    for (dg::LLVMNode *start : criteria_nodes)
        // slice_id = slicer.mark(start, slice_id, _options.forwardSlicing);
        slice_id = slicer.mark(start, slice_id, false);

    assert(slice_id != 0 && "Somethig went wrong when marking nodes");

    // if we have some nodes in the unmark set, unmark them
    // for (dg::LLVMNode *nd : unmark)
    //     nd->setSlice(0);

    tm.stop();
    tm.report("[llvm-slicer] Finding dependent nodes took");

    return true;
}

bool Slicer::slice()
{
    assert(_dg && "Must run buildDG() and computeDependencies()");
    assert(slice_id != 0 && "Must run mark() method before slice()");

    dg::debug::TimeMeasure tm;

    tm.start();
    slicer.slice(_dg.get(), nullptr, slice_id);

    tm.stop();
    tm.report("[llvm-slicer] Slicing dependence graph took");

    dg::SlicerStatistics& st = slicer.getStatistics();
    llvm::errs() << "[llvm-slicer] Sliced away " << st.nodesRemoved
                    << " from " << st.nodesTotal << " nodes in DG\n";

    return true;
}

bool Slicer::createEmptyMain(bool call_entry)
{
    llvm::LLVMContext& ctx = M->getContext();
    llvm::Function *main_func = M->getFunction("main");
    if (!main_func) {
        auto C = M->getOrInsertFunction("main",
                                        llvm::Type::getInt32Ty(ctx)
#if LLVM_VERSION_MAJOR < 5
                                        , nullptr
#endif // LLVM < 5
                                        );
#if LLVM_VERSION_MAJOR < 9
        if (!C) {
            llvm::errs() << "Could not create new main function\n";
            return false;
        }

        main_func = llvm::cast<llvm::Function>(C);
#else
        main_func = llvm::cast<llvm::Function>(C.getCallee());
#endif
    } else {
        // delete old function body
        main_func->deleteBody();
    }

    assert(main_func && "Do not have the main func");
    assert(main_func->size() == 0 && "The main func is not empty");

    // create new function body
    llvm::BasicBlock* blk = llvm::BasicBlock::Create(ctx, "entry", main_func);

    if (call_entry && entryFunction != "main") {
        llvm::Function *entry = M->getFunction(entryFunction);
        assert(entry && "The entry function is not present in the module");

        // TODO: we should set the arguments to undef
        llvm::CallInst::Create(entry, "entry", blk);
    }

    llvm::Type *Ty = main_func->getReturnType();
    llvm::Value *retval = nullptr;
    if (Ty->isIntegerTy())
        retval = llvm::ConstantInt::get(Ty, 0);
    llvm::ReturnInst::Create(ctx, retval, blk);

    return true;
}


}; // fin namespace clang