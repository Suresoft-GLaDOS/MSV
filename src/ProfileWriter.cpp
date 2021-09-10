#include "ProfileWriter.h"
#include "LocalAnalyzer.h"
#include "SourceContextManager.h"
#include "ASTUtils.h"
#include <clang/AST/RecursiveASTVisitor.h>
#include <fstream>

using namespace clang;

static size_t count=0;
class AddProfileWriter: public RecursiveASTVisitor<AddProfileWriter>{
    BenchProgram &program;
    SourceContextManager &manager;
    ASTContext *ctxt;
    std::string file;
    ImplicitCastExpr *writer;
    std::string code;

    std::vector<Stmt*> stmt_stack;
    std::map<std::pair<size_t,size_t>,std::string> profile_writer;
    std::map<long long,std::string> macroCode;
    std::vector<long long> macroFile;

    ASTLocTy getNowLocation(Stmt *n) {
        assert( stmt_stack.size() > 1 );
        Stmt *parent_stmt = stmt_stack[stmt_stack.size() - 2];
        SourceManager &M = ctxt->getSourceManager();
        SourceLocation exp_loc = M.getExpansionLoc(n->getBeginLoc());
        std::string src_file = M.getFilename(exp_loc).str();
        return ASTLocTy(file, src_file, parent_stmt, n);
    }

    std::string createProfileWriter(SourceContextManager &M, ASTContext *ctxt,const ExprListTy &exprs,const std::string funcName) {
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

                    else if (member->getBase()->getType().getTypePtr()->isPointerType()){
                        member->setArrow(true);
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
                else {
                    temp.push_back(exprs[i]);
                }
            }

            // Then, we handle pointer of integral types, without member
            // Add null checker
            else if (tempExpr->getSubExpr()->getType().getTypePtr()->isPointerType()){
                // We ignore pointer of pointer
                if (MemberExpr::classof(tempExpr->getSubExpr())){
                    MemberExpr *member=llvm::dyn_cast<MemberExpr>(tempExpr->getSubExpr());

                    if (member->isArrow() || member->getBase()->getType().getTypePtr()->isPointerType()){
                        member->setArrow(true);
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
        std::string result="";
        StringLiteral *str=StringLiteral::Create(*ctxt,llvm::StringRef(funcName),StringLiteral::StringKind::Ascii,false,ctxt->getConstantArrayType(ctxt->CharTy, llvm::APInt(32, 0),nullptr,ArrayType::Normal,0),SourceLocation());
        // Add basic types, without pointer and arrow member
        for (size_t i = 0; i < temp.size(); ++i) {
            std::vector<Expr*> tmp_argv;
            tmp_argv.clear();
            tmp_argv.push_back(str);
            IntegerLiteral *modeArg;
            if (i==0)
                modeArg=getNewIntegerLiteral(ctxt,0);
            else modeArg=getNewIntegerLiteral(ctxt,1);
            tmp_argv.push_back(modeArg);

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

            CallExpr *CE = CallExpr::Create(*ctxt, writer, tmp_argv,
                ctxt->IntTy, VK_RValue, SourceLocation());

            result+="\n#ifdef __PROFILE_"+std::to_string(count)+"\n";
            result+=stmtToString(*ctxt,CE)+";\n";
            result+="\n#endif\n";
            macroFile.push_back(count);
            macroCode[count++]=stmtToString(*ctxt,CE);
        }

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
            
            result+="\n#ifdef __PROFILE_"+std::to_string(count)+"\n";
            result+=stmtToString(*ctxt,checker)+";\n";
            result+="\n#endif\n";
            macroFile.push_back(count);
            macroCode[count++]=stmtToString(*ctxt,checker);
        }

        // Add arrow member
        for (std::map<Expr *,std::vector<Expr *>>::iterator it=arrowMembers.begin();it!=arrowMembers.end();it++){
            BinaryOperator *check=BinaryOperator::Create(*ctxt,it->first,cast,BO_NE,ctxt->BoolTy,VK_RValue,OK_Ordinary,SourceLocation(),FPOptionsOverride());

            for (size_t i=0;i<it->second.size();i++){
                std::vector<Expr *> pointerArgs;
                pointerArgs.clear();
                pointerArgs.push_back(str);
                pointerArgs.push_back(modeOne);

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

                CallExpr *write = CallExpr::Create(*ctxt, writer, pointerArgs, ctxt->IntTy, VK_RValue, SourceLocation());
                IfStmt *checker=IfStmt::Create(*ctxt,SourceLocation(),false,nullptr,nullptr,check,write);

                result+="\n#ifdef __PROFILE_"+std::to_string(count)+"\n";
                result+=stmtToString(*ctxt,checker)+";\n";
                result+="\n#endif\n";
                macroFile.push_back(count);
                macroCode[count++]=stmtToString(*ctxt,checker);
            }
        }

        return result;
    }

public:
    AddProfileWriter(BenchProgram &p,SourceContextManager &m,ASTContext *ctxt,std::string file,std::string code): program(p),manager(m),ctxt(ctxt),file(file),profile_writer(),macroCode(),macroFile(),code(code) {}
    std::map<std::pair<size_t,size_t>,std::string> getProcessedFuncBody(){
        return profile_writer;
    }
    std::map<long long,std::string> getMacroCode(){
        return macroCode;
    }
    std::vector<long long> getMacroFile(){
        return macroFile;
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

        if (!decl->doesThisDeclarationHaveABody() || ctxt->getSourceManager().getFilename(decl->getLocation())!=file) return res;
        Stmt *tempBody=decl->getBody();
        CompoundStmt *body=llvm::dyn_cast<CompoundStmt>(tempBody);

        if (body){
            stmt_stack.push_back(body);
            for (CompoundStmt::body_iterator it=body->body_begin();it!=body->body_end();it++){
                size_t start=ctxt->getSourceManager().getFileOffset(ctxt->getSourceManager().getExpansionLoc((*it)->getBeginLoc()));
                if (ReturnStmt::classof(*it)){
                    stmt_stack.push_back(*it);
                    ASTLocTy loc=getNowLocation(*it);
                    LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                    ExprListTy exprs=L->getProfileWriterExpr();
                    profile_writer[std::make_pair(start,start)]=createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString());
                    stmt_stack.pop_back();
                }
            }
            if (decl->getReturnType()==ctxt->VoidTy){
                size_t start=ctxt->getSourceManager().getFileOffset(ctxt->getSourceManager().getExpansionLoc(body->getBeginLoc()));
                if (body->body_empty()){
                    stmt_stack.push_back(body);
                    ASTLocTy loc=getNowLocation(body);
                    LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                    ExprListTy exprs=L->getProfileWriterExpr();
                    profile_writer[std::make_pair(start+2,start+2)]=createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString());
                    stmt_stack.pop_back();
                }
                else if (!ReturnStmt::classof(body->body_back())){
                    // CharSourceRange range=ctxt->getSourceManager().getExpansionRange(body->body_back()->getSourceRange());
                    // start=ctxt->getSourceManager().getFileOffset(range.getEnd())+1;
                    // start=code.find(";",start)+1;
                    start=ctxt->getSourceManager().getFileOffset(ctxt->getSourceManager().getExpansionLoc(body->getEndLoc()))-1;
                    stmt_stack.push_back(body->body_back());
                    ASTLocTy loc=getNowLocation(body->body_back());
                    LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                    ExprListTy exprs=L->getProfileWriterExpr();
                    profile_writer[std::make_pair(start,start)]=createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString());
                    stmt_stack.pop_back();
                }
            }

            // CompoundStmt *newStmt=CompoundStmt::Create(*ctxt,newBody,ctxt->getSourceManager().getExpansionLoc(body->getLBracLoc()),ctxt->getSourceManager().getExpansionLoc(body->getRBracLoc()));
            // decl->setBody(newStmt);
            stmt_stack.pop_back();

            // CharSourceRange range=ctxt->getSourceManager().getExpansionRange(decl->getBody()->getBeginLoc());
            // size_t start=ctxt->getSourceManager().getFileOffset(range.getBegin());
            // size_t end=ctxt->getSourceManager().getFileOffset(range.getEnd());
            // size_t start=ctxt->getSourceManager().getFileOffset(ctxt->getSourceManager().getExpansionLoc(decl->getBody()->getBeginLoc()));
            // size_t end=ctxt->getSourceManager().getFileOffset(ctxt->getSourceManager().getExpansionLoc(decl->getBody()->getEndLoc()))+1;
        }
        return res;
    }
    bool TraverseIfStmt(IfStmt *stmt){
        bool res=RecursiveASTVisitor<AddProfileWriter>::TraverseIfStmt(stmt);
        if (ctxt->getSourceManager().getFilename(ctxt->getSourceManager().getExpansionLoc(stmt->getBeginLoc()))!=file) return res;
        // return res;

        ASTLocTy temp_loc=getNowLocation(stmt);
        LocalAnalyzer *temp_L = manager.getLocalAnalyzer(temp_loc);
        if (ctxt->getSourceManager().getFilename(temp_L->getCurrentFunction()->getLocation())!=file) return res;

        Stmt *tempBody=stmt->getThen();
        if (tempBody){
            if (ReturnStmt::classof(tempBody)){
                stmt_stack.push_back(tempBody);
                ASTLocTy loc=getNowLocation(tempBody);
                LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                ExprListTy exprs=L->getProfileWriterExpr();
                size_t start=ctxt->getSourceManager().getFileOffset(ctxt->getSourceManager().getExpansionLoc(tempBody->getBeginLoc()));
                size_t end=code.find(";",start)+1;
                profile_writer[std::make_pair(start,end)]="{\n"+createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString())+stmtToString(*ctxt,tempBody)+"}\n";
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
                            ExprListTy exprs=L->getProfileWriterExpr();
                            size_t start=ctxt->getSourceManager().getFileOffset(ctxt->getSourceManager().getExpansionLoc((*it)->getBeginLoc()));
                            size_t end=code.find(";",start)+1;
                            profile_writer[std::make_pair(start,end)]="{\n"+createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString())+stmtToString(*ctxt,*it)+"}\n";
                            stmt_stack.pop_back();
                        }
                    }
                    stmt_stack.pop_back();
                }
            }
        }

        Stmt *tempElse=stmt->getElse();
        if (tempElse){
            if (ReturnStmt::classof(tempElse)){
                stmt_stack.push_back(tempElse);
                ASTLocTy loc=getNowLocation(tempElse);
                LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                ExprListTy exprs=L->getProfileWriterExpr();
                size_t start=ctxt->getSourceManager().getFileOffset(ctxt->getSourceManager().getExpansionLoc(tempElse->getBeginLoc()));
                size_t end=code.find(";",start)+1;
                profile_writer[std::make_pair(start,end)]="{\n"+createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString())+stmtToString(*ctxt,tempBody)+"}\n";
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
                            ExprListTy exprs=L->getProfileWriterExpr();
                            size_t start=ctxt->getSourceManager().getFileOffset(ctxt->getSourceManager().getExpansionLoc((*it)->getBeginLoc()));
                            size_t end=code.find(";",start)+1;
                            profile_writer[std::make_pair(start,end)]="{\n"+createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString())+stmtToString(*ctxt,*it)+"}\n";
                            stmt_stack.pop_back();
                        }
                    }
                    stmt_stack.pop_back();
                }
            }
        }

        return res;
    }

    bool TraverseCaseStmt(CaseStmt *stmt){
        bool res=RecursiveASTVisitor<AddProfileWriter>::TraverseCaseStmt(stmt);
        if (ctxt->getSourceManager().getFilename(ctxt->getSourceManager().getExpansionLoc(stmt->getBeginLoc()))!=file) return res;

        stmt_stack.push_back(stmt);
        if (stmt->getSubStmt()!=NULL){
            if (ReturnStmt::classof(stmt->getSubStmt())){
                stmt_stack.push_back(stmt->getSubStmt());
                ASTLocTy loc=getNowLocation(stmt->getSubStmt());
                LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                ExprListTy exprs=L->getProfileWriterExpr();

                size_t start=ctxt->getSourceManager().getFileOffset(ctxt->getSourceManager().getExpansionLoc(stmt->getSubStmt()->getBeginLoc()));
                size_t end=code.find(";",start)+1;
                profile_writer[std::make_pair(start,end)]="{\n"+createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString())+stmtToString(*ctxt,stmt->getSubStmt())+"}\n";

                stmt_stack.pop_back();
            }
            else if (CompoundStmt::classof(stmt->getSubStmt())){
                CompoundStmt *body=llvm::dyn_cast<CompoundStmt>(stmt->getSubStmt());
                if (body){
                    stmt_stack.push_back(body);
                    for (CompoundStmt::body_iterator it=body->body_begin();it!=body->body_end();it++){
                        if (ReturnStmt::classof(*it)){
                            stmt_stack.push_back(*it);
                            ASTLocTy loc=getNowLocation(*it);
                            LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                            ExprListTy exprs=L->getProfileWriterExpr();
                            size_t start=ctxt->getSourceManager().getFileOffset(ctxt->getSourceManager().getExpansionLoc((*it)->getBeginLoc()));
                            size_t end=code.find(";",start)+1;
                            profile_writer[std::make_pair(start,end)]="{\n"+createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString())+stmtToString(*ctxt,*it)+"}\n";
                            stmt_stack.pop_back();
                        }
                    }
                    stmt_stack.pop_back();
                }
            }
        }
        stmt_stack.pop_back();

        return res;
    }

    bool TraverseDefaultStmt(DefaultStmt *stmt){
        bool res=RecursiveASTVisitor<AddProfileWriter>::TraverseDefaultStmt(stmt);
        if (ctxt->getSourceManager().getFilename(ctxt->getSourceManager().getExpansionLoc(stmt->getBeginLoc()))!=file) return res;

        stmt_stack.push_back(stmt);
        if (stmt->getSubStmt()!=NULL){
            if (ReturnStmt::classof(stmt->getSubStmt())){
                stmt_stack.push_back(stmt->getSubStmt());
                ASTLocTy loc=getNowLocation(stmt->getSubStmt());
                LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                ExprListTy exprs=L->getProfileWriterExpr();

                size_t start=ctxt->getSourceManager().getFileOffset(ctxt->getSourceManager().getExpansionLoc(stmt->getSubStmt()->getBeginLoc()));
                size_t end=code.find(";",start)+1;
                profile_writer[std::make_pair(start,end)]="{\n"+createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString())+stmtToString(*ctxt,stmt->getSubStmt())+"}\n";

                stmt_stack.pop_back();
            }
            else if (CompoundStmt::classof(stmt->getSubStmt())){
                CompoundStmt *body=llvm::dyn_cast<CompoundStmt>(stmt->getSubStmt());
                if (body){
                    stmt_stack.push_back(body);
                    for (CompoundStmt::body_iterator it=body->body_begin();it!=body->body_end();it++){
                        if (ReturnStmt::classof(*it)){
                            stmt_stack.push_back(*it);
                            ASTLocTy loc=getNowLocation(*it);
                            LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                            ExprListTy exprs=L->getProfileWriterExpr();
                            size_t start=ctxt->getSourceManager().getFileOffset(ctxt->getSourceManager().getExpansionLoc((*it)->getBeginLoc()));
                            size_t end=code.find(";",start)+1;
                            profile_writer[std::make_pair(start,end)]="{\n"+createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString())+stmtToString(*ctxt,*it)+"}\n";
                            stmt_stack.pop_back();
                        }
                    }
                    stmt_stack.pop_back();
                }
            }
        }
        stmt_stack.pop_back();

        return res;
    }
    bool TraverseSwitchStmt(SwitchStmt *stmt){
        bool res=RecursiveASTVisitor<AddProfileWriter>::TraverseSwitchStmt(stmt);
        if (ctxt->getSourceManager().getFilename(ctxt->getSourceManager().getExpansionLoc(stmt->getBeginLoc()))!=file) return res;
        // return res;

        ASTLocTy temp_loc=getNowLocation(stmt);
        LocalAnalyzer *temp_L = manager.getLocalAnalyzer(temp_loc);
        if (ctxt->getSourceManager().getFilename(temp_L->getCurrentFunction()->getLocation())!=file) return res;

        Stmt *tempBody=stmt->getBody();
        if (tempBody){
            stmt_stack.push_back(tempBody);
            CompoundStmt *body=llvm::dyn_cast<CompoundStmt>(tempBody);
            for (CompoundStmt::body_iterator it=body->body_begin();it!=body->body_end();it++){
                if (ReturnStmt::classof(*it)){
                    stmt_stack.push_back(*it);
                    ASTLocTy loc=getNowLocation(*it);
                    LocalAnalyzer *L = manager.getLocalAnalyzer(loc);
                    ExprListTy exprs=L->getProfileWriterExpr();

                    size_t start=ctxt->getSourceManager().getFileOffset(ctxt->getSourceManager().getExpansionLoc((*it)->getBeginLoc()));
                    size_t end=code.find(";",start)+1;
                    profile_writer[std::make_pair(start,end)]=createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString())+stmtToString(*ctxt,*it);
                    stmt_stack.pop_back();
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

std::map<std::string,std::vector<long long>> addProfileWriter(BenchProgram &P,std::map<std::string, std::string> &fileCode,std::vector<long long> &macros,std::map<long long,std::string> &macroCode,std::string output_file){
    SourceContextManager manager(P,false);
    std::map<std::string,std::vector<long long>> macroFile;
    macroFile.clear();
    for (std::map<std::string, std::string>::iterator it=fileCode.begin();it!=fileCode.end();it++){
        ASTContext *ctxt=manager.getSourceContext(it->first);
        it->second=manager.getSourceCode(it->first);
        SourceManager &m=ctxt->getSourceManager();

        AddProfileWriter writer(P,manager,ctxt,it->first,it->second);
        writer.TraverseTranslationUnitDecl(ctxt->getTranslationUnitDecl());
        
        std::map<std::pair<size_t,size_t>,std::string> processed=writer.getProcessedFuncBody();
        std::string backup=it->second;
        std::vector<std::string> codeSegs;
        std::vector<std::string> patches;
        codeSegs.push_back(it->second);
        size_t beforeLast=0;

        for (std::map<std::pair<size_t,size_t>,std::string>::iterator it2=processed.begin();it2!=processed.end();it2++){
            std::string lastSeg=codeSegs.back();
            codeSegs.pop_back();

            std::string first=lastSeg.substr(0,it2->first.first-beforeLast);
            codeSegs.push_back(first);
            
            patches.push_back(it2->second);
            codeSegs.push_back(backup.substr(it2->first.second));
            beforeLast=it2->first.second;
        }

        std::string finalCode="";
        for (size_t i=0;i<patches.size();i++){
            finalCode+=codeSegs[i];
            finalCode+=patches[i];
        }
        finalCode+=codeSegs.back();
        it->second=finalCode;

        macroFile[it->first]=writer.getMacroFile();
        std::map<long long,std::string> currentCode=writer.getMacroCode();
        for (std::map<long long,std::string>::iterator it2=currentCode.begin();it2!=currentCode.end();it2++)
            macroCode[it2->first]=it2->second;
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
    return macroFile;
}