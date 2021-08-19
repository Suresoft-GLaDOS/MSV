#include "ProfileWriter.h"
#include "LocalAnalyzer.h"
#include "SourceContextManager.h"
#include "ASTUtils.h"
#include <clang/AST/RecursiveASTVisitor.h>
#include <fstream>

using namespace clang;

class AddProfileWriter: public RecursiveASTVisitor<AddProfileWriter>{
    BenchProgram &program;
    SourceContextManager &manager;
    ASTContext *ctxt;
    std::string file;
    ImplicitCastExpr *writer;

    std::vector<Stmt*> stmt_stack;

    ASTLocTy getNowLocation(Stmt *n) {
        assert( stmt_stack.size() > 1 );
        Stmt *parent_stmt = stmt_stack[stmt_stack.size() - 2];
        SourceManager &M = ctxt->getSourceManager();
        SourceLocation exp_loc = M.getExpansionLoc(n->getBeginLoc());
        std::string src_file = M.getFilename(exp_loc).str();
        return ASTLocTy(file, src_file, parent_stmt, n);
    }

    Expr* createProfileWriter(SourceContextManager &M, ASTContext *ctxt,const ExprListTy &exprs,const std::string funcName) {
        std::vector<Expr*> tmp_argv;
        tmp_argv.clear();
        StringLiteral *str=StringLiteral::Create(*ctxt,llvm::StringRef(funcName),StringLiteral::StringKind::Ascii,false,ctxt->getConstantArrayType(ctxt->CharTy, llvm::APInt(32, 0),nullptr,ArrayType::Normal,0),SourceLocation());
        tmp_argv.push_back(str);

        std::vector<Expr *> temp;
        temp.clear();
        for (size_t i = 0; i < exprs.size(); ++i) {
            if (exprs[i]->getType().getTypePtr()->isIntegralType(*ctxt) && !exprs[i]->getType().getTypePtr()->isPointerType())
                temp.push_back(exprs[i]);
        }

        tmp_argv.push_back(getNewIntegerLiteral(ctxt, temp.size()));
        for (size_t i = 0; i < temp.size(); ++i) {
            StringLiteral *exprStr=StringLiteral::Create(*ctxt,llvm::StringRef(stmtToString(*ctxt,temp[i])),StringLiteral::StringKind::Ascii,false,ctxt->getConstantArrayType(ctxt->CharTy, llvm::APInt(32, 0),nullptr,ArrayType::Normal,0),SourceLocation());
            tmp_argv.push_back(exprStr);

            tmp_argv.push_back(temp[i]);

        }
        CallExpr *CE = CallExpr::Create(*ctxt, writer, tmp_argv,
                ctxt->IntTy, VK_RValue, SourceLocation());
        return CE;
    }

public:
    AddProfileWriter(BenchProgram &p,SourceContextManager &m,ASTContext *ctxt,std::string file): program(p),manager(m),ctxt(ctxt),file(file) {}
    bool TraverseFunctionDecl(FunctionDecl *decl){
        bool res=RecursiveASTVisitor<AddProfileWriter>::TraverseFunctionDecl(decl);
        if (decl->getNameAsString()=="__write_profile"){
            DeclRefExpr *FRef = DeclRefExpr::Create(*ctxt, NestedNameSpecifierLoc(), SourceLocation(),
                    decl, false, SourceLocation(), decl->getType(), VK_RValue);
            writer= ImplicitCastExpr::Create(*ctxt, ctxt->getDecayedType(decl->getType()), CK_FunctionToPointerDecay,
                    FRef, 0, VK_RValue);
            return res;
        }

        if (!decl->hasBody() || ctxt->getSourceManager().getFilename(decl->getLocation())!=file) return res;
        Stmt *tempBody=decl->getBody();
        CompoundStmt *body=llvm::dyn_cast<CompoundStmt>(tempBody);

        if (body){
            stmt_stack.push_back(body);
            std::vector<Stmt *> newBody;
            newBody.clear();
            for (CompoundStmt::body_iterator it=body->body_begin();it!=body->body_end();it++){
                if (ReturnStmt::classof(*it)){
                    stmt_stack.push_back(*it);
                    ASTLocTy loc=getNowLocation(*it);
                    LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                    ExprListTy exprs=L->genExprAtoms(QualType(),true,true,true,false,false);
                    newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                    stmt_stack.pop_back();
                }
                newBody.push_back(*it);
            }
            if (decl->getReturnType()==ctxt->VoidTy && !ReturnStmt::classof(body->body_back())){
                stmt_stack.push_back(body->body_back());
                ASTLocTy loc=getNowLocation(body->body_back());
                LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                ExprListTy exprs=L->genExprAtoms(QualType(),true,true,true,false,false);
                newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                stmt_stack.pop_back();
            }

            CompoundStmt *newStmt=CompoundStmt::Create(*ctxt,newBody,ctxt->getSourceManager().getExpansionLoc(body->getLBracLoc()),ctxt->getSourceManager().getExpansionLoc(body->getRBracLoc()));
            decl->setBody(newStmt);
            stmt_stack.pop_back();
        }
        return res;
    }
    bool TraverseIfStmt(IfStmt *stmt){
        bool res=RecursiveASTVisitor<AddProfileWriter>::TraverseIfStmt(stmt);
        ASTLocTy temp_loc=getNowLocation(stmt);
        LocalAnalyzer *temp_L = manager.getLocalAnalyzer(temp_loc);
        if (ctxt->getSourceManager().getFilename(temp_L->getCurrentFunction()->getLocation())!=file) return res;

        Stmt *tempBody=stmt->getThen();
        if (tempBody){
            std::vector<Stmt *> newBody;
            newBody.clear();
            if (ReturnStmt::classof(tempBody)){
                stmt_stack.push_back(tempBody);
                ASTLocTy loc=getNowLocation(tempBody);
                LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                ExprListTy exprs=L->genExprAtoms(QualType(),true,true,true,false,false);
                newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                newBody.push_back(tempBody);
                stmt_stack.pop_back();
            }
            else if (CompoundStmt::classof(tempBody)){
                CompoundStmt *body=llvm::dyn_cast<CompoundStmt>(tempBody);
                if (body){
                    stmt_stack.push_back(body);
                    for (CompoundStmt::body_iterator it=body->body_begin();it!=body->body_end();it++){
                        if (ReturnStmt::classof(*it)){
                            stmt_stack.push_back(*it);
                            ASTLocTy loc=getNowLocation(*it);
                            LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                            ExprListTy exprs=L->genExprAtoms(QualType(),true,true,true,false,false);
                            newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                            stmt_stack.pop_back();
                        }
                        newBody.push_back(*it);
                    }
                    stmt_stack.pop_back();
                }
            }
            CompoundStmt *newStmt=CompoundStmt::Create(*ctxt,newBody,ctxt->getSourceManager().getExpansionLoc(tempBody->getBeginLoc()),ctxt->getSourceManager().getExpansionLoc(tempBody->getEndLoc()));
            stmt->setThen(newStmt);
        }

        Stmt *tempElse=stmt->getElse();
        if (tempElse){
            std::vector<Stmt *> newBody;
            newBody.clear();
            if (ReturnStmt::classof(tempElse)){
                stmt_stack.push_back(tempElse);
                ASTLocTy loc=getNowLocation(tempElse);
                LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                ExprListTy exprs=L->genExprAtoms(QualType(),true,true,true,false,false);
                newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                newBody.push_back(tempElse);
                stmt_stack.pop_back();
            }
            else if (CompoundStmt::classof(tempElse)){
                CompoundStmt *body=llvm::dyn_cast<CompoundStmt>(tempElse);
                if (body){
                    stmt_stack.push_back(body);
                    for (CompoundStmt::body_iterator it=body->body_begin();it!=body->body_end();it++){
                        if (ReturnStmt::classof(*it)){
                            stmt_stack.push_back(*it);
                            ASTLocTy loc=getNowLocation(*it);
                            LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                            ExprListTy exprs=L->genExprAtoms(QualType(),true,true,true,false,false);
                            newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                            stmt_stack.pop_back();
                        }
                        newBody.push_back(*it);
                    }
                    stmt_stack.pop_back();
                }
            }
            CompoundStmt *newStmt=CompoundStmt::Create(*ctxt,newBody,ctxt->getSourceManager().getExpansionLoc(tempElse->getBeginLoc()),ctxt->getSourceManager().getExpansionLoc(tempElse->getEndLoc()));
            stmt->setElse(newStmt);
        }

        return res;
    }

    bool TraverseSwitchStmt(SwitchStmt *stmt){
        bool res=RecursiveASTVisitor<AddProfileWriter>::TraverseSwitchStmt(stmt);
        ASTLocTy temp_loc=getNowLocation(stmt);
        LocalAnalyzer *temp_L = manager.getLocalAnalyzer(temp_loc);
        if (ctxt->getSourceManager().getFilename(temp_L->getCurrentFunction()->getLocation())!=file) return res;

        Stmt *tempBody=stmt->getBody();
        if (tempBody){
            stmt_stack.push_back(tempBody);
            std::vector<Stmt *> newBody;
            newBody.clear();
            CompoundStmt *body=llvm::dyn_cast<CompoundStmt>(tempBody);
            for (CompoundStmt::body_iterator it=body->body_begin();it!=body->body_end();it++){
                if (ReturnStmt::classof(*it)){
                    stmt_stack.push_back(*it);
                    ASTLocTy loc=getNowLocation(*it);
                    LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                    ExprListTy exprs=L->genExprAtoms(QualType(),true,true,true,false,false);

                    newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                    newBody.push_back(*it);
                    stmt_stack.pop_back();
                }
                else if (CaseStmt::classof(*it)){
                    stmt_stack.push_back(*it);
                    CaseStmt *caseStmt=llvm::dyn_cast<CaseStmt>(*it);
                    if (caseStmt->getSubStmt()!=NULL){
                        if (ReturnStmt::classof(caseStmt->getSubStmt())){
                            stmt_stack.push_back(caseStmt->getSubStmt());
                            std::vector<Stmt *> newCase;
                            newCase.clear();
                            ASTLocTy loc=getNowLocation(caseStmt->getSubStmt());
                            LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                            ExprListTy exprs=L->genExprAtoms(QualType(),true,true,true,false,false);

                            newCase.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));

                            stmt_stack.pop_back();
                            CompoundStmt *newStmt=CompoundStmt::Create(*ctxt,newCase,ctxt->getSourceManager().getExpansionLoc(caseStmt->getSubStmt()->getBeginLoc()),ctxt->getSourceManager().getExpansionLoc(caseStmt->getSubStmt()->getEndLoc()));
                            caseStmt->setSubStmt(newStmt);
                        }
                        else if (CompoundStmt::classof(caseStmt->getSubStmt())){
                            CompoundStmt *body=llvm::dyn_cast<CompoundStmt>(caseStmt->getSubStmt());
                            if (body){
                                stmt_stack.push_back(body);
                                std::vector<Stmt *> newCase;
                                newCase.clear();
                                for (CompoundStmt::body_iterator it=body->body_begin();it!=body->body_end();it++){
                                    if (ReturnStmt::classof(*it)){
                                        stmt_stack.push_back(*it);
                                        ASTLocTy loc=getNowLocation(*it);
                                        LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                                        ExprListTy exprs=L->genExprAtoms(QualType(),true,true,true,false,false);
                                        newCase.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                                        stmt_stack.pop_back();
                                    }
                                    newCase.push_back(*it);
                                }
                                stmt_stack.pop_back();
                                CompoundStmt *newStmt=CompoundStmt::Create(*ctxt,newCase,ctxt->getSourceManager().getExpansionLoc(caseStmt->getSubStmt()->getBeginLoc()),ctxt->getSourceManager().getExpansionLoc(caseStmt->getSubStmt()->getEndLoc()));
                                caseStmt->setSubStmt(newStmt);
                            }
                        }
                    }
                    stmt_stack.pop_back();
                }
                else if (DefaultStmt::classof(*it)){
                    stmt_stack.push_back(*it);
                    DefaultStmt *caseStmt=llvm::dyn_cast<DefaultStmt>(*it);
                    if (caseStmt->getSubStmt()!=NULL){
                        if (ReturnStmt::classof(caseStmt->getSubStmt())){
                            stmt_stack.push_back(caseStmt->getSubStmt());
                            std::vector<Stmt *> newCase;
                            newCase.clear();
                            ASTLocTy loc=getNowLocation(caseStmt->getSubStmt());
                            LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                            ExprListTy exprs=L->genExprAtoms(QualType(),true,true,true,false,false);

                            newCase.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));

                            stmt_stack.pop_back();
                            CompoundStmt *newStmt=CompoundStmt::Create(*ctxt,newCase,ctxt->getSourceManager().getExpansionLoc(caseStmt->getSubStmt()->getBeginLoc()),ctxt->getSourceManager().getExpansionLoc(caseStmt->getSubStmt()->getEndLoc()));
                            caseStmt->setSubStmt(newStmt);
                        }
                        else if (CompoundStmt::classof(caseStmt->getSubStmt())){
                            CompoundStmt *body=llvm::dyn_cast<CompoundStmt>(caseStmt->getSubStmt());
                            if (body){
                                stmt_stack.push_back(body);
                                std::vector<Stmt *> newCase;
                                newCase.clear();
                                for (CompoundStmt::body_iterator it=body->body_begin();it!=body->body_end();it++){
                                    if (ReturnStmt::classof(*it)){
                                        stmt_stack.push_back(*it);
                                        ASTLocTy loc=getNowLocation(*it);
                                        LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                                        ExprListTy exprs=L->genExprAtoms(QualType(),true,true,true,false,false);
                                        newCase.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                                        stmt_stack.pop_back();
                                    }
                                    newCase.push_back(*it);
                                }
                                stmt_stack.pop_back();
                                CompoundStmt *newStmt=CompoundStmt::Create(*ctxt,newCase,ctxt->getSourceManager().getExpansionLoc(caseStmt->getSubStmt()->getBeginLoc()),ctxt->getSourceManager().getExpansionLoc(caseStmt->getSubStmt()->getEndLoc()));
                                caseStmt->setSubStmt(newStmt);
                            }
                        }
                    }
                    stmt_stack.pop_back();
                }
                else{
                    newBody.push_back(*it);
                }
            }
            stmt_stack.pop_back();
        }
        return res;
    }

    bool TraverseStmt(Stmt *n) {
        stmt_stack.push_back(n);
        bool ret = RecursiveASTVisitor::TraverseStmt(n);
        stmt_stack.pop_back();
        return ret;
    }

};

bool addProfileWriter(BenchProgram &P,SourceContextManager &manager,std::map<std::string, std::string> &fileCode,std::vector<long long> &macros,std::string output_file){
    for (std::map<std::string, std::string>::iterator it=fileCode.begin();it!=fileCode.end();it++){
        manager.fetch(it->first);
        ASTContext *ctxt=manager.getSourceContext(it->first);
        AddProfileWriter writer(P,manager,ctxt,it->first);
        writer.TraverseTranslationUnitDecl(ctxt->getTranslationUnitDecl());
        std::error_code temp;
        llvm::raw_fd_ostream fout(P.getSrcdir()+"/"+it->first,temp);
        // printf(temp.message().c_str());
        ctxt->getTranslationUnitDecl()->print(fout);
        fout.close();

        size_t filePos=it->first.rfind("/");
        std::string bak_file=P.getWorkdir()+"/"+output_file+it->first.substr(filePos+1);
        llvm::raw_fd_ostream bak_out(bak_file,temp);
        ctxt->getTranslationUnitDecl()->print(bak_out);
        bak_out.close();
        outlog_printf(2,"Saved fixed file at: %s\n",bak_file.c_str());
    }
    return true;
}