#include <clang/AST/AST.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include "SBFLLocalizer.h"

using namespace clang;
// TODO: Create AST visitor for parse SBFL to Prophet FL
class ProfileLocationParser: public RecursiveASTVisitor<ProfileLocationParser> {
    std::map<unsigned int,double> &sbflScores;
    std::vector<std::pair<SourcePositionTy,double>> result;
    const ASTContext *ctxt;
    std::string file;

public:
    ProfileLocationParser(std::map<unsigned int,double> &scores,const ASTContext *ctxt,std::string fileName): sbflScores(scores),result(),ctxt(ctxt),file(fileName) {}

    std::vector<std::pair<SourcePositionTy,double>> getResult() {
        return result;
    }

    virtual bool visitStmt(Stmt *stmt){
        const SourceManager &manager=ctxt->getSourceManager();
        unsigned int line=manager.getExpansionLineNumber(stmt->getBeginLoc());
        if (sbflScores.count(line)!=0){
            SourcePositionTy pos;
            pos.expFilename=manager.getFilename(manager.getExpansionLoc(stmt->getBeginLoc())).str();
            pos.expLine=manager.getExpansionLineNumber(stmt->getBeginLoc());
            pos.expColumn=manager.getExpansionColumnNumber(stmt->getBeginLoc());

            pos.spellFilename=manager.getFilename(manager.getSpellingLoc(stmt->getBeginLoc())).str();
            pos.spellLine=manager.getSpellingLineNumber(stmt->getBeginLoc());
            pos.spellColumn=manager.getSpellingColumnNumber(stmt->getBeginLoc());
            result.push_back(std::make_pair(pos,sbflScores[line]));
        }
        return true;
    }
};