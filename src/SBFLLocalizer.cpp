#include <clang/AST/AST.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include "cJSON/cJSON.h"
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

std::vector<SourcePositionTy> SBFLLocalizer::getCandidateLocations() {
    // Get sbfl result
    std::ifstream file(SBFLFile);
    std::string rootString="";
    std::string line;
    while (std::getline(file,line)){
        rootString+=line+"\n";
    }
    file.close();

    std::map<std::string,std::map<unsigned int,double>> sbflResult;
    sbflResult.clear();
    cJSON *root=cJSON_Parse(rootString.c_str());
    cJSON *location=NULL;
    cJSON_ArrayForEach(location,root){
        cJSON *file_elem=cJSON_GetArrayItem(location,0);
        std::string file_name(cJSON_GetStringValue(file_elem));
        unsigned int line=(unsigned int)cJSON_GetNumberValue(cJSON_GetArrayItem(location,1));
        double score=cJSON_GetNumberValue(cJSON_GetArrayItem(location,2));

        if (sbflResult.count(file_name)==0)
            sbflResult[file_name]=std::map<unsigned int,double>();
        
        sbflResult[file_name][line]=score;
    }

    // Ge Profile location from sbfl
    for (std::map<std::string,std::map<unsigned int,double>>::iterator it=sbflResult.begin();it!=sbflResult.end();it++){
        std::string fullPath=program->getSrcdir()+"/"+it->first;
        std::string code;
        readCodeToString(fullPath,code);
        std::unique_ptr<ASTUnit> unit=program->buildClangASTUnit(fullPath,code);
        
        ASTContext &ctxt=unit->getASTContext();
        ProfileLocationParser parser(it->second,&ctxt,it->first);
        parser.TraverseTranslationUnitDecl(ctxt.getTranslationUnitDecl());

        std::vector<std::pair<SourcePositionTy,double>> result=parser.getResult();
    }
}