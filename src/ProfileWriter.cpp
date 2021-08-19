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
    ASTContext &ctxt;
    std::string file;
    ImplicitCastExpr *writer;

    std::vector<Stmt*> stmt_stack;

    ASTLocTy getNowLocation(Stmt *n) {
        assert( stmt_stack.size() > 1 );
        Stmt *parent_stmt = stmt_stack[stmt_stack.size() - 2];
        SourceManager &M = ctxt.getSourceManager();
        SourceLocation exp_loc = M.getExpansionLoc(n->getBeginLoc());
        std::string src_file = M.getFilename(exp_loc).str();
        return ASTLocTy(file, src_file, parent_stmt, n);
    }

    Expr* createProfileWriter(SourceContextManager &M, ASTContext &ctxt,const ExprListTy &exprs,const std::string funcName) {
        std::vector<Expr*> tmp_argv;
        tmp_argv.clear();
        StringLiteral *str=StringLiteral::Create(ctxt,llvm::StringRef(funcName),StringLiteral::StringKind::Ascii,false,ctxt.getConstantArrayType(ctxt.CharTy, llvm::APInt(32, 0),nullptr,ArrayType::Normal,0),SourceLocation());
        tmp_argv.push_back(str);
        tmp_argv.push_back(getNewIntegerLiteral(&ctxt, exprs.size()));
        for (size_t i = 0; i < exprs.size(); ++i) {
            StringLiteral *exprStr=StringLiteral::Create(ctxt,llvm::StringRef(stmtToString(ctxt,exprs[i])),StringLiteral::StringKind::Ascii,false,ctxt.getConstantArrayType(ctxt.CharTy, llvm::APInt(32, 0),nullptr,ArrayType::Normal,0),SourceLocation());
            tmp_argv.push_back(exprStr);

            ParenExpr *ParenE1 = new (ctxt) ParenExpr(SourceLocation(), SourceLocation(), exprs[i]);
            UnaryOperator *AddrsOf = 
                UnaryOperator::Create(ctxt,ParenE1, UO_AddrOf, ctxt.getPointerType(exprs[i]->getType()),
                        VK_RValue, OK_Ordinary, SourceLocation(),false,FPOptionsOverride());
            tmp_argv.push_back(AddrsOf);

            ParenExpr *ParenE2 = new (ctxt) ParenExpr(SourceLocation(), SourceLocation(), exprs[i]);
            UnaryExprOrTypeTraitExpr *SizeofE = new (ctxt) UnaryExprOrTypeTraitExpr(
                UETT_SizeOf, ParenE2, ctxt.UnsignedLongTy, SourceLocation(), SourceLocation());
            tmp_argv.push_back(SizeofE);
        }
        CallExpr *CE = CallExpr::Create(ctxt, writer, tmp_argv,
                ctxt.IntTy, VK_RValue, SourceLocation());
        return CE;
    }

public:
    AddProfileWriter(BenchProgram &p,SourceContextManager &m,std::unique_ptr<ASTUnit> &unit): program(p),manager(m),ctxt(unit->getASTContext()),file(unit->getOriginalSourceFileName().str()) {}
    bool TraverseFunctionDecl(FunctionDecl *decl){
        bool res=RecursiveASTVisitor<AddProfileWriter>::TraverseFunctionDecl(decl);
        if (decl->getNameAsString()=="__write_profile"){
            DeclRefExpr *FRef = DeclRefExpr::Create(ctxt, NestedNameSpecifierLoc(), SourceLocation(),
                    decl, false, SourceLocation(), decl->getType(), VK_RValue);
            writer= ImplicitCastExpr::Create(ctxt, ctxt.getDecayedType(decl->getType()), CK_FunctionToPointerDecay,
                    FRef, 0, VK_RValue);
            return res;
        }

        if (!decl->hasBody() || ctxt.getSourceManager().getFilename(decl->getLocation())!=file) return res;
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
                    LocalAnalyzer *L = manager.getLocalAnalyzer(loc,&ctxt);
                    ExprListTy exprs=L->genExprAtoms(QualType(),true,false,true,false,false);
                    newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                    stmt_stack.pop_back();
                }
                newBody.push_back(*it);
            }
            if (decl->getReturnType()==ctxt.VoidTy && !ReturnStmt::classof(body->body_back())){
                stmt_stack.push_back(body->body_back());
                ASTLocTy loc=getNowLocation(body->body_back());
                LocalAnalyzer *L = manager.getLocalAnalyzer(loc,&ctxt);
                ExprListTy exprs=L->genExprAtoms(QualType(),true,false,true,false,false);
                newBody.push_back(createProfileWriter(manager,ctxt,exprs,L->getCurrentFunction()->getNameAsString()));
                stmt_stack.pop_back();
            }

            CompoundStmt *newStmt=CompoundStmt::Create(ctxt,newBody,ctxt.getSourceManager().getExpansionLoc(body->getLBracLoc()),ctxt.getSourceManager().getExpansionLoc(body->getRBracLoc()));
            decl->setBody(newStmt);
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
        std::unique_ptr<ASTUnit> unit=P.buildClangASTUnit(it->first,it->second,macros);
        AddProfileWriter writer(P,manager,unit);
        writer.TraverseTranslationUnitDecl(unit->getASTContext().getTranslationUnitDecl());
        std::error_code temp;
        llvm::raw_fd_ostream fout(P.getSrcdir()+"/"+it->first,temp);
        // printf(temp.message().c_str());
        unit->getASTContext().getTranslationUnitDecl()->print(fout);
        fout.close();

        size_t filePos=it->first.rfind("/");
        std::string bak_file=P.getWorkdir()+"/"+output_file+it->first.substr(filePos+1);
        llvm::raw_fd_ostream bak_out(bak_file,temp);
        unit->getASTContext().getTranslationUnitDecl()->print(bak_out);
        bak_out.close();
        outlog_printf(2,"Saved fixed file at: %s\n",bak_file.c_str());
    }
    return true;
}