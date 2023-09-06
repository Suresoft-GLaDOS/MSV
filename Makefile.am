AM_CPPFLAGS = -fopenmp `llvm-config --cxxflags`\
	-Wall -Wno-unknown-pragmas -fno-strict-aliasing

bin_PROGRAMS = msv msv-diff msv-learn msv-playground
dist_bin_SCRIPTS = tools/msv-afl tools/msv-dd\
	tools/msv-profilecc tools/msv-testcc

msv_SOURCES = src/Main.cpp src/ASTUtils.cpp src/BenchProgram.cpp\
	src/CodeRewrite.cpp src/DuplicateDetector.cpp src/ErrorLocalizer.cpp\
	src/ExprSynthesizer.cpp src/FeatureExtract.cpp src/GlobalAnalyzer.cpp\
	src/LocalAnalyzer.cpp src/ProfileErrorLocalizer.cpp\
	src/ProfileWriter.cpp src/RepairCandidateGenerator.cpp\
	src/RepairSearchEngine.cpp src/SBFLLocalizer.cpp\
	src/SourceContextManager.cpp src/Utils.cpp
msv_LDFLAGS = `llvm-config --ldflags`
msv_LDADD = -lLLVM -lcjson -lclang-cpp -lexplain

msv_diff_SOURCES = src/pdiffer.cpp src/ASTDiffer.cpp src/ASTUtils.cpp\
	src/BenchProgram.cpp src/CodeRewrite.cpp src/FeatureExtract.cpp\
	src/GlobalAnalyzer.cpp src/LocalAnalyzer.cpp src/ProfileWriter.cpp\
	src/RepairCandidateGenerator.cpp src/SourceContextManager.cpp\
	src/Utils.cpp
msv_diff_LDFLAGS = `llvm-config --ldflags`
msv_diff_LDADD = -lLLVM -lcjson -lclang-cpp -lexplain

msv_learn_SOURCES = src/learner.cpp src/ASTUtils.cpp\
	src/BenchProgram.cpp src/CodeRewrite.cpp src/FeatureExtract.cpp\
	src/GlobalAnalyzer.cpp src/LocalAnalyzer.cpp src/ProfileWriter.cpp\
	src/RepairCandidateGenerator.cpp src/SourceContextManager.cpp\
	src/Utils.cpp
msv_learn_LDFLAGS = `llvm-config --ldflags`
msv_learn_LDADD = -lLLVM -lcjson -lclang-cpp -lexplain

msv_playground_SOURCES = src/PlayGround.cpp src/BenchProgram.cpp src/Utils.cpp
msv_playground_LDFLAGS = `llvm-config --ldflags`
msv_playground_LDADD = -lLLVM -lcjson -lclang-cpp -lexplain

# FIXME: rename to be less generic
lib_LTLIBRARIES = libprofiler.la libprofile_runtime.la libtest_runtime.la

libprofiler_la_SOURCES = src/ProfilerAction.cpp
libprofiler_la_LDFLAGS = `llvm-config --ldflags`
libprofiler_la_LIBADD = -lLLVM -lclang-cpp -lexplain

libprofile_runtime_la_SOURCES = src/_prophet_profile.cpp
libprofile_runtime_la_CPPFLAGS = -Iinclude
libprofile_runtime_la_LIBADD = -lpthread
include_HEADERS = include/_prophet_profile.h

libtest_runtime_la_SOURCES = src/_test_runtime.cpp
