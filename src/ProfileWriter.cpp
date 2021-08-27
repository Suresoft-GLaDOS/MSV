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
    std::vector<Stmt *> added_function;

    ASTLocTy getNowLocation(Stmt *n) {
        assert( stmt_stack.size() > 1 );
        Stmt *parent_stmt = stmt_stack[stmt_stack.size() - 2];
        SourceManager &M = ctxt->getSourceManager();
        SourceLocation exp_loc = M.getExpansionLoc(n->getBeginLoc());
        std::string src_file = M.getFilename(exp_loc).str();
        return ASTLocTy(file, src_file, parent_stmt, n);
    }

    Stmt* createProfileWriter(SourceContextManager &M, ASTContext *ctxt,const ExprListTy &exprs,const std::string funcName) {
        std::vector<Expr*> tmp_argv;
        tmp_argv.clear();
        StringLiteral *str=StringLiteral::Create(*ctxt,llvm::StringRef(funcName),StringLiteral::StringKind::Ascii,false,ctxt->getConstantArrayType(ctxt->CharTy, llvm::APInt(32, 0),nullptr,ArrayType::Normal,0),SourceLocation());
        tmp_argv.push_back(str);
        IntegerLiteral *modeArg=getNewIntegerLiteral(ctxt,0);
        tmp_argv.push_back(modeArg);

        std::vector<Expr *> temp;
        temp.clear();
        std::map<Expr *,std::vector<Expr *>> arrowMembers;
        arrowMembers.clear();
        std::vector<UnaryOperator *> pointers;
        pointers.clear();
        std::map<Expr *,std::vector<UnaryOperator *>> arrowPointers;
        arrowPointers.clear();
        for (size_t i = 0; i < exprs.size(); ++i) {
            ImplicitCastExpr *tempExpr=llvm::dyn_cast<ImplicitCastExpr>(exprs[i]);
            // First, we use integral types(e.g. int, long, unsigned) only, without pointers
            if (tempExpr->getSubExpr()->getType().getTypePtr()->isIntegralType(*ctxt) && !tempExpr->getSubExpr()->getType().getTypePtr()->isPointerType()){
                if (MemberExpr::classof(tempExpr->getSubExpr())){
                    MemberExpr *member=llvm::dyn_cast<MemberExpr>(tempExpr->getSubExpr());

                    // If member is arrow
                    if (member->isArrow()){
                        Expr *base=nullptr;
                        for (std::map<Expr *,std::vector<Expr *>>::iterator it=arrowMembers.begin();it!=arrowMembers.end();it++){
                            if(stmtToString(*ctxt,it->first)==stmtToString(*ctxt,member->getBase())){
                                base=it->first;
                                break;
                            }
                        }
                        if (base==nullptr){
                            arrowMembers[member->getBase()]=std::vector<Expr *>();
                            base=member->getBase();
                        }
                        arrowMembers[base].push_back(tempExpr);
                    }

                    else
                        temp.push_back(exprs[i]);
                }
                else temp.push_back(exprs[i]);
            }

            // Then, we handle pointer of integral types, without member
            // Add null checker
            else if (tempExpr->getSubExpr()->getType().getTypePtr()->isPointerType()){
                // We ignore pointer of pointer
                if (MemberExpr::classof(tempExpr->getSubExpr())){
                    MemberExpr *member=llvm::dyn_cast<MemberExpr>(tempExpr->getSubExpr());
                    if (member->isArrow()){
                        Expr *base=nullptr;
                        for (std::map<Expr *,std::vector<UnaryOperator *>>::iterator it=arrowPointers.begin();it!=arrowPointers.end();it++){
                            if(stmtToString(*ctxt,it->first)==stmtToString(*ctxt,member->getBase())){
                                base=it->first;
                                break;
                            }
                        }
                        if (base==nullptr){
                            arrowPointers[member->getBase()]=std::vector<UnaryOperator *>();
                            base=member->getBase();
                        }
                        // This member is pointer, so dereference is needed
                        UnaryOperator *deref=UnaryOperator::Create(*ctxt,member,UO_Deref,member->getType().getTypePtr()->getPointeeType(),VK_RValue,OK_Ordinary,SourceLocation(),false,FPOptionsOverride());
                        arrowPointers[base].push_back(deref);
                    }
                    else if (tempExpr->getSubExpr()->getType().getTypePtr()->getPointeeType().getTypePtr()->isIntegralType(*ctxt) && !tempExpr->getSubExpr()->getType().getTypePtr()->getPointeeType().getTypePtr()->isPointerType()){
                        UnaryOperator *deref=UnaryOperator::Create(*ctxt,member,UO_Deref,member->getType().getTypePtr()->getPointeeType(),VK_RValue,OK_Ordinary,SourceLocation(),false,FPOptionsOverride());
                        pointers.push_back(deref);
                    }
                }
                else if (tempExpr->getSubExpr()->getType().getTypePtr()->getPointeeType().getTypePtr()->isIntegralType(*ctxt) && !tempExpr->getSubExpr()->getType().getTypePtr()->getPointeeType().getTypePtr()->isPointerType()){
                    UnaryOperator *deref=UnaryOperator::Create(*ctxt,exprs[i],UO_Deref,exprs[i]->getType().getTypePtr()->getPointeeType(),VK_RValue,OK_Ordinary,SourceLocation(),false,FPOptionsOverride());
                    pointers.push_back(deref);
                }
            }
        }

        std::vector<Stmt *> finalStmts;
        finalStmts.clear();
        // Add basic types, without pointer and arrow member
        tmp_argv.push_back(getNewIntegerLiteral(ctxt, temp.size()));
        for (size_t i = 0; i < temp.size(); ++i) {
            StringLiteral *exprStr=StringLiteral::Create(*ctxt,llvm::StringRef(stmtToString(*ctxt,temp[i])),StringLiteral::StringKind::Ascii,false,ctxt->getConstantArrayType(ctxt->CharTy, llvm::APInt(32, 0),nullptr,ArrayType::Normal,0),SourceLocation());
            tmp_argv.push_back(exprStr);

            ParenExpr *ParenE1 = new (*ctxt) ParenExpr(SourceLocation(), SourceLocation(), temp[i]);
            UnaryOperator *AddrsOf = 
                    UnaryOperator::Create(*ctxt,ParenE1, UO_AddrOf, ctxt->getPointerType(temp[i]->getType()),
                            VK_RValue, OK_Ordinary, SourceLocation(),false,FPOptionsOverride());
            tmp_argv.push_back(AddrsOf);

            ParenExpr *ParenE2 = new (*ctxt) ParenExpr(SourceLocation(), SourceLocation(), temp[i]);
            UnaryExprOrTypeTraitExpr *SizeofE = new (*ctxt) UnaryExprOrTypeTraitExpr(
                    UETT_SizeOf, ParenE2, ctxt->UnsignedLongTy, SourceLocation(), SourceLocation());
            tmp_argv.push_back(SizeofE);
        }
        CallExpr *CE = CallExpr::Create(*ctxt, writer, tmp_argv,
                ctxt->IntTy, VK_RValue, SourceLocation());
        finalStmts.push_back(CE);

        // Add pointers of integral types
        IntegerLiteral *modeOne=getNewIntegerLiteral(ctxt,1);
        IntegerLiteral *null=getNewIntegerLiteral(ctxt,0);
        CStyleCastExpr *cast=CStyleCastExpr::Create(*ctxt,ctxt->VoidPtrTy,VK_RValue,CK_IntegralToPointer,null,nullptr,ctxt->CreateTypeSourceInfo(ctxt->VoidPtrTy),SourceLocation(),SourceLocation());
        for (size_t i=0;i<pointers.size();i++){
            BinaryOperator *check=BinaryOperator::Create(*ctxt,pointers[i]->getSubExpr(),cast,BO_NE,ctxt->BoolTy,VK_RValue,OK_Ordinary,SourceLocation(),FPOptionsOverride());

            std::vector<Expr *> pointerArgs;
            pointerArgs.clear();
            pointerArgs.push_back(str);
            pointerArgs.push_back(modeOne);
            pointerArgs.push_back(modeOne);

            StringLiteral *exprStr=StringLiteral::Create(*ctxt,llvm::StringRef(stmtToString(*ctxt,pointers[i])),StringLiteral::StringKind::Ascii,false,ctxt->getConstantArrayType(ctxt->CharTy, llvm::APInt(32, 0),nullptr,ArrayType::Normal,0),SourceLocation());
            pointerArgs.push_back(exprStr);

            ParenExpr *ParenE1 = new (*ctxt) ParenExpr(SourceLocation(), SourceLocation(), pointers[i]);
            UnaryOperator *AddrsOf = 
                    UnaryOperator::Create(*ctxt,ParenE1, UO_AddrOf, ctxt->getPointerType(pointers[i]->getType()),
                            VK_RValue, OK_Ordinary, SourceLocation(),false,FPOptionsOverride());
            pointerArgs.push_back(AddrsOf);

            ParenExpr *ParenE2 = new (*ctxt) ParenExpr(SourceLocation(), SourceLocation(), pointers[i]);
            UnaryExprOrTypeTraitExpr *SizeofE = new (*ctxt) UnaryExprOrTypeTraitExpr(
                    UETT_SizeOf, ParenE2, ctxt->UnsignedLongTy, SourceLocation(), SourceLocation());
            pointerArgs.push_back(SizeofE);

            CallExpr *write = CallExpr::Create(*ctxt, writer, pointerArgs, ctxt->IntTy, VK_RValue, SourceLocation());
            IfStmt *checker=IfStmt::Create(*ctxt,SourceLocation(),false,nullptr,nullptr,check,write);
            finalStmts.push_back(checker);
        }

        // Add arrow member
        for (std::map<Expr *,std::vector<Expr *>>::iterator it=arrowMembers.begin();it!=arrowMembers.end();it++){
            BinaryOperator *check=BinaryOperator::Create(*ctxt,it->first,cast,BO_NE,ctxt->BoolTy,VK_RValue,OK_Ordinary,SourceLocation(),FPOptionsOverride());

            std::vector<Expr *> pointerArgs;
            pointerArgs.clear();
            pointerArgs.push_back(str);
            pointerArgs.push_back(modeOne);
            pointerArgs.push_back(getNewIntegerLiteral(ctxt,it->second.size()));

            for (size_t i=0;i<it->second.size();i++){
                StringLiteral *exprStr=StringLiteral::Create(*ctxt,llvm::StringRef(stmtToString(*ctxt,it->second[i])),StringLiteral::StringKind::Ascii,false,ctxt->getConstantArrayType(ctxt->CharTy, llvm::APInt(32, 0),nullptr,ArrayType::Normal,0),SourceLocation());
                pointerArgs.push_back(exprStr);

                ParenExpr *ParenE1 = new (*ctxt) ParenExpr(SourceLocation(), SourceLocation(), it->second[i]);
                UnaryOperator *AddrsOf = 
                        UnaryOperator::Create(*ctxt,ParenE1, UO_AddrOf, ctxt->getPointerType(it->second[i]->getType()),
                                VK_RValue, OK_Ordinary, SourceLocation(),false,FPOptionsOverride());
                pointerArgs.push_back(AddrsOf);

                ParenExpr *ParenE2 = new (*ctxt) ParenExpr(SourceLocation(), SourceLocation(), it->second[i]);
                UnaryExprOrTypeTraitExpr *SizeofE = new (*ctxt) UnaryExprOrTypeTraitExpr(
                        UETT_SizeOf, ParenE2, ctxt->UnsignedLongTy, SourceLocation(), SourceLocation());
                pointerArgs.push_back(SizeofE);
            }

            CallExpr *write = CallExpr::Create(*ctxt, writer, pointerArgs, ctxt->IntTy, VK_RValue, SourceLocation());
            IfStmt *checker=IfStmt::Create(*ctxt,SourceLocation(),false,nullptr,nullptr,check,write);
            finalStmts.push_back(checker);
        }

        CompoundStmt *finalStmt=CompoundStmt::Create(*ctxt,finalStmts,SourceLocation(),SourceLocation());
        return finalStmt;
    }

public:
    AddProfileWriter(BenchProgram &p,SourceContextManager &m,ASTContext *ctxt,std::string file): program(p),manager(m),ctxt(ctxt),file(file),added_function() {}
    std::vector<Stmt *> getProcessedFuncBody(){
        return added_function;
    }
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
                    ExprListTy exprs=L->getProfileWriterExpr();
                    newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                    stmt_stack.pop_back();
                }
                newBody.push_back(*it);
            }
            if (decl->getReturnType()==ctxt->VoidTy && !ReturnStmt::classof(body->body_back())){
                stmt_stack.push_back(body->body_back());
                ASTLocTy loc=getNowLocation(body->body_back());
                LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                ExprListTy exprs=L->getProfileWriterExpr();
                newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                stmt_stack.pop_back();
            }

            CompoundStmt *newStmt=CompoundStmt::Create(*ctxt,newBody,ctxt->getSourceManager().getExpansionLoc(body->getLBracLoc()),ctxt->getSourceManager().getExpansionLoc(body->getRBracLoc()));
            decl->setBody(newStmt);
            stmt_stack.pop_back();

            added_function.push_back(decl->getBody());
        }
        return res;
    }
    bool TraverseIfStmt(IfStmt *stmt){
        bool res=RecursiveASTVisitor<AddProfileWriter>::TraverseIfStmt(stmt);
        // return res;

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
                ExprListTy exprs=L->getProfileWriterExpr();
                newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                newBody.push_back(tempBody);
                stmt_stack.pop_back();

                CompoundStmt *newStmt=CompoundStmt::Create(*ctxt,newBody,ctxt->getSourceManager().getExpansionLoc(tempBody->getBeginLoc()),ctxt->getSourceManager().getExpansionLoc(tempBody->getEndLoc()));
                stmt->setThen(newStmt);
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
                            ExprListTy exprs=L->getProfileWriterExpr();
                            newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                            stmt_stack.pop_back();
                        }
                        newBody.push_back(*it);
                    }
                    stmt_stack.pop_back();
                }

                CompoundStmt *newStmt=CompoundStmt::Create(*ctxt,newBody,ctxt->getSourceManager().getExpansionLoc(tempBody->getBeginLoc()),ctxt->getSourceManager().getExpansionLoc(tempBody->getEndLoc()));
                stmt->setThen(newStmt);
            }
        }

        Stmt *tempElse=stmt->getElse();
        if (tempElse){
            std::vector<Stmt *> newBody;
            newBody.clear();
            if (ReturnStmt::classof(tempElse)){
                stmt_stack.push_back(tempElse);
                ASTLocTy loc=getNowLocation(tempElse);
                LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                ExprListTy exprs=L->getProfileWriterExpr();
                newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                newBody.push_back(tempElse);
                stmt_stack.pop_back();

                CompoundStmt *newStmt=CompoundStmt::Create(*ctxt,newBody,ctxt->getSourceManager().getExpansionLoc(tempElse->getBeginLoc()),ctxt->getSourceManager().getExpansionLoc(tempElse->getEndLoc()));
                stmt->setElse(newStmt);
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
                            ExprListTy exprs=L->getProfileWriterExpr();
                            newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                            stmt_stack.pop_back();
                        }
                        newBody.push_back(*it);
                    }
                    stmt_stack.pop_back();
                }

                CompoundStmt *newStmt=CompoundStmt::Create(*ctxt,newBody,ctxt->getSourceManager().getExpansionLoc(tempElse->getBeginLoc()),ctxt->getSourceManager().getExpansionLoc(tempElse->getEndLoc()));
                stmt->setElse(newStmt);
            }
        }

        return res;
    }

    bool TraverseSwitchStmt(SwitchStmt *stmt){
        bool res=RecursiveASTVisitor<AddProfileWriter>::TraverseSwitchStmt(stmt);
        // return res;

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
                    ExprListTy exprs=L->getProfileWriterExpr();

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
                            ExprListTy exprs=L->getProfileWriterExpr();

                            newCase.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                            newBody.push_back(caseStmt->getSubStmt());

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
                                        ExprListTy exprs=L->getProfileWriterExpr();
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
                            ExprListTy exprs=L->getProfileWriterExpr();

                            newCase.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                            newBody.push_back(caseStmt->getSubStmt());

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
                                        ExprListTy exprs=L->getProfileWriterExpr();
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

bool addProfileWriter(BenchProgram &P,std::map<std::string, std::string> &fileCode,std::vector<long long> &macros,std::string output_file){
    SourceContextManager manager(P,false);
    for (std::map<std::string, std::string>::iterator it=fileCode.begin();it!=fileCode.end();it++){
        ASTContext *ctxt=manager.getSourceContext(it->first);
        it->second=manager.getSourceCode(it->first);
        SourceManager &m=ctxt->getSourceManager();

        AddProfileWriter writer(P,manager,ctxt,it->first);
        writer.TraverseTranslationUnitDecl(ctxt->getTranslationUnitDecl());
        
        std::vector<Stmt *> processed=writer.getProcessedFuncBody();
        std::string backup=it->second;
        std::vector<std::string> codeSegs;
        std::vector<std::string> patches;
        codeSegs.push_back(it->second);
        size_t beforeLast=0;

        for (size_t i=0;i<processed.size();i++){
            if (m.getFilename(m.getExpansionLoc(processed[i]->getBeginLoc()))==it->first){
                size_t start=m.getFileOffset(m.getExpansionLoc(processed[i]->getBeginLoc()));
                size_t end=m.getFileOffset(m.getExpansionLoc(processed[i]->getEndLoc()))+1;
                // while (end < backup.size()) {
                //     if (backup[end] == '\n') {
                //         end ++;
                //         break;
                //     }
                //     end ++;
                // }

                std::string lastSeg=codeSegs.back();
                codeSegs.pop_back();

                std::string first=lastSeg.substr(0,start-beforeLast);
                codeSegs.push_back(first);
                std::string func=stmtToString(*ctxt,processed[i]);
                
                patches.push_back(func);
                codeSegs.push_back(backup.substr(end));
                beforeLast=end;
            }
        }

        std::string finalCode="";
        for (size_t i=0;i<patches.size();i++){
            finalCode+=codeSegs[i];
            finalCode+=patches[i];
        }
        finalCode+=codeSegs.back();
        it->second=finalCode;
        // std::error_code temp;
        // llvm::raw_fd_ostream fout(P.getSrcdir()+"/"+it->first,temp);
        // // printf(temp.message().c_str());
        // ctxt->getTranslationUnitDecl()->print(fout);
        // fout.close();
        std::string fileName=P.getSrcdir()+"/"+it->first;
        std::ofstream fo(fileName.c_str());
        fo << it->second;
        fo.close();

        size_t filePos=it->first.rfind("/");
        std::string bak_file=P.getWorkdir()+"/"+output_file+it->first.substr(filePos+1);
        std::ofstream bak_fo(bak_file.c_str());
        bak_fo << it->second;
        bak_fo.close();

        outlog_printf(2,"Saved fixed file at: %s\n",bak_file.c_str());
    }
    return true;
}