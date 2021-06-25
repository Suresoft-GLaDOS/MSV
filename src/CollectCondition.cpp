#include "CollectCondition.h"
#include <unistd.h>
#include <sys/wait.h>

namespace clang{

std::vector<unsigned long> CollectCondition::parseBranchRecord(std::string tmpFile) {
    std::vector<unsigned long> res;
    res.clear();
    FILE *f = fopen(tmpFile.c_str(), "r");
    //llvm::errs() << "New code\n" << new_code << "\n";
    if (!f) {
        outlog_printf(3, "Not caught by the negative case, give up!\n");
        //DEBUG(fprintf(stderr, "Not caught by the negative case, give up!\n"));
        //llvm::errs() << tmp_passed.size() << "\n";
        //assert(0);
        return res;
    }
    unsigned long n;
    int ret = fscanf(f, "%lu", &n);
    // FIXME: this is hacky!
    if (ret != 1) {
        fclose(f);
        return res;
    }
    for (size_t i = 0; i < n; i++) {
        unsigned long tmp;
        int ret = fscanf(f, "%lu", &tmp);
        // FIXME: something wierd, we get out!
        if (ret != 1) {
            fclose(f);
            printf("Error on reading records!\n");
            return std::vector<unsigned long>();
        }
        res.push_back(tmp);
    }
    fclose(f);
    return res;
}

void CollectCondition::writeBranchRecordTerminator(std::string tmpFile) {
    FILE*f = fopen(tmpFile.c_str(), "a");
    fprintf(f, " 0");
    fclose(f);
}

void CollectCondition::writeBranchRecord(std::string tmpFile,const std::map<unsigned long,std::vector<unsigned long>> &negative_records,unsigned long case_id) {
    FILE *f = fopen(tmpFile.c_str(), "w");
    std::map<unsigned long,std::vector<unsigned long>>::const_iterator fit = negative_records.find(case_id);
    assert( fit != negative_records.end() );
    // FIXME: We use only closest record, write all record!
    const std::vector<unsigned long> &tmp_vec = fit->second;
    fprintf(f, "%lu", (unsigned long)tmp_vec.size());
    for (size_t i = 0; i < tmp_vec.size(); i++)
        fprintf(f, " %lu", tmp_vec[i]);
    fprintf(f, "\n");
    fclose(f);
}
void CollectCondition::parseValueRecord(std::string recordFile,std::vector<std::vector<long long> > &vec) {
    vec.clear();
    FILE* f = fopen(recordFile.c_str(), "r");
    // Did not hit the condition, so it is an empty set
    if (f == NULL)
        return;
    bool over = false;
    while (!over) {
        std::vector<long long> v;
        v.clear();
        unsigned long n;
        int ret = fscanf(f, "%lu", &n);
        if (ret != 1) {
            over = true;
            break;
        }
        for (size_t i = 0; i < n; i++) {
            unsigned long tmp;
            ret = fscanf(f, "%lu", &tmp);
            assert(ret == 1);
            v.push_back(tmp);
        }
        vec.push_back(v);
    }
    fclose(f);
}
void CollectCondition::record(std::pair<size_t,size_t> currentPatch,int outPipe){
    std::string tmp_file="/tmp/"+std::to_string(getpid())+".tmp";

    // First going to make sure it passes all negative cases
    for (BenchProgram::TestCaseSetTy::iterator case_it = negative_cases.begin();
            case_it != negative_cases.end(); ++case_it) {
        // OK, we are going to try 10 times for different path combination before
        // we give up!
        size_t it_cnt = 0;
        BenchProgram::EnvMapTy testEnv = env;
        testEnv.insert(std::make_pair("IS_NEG", "1"));
        testEnv.insert(std::make_pair("NEG_ARG", "1"));
        testEnv.insert(std::make_pair("TMP_FILE", tmp_file));
        int ret = system((std::string("rm -rf ") + tmp_file).c_str());
        assert( ret == 0);
        bool passed = false;
        while (it_cnt < 10) {
            // llvm::errs() << "Testing iteration: " << it_cnt << "\n";
            passed = program.test(std::string("src"), *case_it, testEnv,switchNum,currentPatch.first,currentPatch.second, getpid(),false);
            std::vector<unsigned long> tmp_v = parseBranchRecord(tmp_file);
            writeBranchRecordTerminator(tmp_file);
            // We hit some strange error, we just assume we cannot pass this case
            if (tmp_v.size() == 0) passed = false;
            if (passed) {
                outlog_printf(2, "Passed in iteration: %u %u\n",currentPatch.first,currentPatch.second);
                write(outPipe,&(*case_it),sizeof(unsigned long));
                for (size_t i=0;i<tmp_v.size();i++)
                    write(outPipe,&tmp_v[i],sizeof(size_t));
                size_t fin=2;
                write(outPipe,&fin,sizeof(int));
                break;
            }
            bool has_zero = false;
            for (size_t j = 0; j < tmp_v.size(); j++)
                if (tmp_v[j] == 0) {
                    has_zero = true;
                    break;
                }
            if (!has_zero) break;
            it_cnt ++;
        }
        // We will going to try all 1 before we finally give up this case
        if (!passed){
            testEnv = env;
            testEnv.insert(std::make_pair("IS_NEG", "1"));
            testEnv.insert(std::make_pair("NEG_ARG", "0"));
            testEnv.insert(std::make_pair("TMP_FILE", tmp_file));
            ret = system((std::string("rm -rf ") + tmp_file).c_str());
            assert( ret == 0);
            passed = program.test(std::string("src"), *case_it, testEnv,switchNum,currentPatch.first,currentPatch.second, getpid(),false);
            if (passed) {
                std::vector<unsigned long> tmp_v = parseBranchRecord(tmp_file);
                // FIXME: strange error in wireshark, we just ignore right now
                if (tmp_v.size() == 0) {
                    outlog_printf(0, "Strange error or non-deterministic behavior!\n");
                    continue;
                }
                assert(tmp_v.size() != 0);
                write(outPipe,&(*case_it),sizeof(unsigned long));
                for (size_t i=0;i<tmp_v.size();i++)
                    write(outPipe,&tmp_v[i],sizeof(size_t));
                size_t fin=2;
                write(outPipe,&fin,sizeof(int));

                outlog_printf(2, "Passed with all 1: %u %u\n",currentPatch.first,currentPatch.second);
            }
            else {
                // Still failed, we are going to give up
                continue;
            }
        }
    }

    exit(0);
}

void CollectCondition::getConditionRecord(){
    size_t currentProcess=0;
    std::vector<std::pair<size_t,size_t>> currentSwitches;
    currentSwitches.clear();

    outlog_printf(2,"Recording bit vectors for condition synthesizing...\n");
    for (size_t i=0;i<conditionLocation.size();i++){
        int tmpPipe[2];
        assert(pipe(tmpPipe)==0 && "Pipeline open failed\n");

        std::pair<size_t,size_t> currentPatch(conditionLocation[i].first,conditionLocation[i].second);
        currentSwitches.push_back(currentPatch);

        pid_t id=fork();
        if (id<0){
            outlog_printf(2,"Error in fork\n");
            exit(1);
        }
        if (id==0){
            close(tmpPipe[0]);
            record(currentPatch,tmpPipe[1]);

            // Shouldn't reach here!
            exit(0);
        }

        close(tmpPipe[1]);
        currentProcess++;
        pids[currentPatch]=id;
        readPipes[currentPatch]=tmpPipe[0];

        if (currentProcess==totalProcess){
            currentProcess--;
            std::pair<size_t,size_t> waitPatch=currentSwitches[0];
            currentSwitches.erase(currentSwitches.begin());
            unsigned long temp_case;

            while (read(readPipes[waitPatch],&temp_case,sizeof(unsigned long))!=0){
                std::vector<size_t> currentRecords;
                currentRecords.clear();

                while(true){
                    size_t temp;
                    read(readPipes[waitPatch],&temp,sizeof(size_t));

                    if (temp==2){
                        break;
                    }
                    else{
                        currentRecords.push_back(temp);
                    }
                }

                records[waitPatch][temp_case]=currentRecords;
            }

            close(readPipes[waitPatch]);
            int status;
            waitpid(pids[waitPatch],&status,0);
        }
    }

    for (size_t i=0;i<currentSwitches.size();i++){
        unsigned long temp_case;

        while (read(readPipes[currentSwitches[i]],&temp_case,sizeof(unsigned long))!=0){
            std::vector<size_t> currentRecords;
            currentRecords.clear();

            while(true){
                size_t temp;
                read(readPipes[currentSwitches[i]],&temp,sizeof(size_t));

                if (temp==2){
                    break;
                }
                else{
                    currentRecords.push_back(temp);
                }
            }

            records[currentSwitches[i]][temp_case]=currentRecords;
        }

        close(readPipes[currentSwitches[i]]);
        int status;
        waitpid(pids[currentSwitches[i]],&status,0);
    }

    wait(NULL);
}

void CollectCondition::collect(std::pair<size_t,size_t> currentPatch,std::map<size_t,std::vector<size_t>> currentRecord,unsigned long currentCase,int outPipe,bool isNegative){
    std::string tmp_file="/tmp/"+std::to_string(getpid())+".tmp";
    std::string neg_file="/tmp/"+std::to_string(getpid())+".log";

    BenchProgram::EnvMapTy testEnv = env;
    if (isNegative){
        testEnv.insert(std::make_pair("IS_NEG", "RECORD1"));
        testEnv.insert(std::make_pair("NEG_ARG", tmp_file));
        testEnv.insert(std::make_pair("TMP_FILE", neg_file));

        writeBranchRecord(tmp_file,currentRecord,currentCase);
    }
    else{
        testEnv.insert(std::make_pair("IS_NEG", "RECORD0"));
        testEnv.insert(std::make_pair("TMP_FILE", neg_file));
    }

    std::string cmd = std::string("rm -rf ") + neg_file;
    int ret = system(cmd.c_str());
    assert( ret == 0);
    bool passed = program.test(std::string("src"), currentCase, testEnv, switchNum,currentPatch.first,currentPatch.second,getpid());

    std::vector<std::vector<long long>> currentMap;
    parseValueRecord(neg_file,currentMap);

    write(outPipe,&currentCase,sizeof(unsigned long));

    size_t resultSize=currentMap.size();
    write(outPipe,&resultSize,sizeof(size_t));
    for (size_t i=0;i<resultSize;i++){
        size_t size2=currentMap[i].size();
        write(outPipe,&size2,sizeof(size_t));

        for (size_t j=0;j<size2;j++){
            write(outPipe,&(currentMap[i][j]),sizeof(long long));
        }
    }

    exit(0);
}

bool CollectCondition::collectValues(std::map<std::pair<size_t,size_t>,std::map<unsigned long,std::vector<std::vector<long long>>>> &caseVMap) {
    caseVMap.clear();
    
    for (std::map<std::pair<size_t,size_t>,std::map<size_t,std::vector<size_t>>>::iterator it=records.begin();it!=records.end();it++){
        // outlog_printf(2,"%u %u\n",it->first.first,it->first.second);
        std::map<pid_t,int> pipes;
        std::vector<pid_t> pid;
        pid.clear();
        pipes.clear();
        std::map<unsigned long,std::vector<std::vector<long long>>> currentVMap;
        currentVMap.clear();
        std::map<size_t,std::vector<size_t>> currentRecord=it->second;
        size_t currentProcess=0;

        // We first deal with the negative cases
        for (BenchProgram::TestCaseSetTy::iterator tit = negative_cases.begin();
                tit != negative_cases.end(); ++tit) {
            int tmpPipe[2];
            assert(pipe(tmpPipe)==0 && "Pipeline open failed\n");
            currentProcess++;
            
            pid_t id=fork();
            if (id<0){
                outlog_printf(2,"Error in fork\n");
                exit(1);
            }
            if (id==0){
                close(tmpPipe[0]);
                collect(it->first,records[it->first],*tit,tmpPipe[1],true);
                exit(0);
            }

            close(tmpPipe[1]);
            pipes[id]=tmpPipe[0];
            pid.push_back(id);

            if (currentProcess==totalProcess){
                currentProcess--;
                pid_t waitPid=pid[0];
                pid.erase(pid.begin());

                unsigned long waitTest;

                read(pipes[waitPid],&waitTest,sizeof(unsigned long));
                std::vector<std::vector<long long>> currentCollects;
                currentCollects.clear();
                size_t currentSize;
                read(pipes[waitPid],&currentSize,sizeof(size_t));

                for(size_t i=0;i<currentSize;i++){
                    size_t size2;
                    read(pipes[waitPid],&size2,sizeof(size_t));
                    std::vector<long long> temp;
                    temp.clear();

                    for (size_t j=0;j<size2;j++){
                        long long temp2;
                        read(pipes[waitPid],&temp2,sizeof(long long));
                        temp.push_back(temp2);
                    }
                    currentCollects.push_back(temp);
                }

                caseVMap[it->first][waitTest]=currentCollects;
                close(pipes[waitPid]);
                int status;
                waitpid(waitPid,&status,0);
            }
        }
        // Then we deal with positive cases
        if (false)
        for (BenchProgram::TestCaseSetTy::iterator tit = positive_cases.begin();
                tit != positive_cases.end(); ++tit) {
            int tmpPipe[2];
            assert(pipe(tmpPipe)==0 && "Pipeline open failed\n");
            currentProcess++;
            
            pid_t id=fork();
            if (id<0){
                outlog_printf(2,"Error in fork\n");
                exit(1);
            }

            if (id==0){
                close(tmpPipe[0]);
                collect(it->first,records[it->first],*tit,tmpPipe[1],false);
                exit(0);
            }

            close(tmpPipe[1]);
            pipes[id]=tmpPipe[0];
            pid.push_back(id);

            if (currentProcess==totalProcess){
                currentProcess--;
                pid_t waitPid=pid[0];
                pid.erase(pid.begin());

                unsigned long waitTest;

                read(pipes[waitPid],&waitTest,sizeof(unsigned long));
                std::vector<std::vector<long long>> currentCollects;
                currentCollects.clear();
                size_t currentSize;
                read(pipes[waitPid],&currentSize,sizeof(size_t));

                for(size_t i=0;i<currentSize;i++){
                    size_t size2;
                    read(pipes[waitPid],&size2,sizeof(size_t));
                    std::vector<long long> temp;
                    temp.clear();

                    for (size_t j=0;j<size2;j++){
                        long long temp2;
                        read(pipes[waitPid],&temp2,sizeof(long long));
                        temp.push_back(temp2);
                    }
                    currentCollects.push_back(temp);
                }

                caseVMap[it->first][waitTest]=currentCollects;

                close(pipes[waitPid]);
                int status;
                waitpid(waitPid,&status,0);
            }
        }

        for (size_t i=0;i<pid.size();i++){
            pid_t waitPid=pid[i];
            unsigned long waitTest;

            read(pipes[waitPid],&waitTest,sizeof(unsigned long));
            std::vector<std::vector<long long>> currentCollects;
            currentCollects.clear();
            size_t currentSize;
            read(pipes[waitPid],&currentSize,sizeof(size_t));

            for(size_t i=0;i<currentSize;i++){
                size_t size2;
                read(pipes[waitPid],&size2,sizeof(size_t));
                std::vector<long long> temp;
                temp.clear();

                for (size_t j=0;j<size2;j++){
                    long long temp2;
                    read(pipes[waitPid],&temp2,sizeof(long long));
                    temp.push_back(temp2);
                }
                currentCollects.push_back(temp);
            }

            caseVMap[it->first][waitTest]=currentCollects;
            close(pipes[waitPid]);
            int status;
            waitpid(waitPid,&status,0);
        }
    }
    return true;
}

} //fin namespace clang