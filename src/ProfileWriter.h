#pragma once
#include <cstdio>
#include "BenchProgram.h"

class SourceContextManager;

std::map<std::string,std::vector<long long>> addProfileWriter(BenchProgram &P,std::map<std::string, std::string> &fileCode,std::vector<long long> &macros,std::map<long long,std::string> &macroCode,
        std::string output_file="");
