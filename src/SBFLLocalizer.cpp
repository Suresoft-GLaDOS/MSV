#include <clang/AST/AST.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <iostream>
#include "cJSON/cJSON.h"
#include "SBFLLocalizer.h"
#include "Utils.h"

using namespace clang;

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

    bool VisitCompoundStmt(CompoundStmt *stmt){
        const SourceManager &manager=ctxt->getSourceManager();
        for (CompoundStmt::const_body_iterator it=stmt->body_begin();it!=stmt->body_end();it++){
            unsigned int line=manager.getExpansionLineNumber((*it)->getBeginLoc());
            if (sbflScores.count(line)!=0){
                SourcePositionTy pos;
                pos.expFilename=manager.getFilename(manager.getExpansionLoc((*it)->getBeginLoc())).str();
                pos.expLine=manager.getExpansionLineNumber((*it)->getBeginLoc());
                pos.expColumn=manager.getExpansionColumnNumber((*it)->getBeginLoc());

                pos.spellFilename=manager.getFilename(manager.getSpellingLoc((*it)->getBeginLoc())).str();
                pos.spellLine=manager.getSpellingLineNumber((*it)->getBeginLoc());
                pos.spellColumn=manager.getSpellingColumnNumber((*it)->getBeginLoc());
                result.push_back(std::make_pair(pos,sbflScores[line]));
            }
        }
        return true;
    }

    bool VisitIfStmt(IfStmt *stmt){
        if (stmt == NULL) return true;
        const SourceManager &manager=ctxt->getSourceManager();
        Stmt* thenS = stmt->getThen();
        Stmt* elseS = stmt->getElse();

        if (thenS!=nullptr && !CompoundStmt::classof(thenS)){
            unsigned int line=manager.getExpansionLineNumber(thenS->getBeginLoc());
            if (sbflScores.count(line)!=0){
                SourcePositionTy pos;
                pos.expFilename=manager.getFilename(manager.getExpansionLoc(thenS->getBeginLoc())).str();
                pos.expLine=manager.getExpansionLineNumber(thenS->getBeginLoc());
                pos.expColumn=manager.getExpansionColumnNumber(thenS->getBeginLoc());

                pos.spellFilename=manager.getFilename(manager.getSpellingLoc(thenS->getBeginLoc())).str();
                pos.spellLine=manager.getSpellingLineNumber(thenS->getBeginLoc());
                pos.spellColumn=manager.getSpellingColumnNumber(thenS->getBeginLoc());
                result.push_back(std::make_pair(pos,sbflScores[line]));
            }
        }

        if (elseS!=nullptr && !CompoundStmt::classof(elseS)){
            unsigned int line=manager.getExpansionLineNumber(elseS->getBeginLoc());
            if (sbflScores.count(line)!=0){
                SourcePositionTy pos;
                pos.expFilename=manager.getFilename(manager.getExpansionLoc(elseS->getBeginLoc())).str();
                pos.expLine=manager.getExpansionLineNumber(elseS->getBeginLoc());
                pos.expColumn=manager.getExpansionColumnNumber(elseS->getBeginLoc());

                pos.spellFilename=manager.getFilename(manager.getSpellingLoc(elseS->getBeginLoc())).str();
                pos.spellLine=manager.getSpellingLineNumber(elseS->getBeginLoc());
                pos.spellColumn=manager.getSpellingColumnNumber(elseS->getBeginLoc());
                result.push_back(std::make_pair(pos,sbflScores[line]));
            }
        }

        return true;
    }
};

bool skipFile(std::string &fileName){
    return fileName=="crypto/des/ncbc_enc.c" || fileName=="providers/implementations/ciphers/cipher_aes_cts.inc";
}

SBFLLocalizer::SBFLLocalizer(std::string fileName,BenchProgram *program): SBFLFile(fileName),program(program),result() {
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

        if (score!=0.0){
            if (sbflResult.count(file_name)==0)
                sbflResult[file_name]=std::map<unsigned int,double>();
            
            sbflResult[file_name][line]=score;
        }
    }

    // Get Profile location from sbfl
    // We use <score,location> for sorting descending order(high score = high priority)
    std::vector<std::pair<double,SourcePositionTy>> sortedResult;
    sortedResult.clear();
    for (std::map<std::string,std::map<unsigned int,double>>::iterator it=sbflResult.begin();it!=sbflResult.end();it++){
        std::string fileName=it->first;
        if (fileName.find("//")!=std::string::npos){
            size_t loc=fileName.find("//");
            fileName=fileName.substr(loc+2);
        }
        if (is_header(fileName)) continue; // Skip header files
        if (skipFile(fileName)) continue; // Skip invalid files
        std::string fullPath=program->getSrcdir()+"/"+fileName;
        std::string code;
        readCodeToString(fullPath,code);
        std::unique_ptr<ASTUnit> unit=program->buildClangASTUnit(fileName,code);
        
        ASTContext &ctxt=unit->getASTContext();
        ProfileLocationParser parser(it->second,&ctxt,fileName);
        TranslationUnitDecl *decl=ctxt.getTranslationUnitDecl();
        bool res=parser.TraverseDecl(decl);

        std::vector<std::pair<SourcePositionTy,double>> result=parser.getResult();
        for (size_t i=0;i<result.size();i++){
            sortedResult.push_back(std::make_pair(result[i].second,result[i].first));
        }
    }

    // Sort FL results
    std::sort(sortedResult.begin(),sortedResult.end(),std::greater<std::pair<double,SourcePositionTy>>());
    result=sortedResult;

    printResult(program->getLocalizationResultFilename(),program->getLocalizationResultBackupFilename());
}

std::vector<SourcePositionTy> SBFLLocalizer::getCandidateLocations(){
    std::vector<SourcePositionTy> ret;
    for (size_t i=0;i<result.size();i++){
        ret.push_back(result[i].second);
    }
    return ret;
}

std::vector<ProfileErrorLocalizer::ResRecordTy> SBFLLocalizer::getCandidates(){
    std::vector<ProfileErrorLocalizer::ResRecordTy> ret;
    for (size_t i=0;i<result.size();i++){
        ProfileErrorLocalizer::ResRecordTy record;
        record.primeScore=(long long)(result[i].first*1000000);
        record.secondScore=5;
        record.loc=result[i].second;
        record.pid="0";
        ret.push_back(record);
    }
    return ret;
}

void SBFLLocalizer::printResult(const std::string &outfile,const std::string backupFile){
    std::string cmd;
    cmd="cp -rf "+outfile+" "+backupFile;
    system(cmd.c_str());

    std::ofstream file(outfile);
    for (size_t i=0;i<result.size();i++){
        file<<result[i].second.expFilename<<" "<<result[i].second.expLine<<" "<<result[i].second.expColumn<<" "
                <<result[i].second.spellFilename  <<" "<< result[i].second.spellLine <<" "<< result[i].second.spellColumn<<"\t\t"
                <<(size_t)(result[i].first*1000000) << "\t\t" << 0 << "\t\t" << 0 << std::endl;
    }
    file.close();
}

void SBFLLocalizer::printResult(const std::string &outfile){
    std::ofstream file(outfile);
    for (size_t i=0;i<result.size();i++){
        file<<result[i].second.expFilename<<" "<<result[i].second.expLine<<" "<<result[i].second.expColumn<<" "
                <<result[i].second.spellFilename  <<" "<< result[i].second.spellLine <<" "<< result[i].second.spellColumn<<"\t\t"
                <<(size_t)(result[i].first*1000000) << "\t\t" << 0 << "\t\t" << 0 << std::endl;
    }
    file.close();
}