AM_CXXFLAGS = -fopenmp `llvm-config --cxxflags` -Wall -Wno-unknown-pragmas -fno-strict-aliasing

bin_PROGRAMS = pdiffer prophet learner playground

lib_LTLIBRARIES = libprofiler.la libprofile_runtime.la libtest_runtime.la

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


pdiffer_SOURCES = pdiffer.cpp Utils.cpp ASTDiffer.cpp SourceContextManager.cpp LocalAnalyzer.cpp \
				  GlobalAnalyzer.cpp ASTUtils.cpp BenchProgram.cpp RepairCandidateGenerator.cpp \
				  CodeRewrite.cpp FeatureExtract.cpp DeltaDebuggingTest.cpp
pdiffer_LDFLAGS = `llvm-config --ldflags`
pdiffer_LDADD = $(ClangLDADD) -lexplain

learner_SOURCES = learner.cpp FeatureExtract.cpp SourceContextManager.cpp LocalAnalyzer.cpp \
				  GlobalAnalyzer.cpp ASTUtils.cpp BenchProgram.cpp RepairCandidateGenerator.cpp \
				  Utils.cpp CodeRewrite.cpp DeltaDebuggingTest.cpp
learner_LDFLAGS = `llvm-config --ldflags`
learner_LDADD = $(ClangLDADD) -lexplain

prophet_SOURCES = Main.cpp Utils.cpp RepairSearchEngine.cpp BenchProgram.cpp ASTUtils.cpp \
				  ErrorLocalizer.cpp LocalAnalyzer.cpp ProfileErrorLocalizer.cpp SourceContextManager.cpp \
				  GlobalAnalyzer.cpp DuplicateDetector.cpp ExprSynthesizer.cpp RepairCandidateGenerator.cpp \
				  CodeRewrite.cpp FeatureExtract.cpp DeltaDebuggingTest.cpp
prophet_LDFLAGS = `llvm-config --ldflags`
prophet_LDADD = $(ClangLDADD) -lexplain

playground_SOURCES = PlayGround.cpp Utils.cpp BenchProgram.cpp DeltaDebuggingTest.cpp
playground_LDFLAGS = `llvm-config --ldflags`
playground_LDADD = $(ClangLDADD) -lexplain

libprofiler_la_SOURCES = ProfilerAction.cpp
libprofiler_la_LDFLAGS = `llvm-config --ldflags`
libprofiler_la_LIBADD = -lLLVMSupport

libprofile_runtime_la_SOURCES = _prophet_profile.cpp
libprofile_runtime_la_LIBADD = -lpthread

libtest_runtime_la_SOURCES = _test_runtime.cpp
