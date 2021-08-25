#pragma once
#include <cstdio>
#include "BenchProgram.h"

class SourceContextManager;

bool addProfileWriter(BenchProgram &P,std::map<std::string, std::string> &fileCode,std::vector<long long> &macros,
        std::string output_file="");
