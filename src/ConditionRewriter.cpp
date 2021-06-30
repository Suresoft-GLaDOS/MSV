#include "ConditionRewriter.h"
#include "SourceContextManager.h"
#include "ASTUtils.h"

namespace clang{

class ConditionRewriter{
private:
    std::map<std::pair<size_t,size_t>,std::vector<std::vector<Expr *>>> newExprs;
    std::string workDir;
    ASTContext *ctxt;
public:
    std::vector<Information> infos;
    ConditionRewriter(std::map<std::pair<size_t,size_t>,std::vector<std::vector<Expr *>>> newExprs,std::string workDir,ASTContext *ctxt):newExprs(newExprs),workDir(workDir),ctxt(ctxt) {}
    void rewrite(){
        std::string source="";
        std::ifstream file(workDir+"/_test_runtime.cpp");
        std::string temp;
        while (std::getline(file,temp)){
            source+=temp;
            source+="\n";
        }
        file.close();
        infos.clear();

        size_t start=source.find("__is_neg");
        start=source.find("{",start);
        size_t end=source.find_last_of('}');
        std::string front=source.substr(0,start+1);
        std::string last=source.substr(end);

        std::string code="std::string locStr(location);\n";
        code+="size_t split=locStr.find(\"-\");\n";
        code+="size_t switchNum=atoi(locStr.substr(0,split).c_str());\n";
        code+="size_t caseNum=atoi(locStr.substr(split+1).c_str());\n\n";

        for (std::map<std::pair<size_t,size_t>,std::vector<std::vector<Expr *>>>::iterator it=newExprs.begin();it!=newExprs.end();it++){
            code+="if(switchNum=="+std::to_string(it->first.first)+" && caseNum=="+std::to_string(it->first.second)+"){\n";
            code+="switch(__choose(std::string(\"__CONDITION_"+std::to_string(it->first.first)+"_"+std::to_string(it->first.second)+"\").c_str())){\n";
            size_t count=1;

            code+="case 0:\n";
            code+="return 1;\n";

            for (size_t i=0;i<it->second.size();i++){
                for (size_t j=0;j<it->second[i].size();j++){
                    code+="case "+std::to_string(count)+":{\n";
                    code+="va_list ap;\nva_start(ap, count);\n";
                    code+="void *p;unsigned long sz;long long v = 0;\n";
                    code+="for (size_t __i=0;__i<"+std::to_string(i)+"+1;__i++){\n";
                    code+="p = va_arg(ap, void*);\nsz = va_arg(ap, unsigned long);\n}\n";
                    code+="if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}\n";
                    code+="else return 0;\n";

                    std::string patch=stmtToString(*ctxt,it->second[i][j]);
                    size_t op=patch.find("==");
                    if (op==std::string::npos) op=patch.find("!=");
                    if (op==std::string::npos) op=patch.find(" >");
                    if (op==std::string::npos) op=patch.find(" <");
                    std::string keep=patch.substr(op);
                    code+="return (v"+keep+");\n}\n";

                    Information info;
                    info.currentSwitch=it->first.first;
                    info.currentCase=it->first.second;
                    info.isCondition=true;

                    Expr *currentCond=it->second[i][j];
                    BinaryOperator *bo=llvm::dyn_cast<BinaryOperator>(currentCond);
                    if (bo){
                        info.oper=bo->getOpcodeStr().str();
                        info.variable=stmtToString(*ctxt,bo->getLHS());
                        info.constant=stoi(stmtToString(*ctxt,bo->getRHS()));
                    }
                    infos.push_back(info);

                    count++;
                }
            }
            code+="}\n"; //fin switch
            code+="}\n"; // fin if
        }

        std::string finalCode=front+code+last;
        std::ofstream output(workDir+"/_test_runtime.cpp");
        output << finalCode << std::endl;
        output.close();
    }

    void build(std::string tools_dir){
        std::string cmd=tools_dir+"/runtime-build.py "+workDir+" "+tools_dir+"/../src";
        int result=system(cmd.c_str());
        assert(result==0);
    }
};

std::vector<Information> rewriteCondition(std::map<std::pair<size_t,size_t>,std::vector<std::vector<Expr *>>> newExprs,std::string workDir,std::string toolsDir,ASTContext *ctxt){
    ConditionRewriter cond(newExprs,workDir,ctxt);
    outlog_printf(2,"Generating condition patches...\n");
    cond.rewrite();
    cond.build(toolsDir);
    return cond.infos;
}
}