AM_CXXFLAGS = -fopenmp `llvm-config --cxxflags` -Wall -Wno-unknown-pragmas -fno-strict-aliasing

bin_PROGRAMS = pdiffer prophet learner playground

lib_LTLIBRARIES = libprofiler.la libprofile_runtime.la libtest_runtime.la

AM_CPPFLAGS = -I../include

ClangLDADD = -lclangTooling -lclangFrontendTool -lclangFrontend -lclangDriver -lclangSerialization -lclangCodeGen \
			 -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers \
			 -lclangStaticAnalyzerCore -lclangAnalysis -lclangIndex -lclangARCMigrate -lclangEdit \
			 -lclangAST -lclangLex -lclangBasic
ClangLDADD += -lLLVMXRay -lLLVMWindowsManifest -lLLVMTableGen -lLLVMSymbolize -lLLVMDebugInfoPDB \
-lLLVMOrcJIT -lLLVMOrcError -lLLVMJITLink -lLLVMObjectYAML -lLLVMMCA -lLLVMLTO -lLLVMPasses \
-lLLVMCoroutines -lLLVMObjCARCOpts -lLLVMExtensions -lLLVMLineEditor -lLLVMLibDriver \
-lLLVMInterpreter -lLLVMFuzzMutate -lLLVMMCJIT -lLLVMExecutionEngine -lLLVMRuntimeDyld \
-lLLVMDWARFLinker -lLLVMDlltoolDriver -lLLVMOption -lLLVMDebugInfoGSYM -lLLVMCoverage \
-lLLVMXCoreDisassembler -lLLVMXCoreCodeGen -lLLVMXCoreDesc -lLLVMXCoreInfo \
-lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMX86Desc -lLLVMX86Info \
-lLLVMWebAssemblyDisassembler -lLLVMWebAssemblyCodeGen -lLLVMWebAssemblyDesc \
-lLLVMWebAssemblyAsmParser -lLLVMWebAssemblyInfo -lLLVMSystemZDisassembler \
-lLLVMSystemZCodeGen -lLLVMSystemZAsmParser -lLLVMSystemZDesc -lLLVMSystemZInfo \
-lLLVMSparcDisassembler -lLLVMSparcCodeGen -lLLVMSparcAsmParser -lLLVMSparcDesc \
-lLLVMSparcInfo -lLLVMRISCVDisassembler -lLLVMRISCVCodeGen -lLLVMRISCVAsmParser \
-lLLVMRISCVDesc -lLLVMRISCVUtils -lLLVMRISCVInfo -lLLVMPowerPCDisassembler \
-lLLVMPowerPCCodeGen -lLLVMPowerPCAsmParser -lLLVMPowerPCDesc -lLLVMPowerPCInfo \
-lLLVMNVPTXCodeGen -lLLVMNVPTXDesc -lLLVMNVPTXInfo -lLLVMMSP430Disassembler \
-lLLVMMSP430CodeGen -lLLVMMSP430AsmParser -lLLVMMSP430Desc -lLLVMMSP430Info \
-lLLVMMipsDisassembler -lLLVMMipsCodeGen -lLLVMMipsAsmParser -lLLVMMipsDesc -lLLVMMipsInfo \
-lLLVMLanaiDisassembler -lLLVMLanaiCodeGen -lLLVMLanaiAsmParser -lLLVMLanaiDesc \
-lLLVMLanaiInfo -lLLVMHexagonDisassembler -lLLVMHexagonCodeGen -lLLVMHexagonAsmParser \
-lLLVMHexagonDesc -lLLVMHexagonInfo -lLLVMBPFDisassembler -lLLVMBPFCodeGen -lLLVMBPFAsmParser \
-lLLVMBPFDesc -lLLVMBPFInfo -lLLVMAVRDisassembler -lLLVMAVRCodeGen -lLLVMAVRAsmParser \
-lLLVMAVRDesc -lLLVMAVRInfo -lLLVMARMDisassembler -lLLVMARMCodeGen -lLLVMARMAsmParser \
-lLLVMARMDesc -lLLVMARMUtils -lLLVMARMInfo -lLLVMAMDGPUDisassembler -lLLVMAMDGPUCodeGen \
-lLLVMMIRParser -lLLVMipo -lLLVMInstrumentation -lLLVMVectorize -lLLVMLinker -lLLVMIRReader \
-lLLVMAsmParser -lLLVMFrontendOpenMP -lLLVMAMDGPUAsmParser -lLLVMAMDGPUDesc -lLLVMAMDGPUUtils \
-lLLVMAMDGPUInfo -lLLVMAArch64Disassembler -lLLVMMCDisassembler -lLLVMAArch64CodeGen \
-lLLVMCFGuard -lLLVMGlobalISel -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMDebugInfoDWARF \
-lLLVMCodeGen -lLLVMTarget -lLLVMScalarOpts -lLLVMInstCombine -lLLVMAggressiveInstCombine \
-lLLVMTransformUtils -lLLVMBitWriter -lLLVMAnalysis -lLLVMProfileData -lLLVMObject \
-lLLVMTextAPI -lLLVMBitReader -lLLVMCore -lLLVMRemarks -lLLVMBitstreamReader \
-lLLVMAArch64AsmParser -lLLVMMCParser -lLLVMAArch64Desc -lLLVMMC -lLLVMDebugInfoCodeView \
-lLLVMDebugInfoMSF -lLLVMBinaryFormat -lLLVMAArch64Utils -lLLVMAArch64Info -lLLVMSupport \
-lLLVMDemangle -lz -lcurses

DG_SRC = dg/BBlockBase.cpp dg/Debug.cpp dg/Offset.cpp 
DG_SRC += dg/ControlDependence/NTSCD.cpp 
DG_SRC += dg/MemorySSA/Definitions.cpp dg/MemorySSA/MemorySSA.cpp dg/MemorySSA/ModRef.cpp 
DG_SRC += dg/PointerAnalysis/Pointer.cpp dg/PointerAnalysis/PointerAnalysis.cpp dg/PointerAnalysis/PointerGraph.cpp \
dg/PointerAnalysis/PointerGraphOptimizations.cpp dg/PointerAnalysis/PointerGraphValidator.cpp dg/PointerAnalysis/PointsToSet.cpp 
DG_SRC += dg/ReadWriteGraph/ReadWriteGraph.cpp 
DG_SRC += dg/SystemDependenceGraph/DependenceGraph.cpp 
DG_SRC += dg/llvm/LLVMDependenceGraph.cpp dg/llvm/LLVMDGVerifier.cpp dg/llvm/LLVMNode.cpp dg/llvm/LLVMSlicerCriteria.cpp
DG_SRC += dg/llvm/ControlDependence/ControlDependence.cpp dg/llvm/ControlDependence/InterproceduralCD.cpp \
dg/llvm/ControlDependence/SCD.cpp dg/llvm/ControlDependence/legacy/Block.cpp dg/llvm/ControlDependence/legacy/Function.cpp \
dg/llvm/ControlDependence/legacy/GraphBuilder.cpp dg/llvm/ControlDependence/legacy/NTSCD.cpp 
DG_SRC += dg/llvm/DataDependenceAnalysis/LLVMDataDependenceAnalysis.cpp dg/llvm/DefUse/DefUse.cpp dg/llvm/Dominators/PostDominators.cpp \
dg/llvm/ForkJoin/ForkJoin.cpp 
DG_SRC += dg/llvm/PointerAnalysis/Block.cpp dg/llvm/PointerAnalysis/Calls.cpp dg/llvm/PointerAnalysis/Constants.cpp dg/llvm/PointerAnalysis/Globals.cpp \
dg/llvm/PointerAnalysis/Instructions.cpp dg/llvm/PointerAnalysis/Interprocedural.cpp dg/llvm/PointerAnalysis/PointerAnalysis.cpp dg/llvm/PointerAnalysis/PointerGraph.cpp \
dg/llvm/PointerAnalysis/PointerGraphValidator.cpp dg/llvm/PointerAnalysis/Structure.cpp dg/llvm/PointerAnalysis/Threads.cpp 
DG_SRC += dg/llvm/ReadWriteGraph/Calls.cpp dg/llvm/ReadWriteGraph/Instructions.cpp dg/llvm/ReadWriteGraph/LLVMReadWriteGraphBuilder.cpp 
DG_SRC += dg/llvm/SystemDependenceGraph/Dependencies.cpp dg/llvm/SystemDependenceGraph/SDG2Dot.cpp  dg/llvm/SystemDependenceGraph/SystemDependenceGraph.cpp 
DG_SRC += dg/llvm/ThreadRegions/MayHappenInParallel.cpp dg/llvm/ThreadRegions/ThreadRegion.cpp dg/llvm/ThreadRegions/Graphs/BlockGraph.cpp \
dg/llvm/ThreadRegions/Graphs/ControlFlowGraph.cpp dg/llvm/ThreadRegions/Graphs/CriticalSectionsBuilder.cpp dg/llvm/ThreadRegions/Graphs/FunctionGraph.cpp \
dg/llvm/ThreadRegions/Graphs/GraphBuilder.cpp dg/llvm/ThreadRegions/Graphs/ThreadRegionsBuilder.cpp dg/llvm/ThreadRegions/Nodes/CallFuncPtrNode.cpp \
dg/llvm/ThreadRegions/Nodes/CallNode.cpp dg/llvm/ThreadRegions/Nodes/CallReturnNode.cpp dg/llvm/ThreadRegions/Nodes/EntryNode.cpp \
dg/llvm/ThreadRegions/Nodes/ExitNode.cpp dg/llvm/ThreadRegions/Nodes/ForkNode.cpp dg/llvm/ThreadRegions/Nodes/GeneralNode.cpp \
dg/llvm/ThreadRegions/Nodes/JoinNode.cpp dg/llvm/ThreadRegions/Nodes/LockNode.cpp dg/llvm/ThreadRegions/Nodes/Node.cpp dg/llvm/ThreadRegions/Nodes/NodeIterator.cpp \
dg/llvm/ThreadRegions/Nodes/ReturnNode.cpp dg/llvm/ThreadRegions/Nodes/UnlockNode.cpp

pdiffer_SOURCES = pdiffer.cpp Utils.cpp ASTDiffer.cpp SourceContextManager.cpp LocalAnalyzer.cpp \
				  GlobalAnalyzer.cpp ASTUtils.cpp BenchProgram.cpp RepairCandidateGenerator.cpp \
				  CodeRewrite.cpp FeatureExtract.cpp DGController.cpp cJSON/cJSON_Utils.c CollectCondition.cpp cJSON/cJSON.c $(DG_SRC)
pdiffer_LDFLAGS = `llvm-config --ldflags`
pdiffer_LDADD = $(ClangLDADD) -lexplain

learner_SOURCES = learner.cpp FeatureExtract.cpp SourceContextManager.cpp LocalAnalyzer.cpp \
				  GlobalAnalyzer.cpp ASTUtils.cpp BenchProgram.cpp RepairCandidateGenerator.cpp \
				  Utils.cpp CodeRewrite.cpp DGController.cpp CollectCondition.cpp cJSON/cJSON_Utils.c cJSON/cJSON.c $(DG_SRC)
learner_LDFLAGS = `llvm-config --ldflags`
learner_LDADD = $(ClangLDADD) -lexplain

prophet_SOURCES = Main.cpp Utils.cpp RepairSearchEngine.cpp BenchProgram.cpp ASTUtils.cpp \
				  ErrorLocalizer.cpp LocalAnalyzer.cpp ProfileErrorLocalizer.cpp SourceContextManager.cpp \
				  GlobalAnalyzer.cpp DuplicateDetector.cpp ExprSynthesizer.cpp RepairCandidateGenerator.cpp \
				  CodeRewrite.cpp FeatureExtract.cpp DGController.cpp CollectCondition.cpp cJSON/cJSON_Utils.c cJSON/cJSON.c $(DG_SRC)
prophet_LDFLAGS = `llvm-config --ldflags`
prophet_LDADD = $(ClangLDADD) -lexplain

playground_SOURCES = PlayGround.cpp Utils.cpp BenchProgram.cpp DGController.cpp CollectCondition.cpp cJSON/cJSON_Utils.c cJSON/cJSON.c $(DG_SRC)
playground_LDFLAGS = `llvm-config --ldflags`
playground_LDADD = $(ClangLDADD) -lexplain

libprofiler_la_SOURCES = ProfilerAction.cpp
libprofiler_la_LDFLAGS = `llvm-config --ldflags`
libprofiler_la_LIBADD = -lLLVMSupport

libprofile_runtime_la_SOURCES = _prophet_profile.cpp
libprofile_runtime_la_LIBADD = -lpthread

libtest_runtime_la_SOURCES = _test_runtime.cpp
