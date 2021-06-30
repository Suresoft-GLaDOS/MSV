#pragma once
#include <clang/AST/ASTContext.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include "BenchProgram.h"

namespace clang{
    std::vector<Information> rewriteCondition(std::map<std::pair<size_t,size_t>,std::vector<std::vector<Expr *>>> newExprs,std::string workDir,std::string toolsDir,ASTContext *ctxt);
}