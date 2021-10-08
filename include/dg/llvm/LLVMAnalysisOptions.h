#ifndef DG_LLVM_ANALYSIS_OPTIONS_H_
#define DG_LLVM_ANALYSIS_OPTIONS_H_

#include <string>
#include "dg/AnalysisOptions.h"

namespace dg {

struct LLVMAnalysisOptions {
    // Number of bytes in objects to track precisely
    std::string entryFunction{"main"};

    LLVMAnalysisOptions& setEntryFunction(const std::string& e) {
        entryFunction = e; return *this;
    }
};

} // namespace dg

#endif
