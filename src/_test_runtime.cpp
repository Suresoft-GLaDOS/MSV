// Copyright (C) 2016 Fan Long, Martin Rianrd and MIT CSAIL 
// Prophet
// 
// This file is part of Prophet.
// 
// Prophet is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Prophet is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Prophet.  If not, see <http://www.gnu.org/licenses/>.
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <assert.h>
#include <stdarg.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <time.h>
#include <cstdint>

#include <limits.h>
#include <dirent.h>
#include <stdbool.h>

#define MAXSZ 1048576
#define ONE_OR_N_BIT 50

static bool init = false;
static bool enable = false;
static bool mutant_init = false;
static int mutant_id = 0;
static unsigned long current_cnt = 0;
static unsigned long records[MAXSZ];
static unsigned long records_sz;

static void __attribute((constructor)) _init() {
    memset(records, 0, sizeof(records));
    records_sz = 0;
    char* tmp = getenv("IS_NEG");
    if (!tmp) return;
    std::string is_neg = tmp;
    if (is_neg == "") return;
    pid_t pid = getpid();
    std::ostringstream sout;
    sout << "/proc/" << pid << "/cmdline";
    FILE *f = fopen(sout.str().c_str(), "rb");
    assert(f && "Cannot get the cmdline str");

    std::vector<std::string> res;
    res.clear();
    char c;
    std::string now_str = "";
    int read_cnt = fread(&c, 1, 1, f);
    while (read_cnt == 1 && !feof(f)) {
        if (c == '\0' || c == '\n') {
            res.push_back(now_str);
            now_str = "";
        }
        else
            now_str.push_back(c);
        read_cnt = fread(&c, 1, 1, f);
    }
    fclose(f);

    // FIXME: very hacky for php, php invokes multiple times
    enable = true;
    for (unsigned long i = 0; i < res.size(); i++) {
        if (res[i].find("run-test") != std::string::npos) {
            enable = false;
            break;
        }
        if (res[i].find("echo PHP_VERSION") != std::string::npos) {
            enable = false;
            break;
        }
    }
}

bool isGoodAddr(void *pointer, size_t size) {
    int nullfd = open("/dev/random", O_WRONLY);

    if (write(nullfd, pointer, size) < 0)
    {
        close(nullfd);
        return false;
    }
    close(nullfd);

    return true;
}

extern "C" int __get_mutant() {
    if (!mutant_init) {
        char* tmp = getenv("MUTANT_ID");
        sscanf(tmp, "%d", &mutant_id);
        mutant_init = true;
    }
    //fprintf(stderr, "running mutant: %d\n", mutant_id);
    return mutant_id;
}

extern "C" int __choose(const char *switch_id) {
    // fprintf(stderr,"id: %d\n",id);
    char *env=getenv(switch_id);
    if (env==NULL) return 0;
    int result=atoi(env);
    return result;
}

#define MAGIC_NUMBER -123456789

extern "C" char *__stat_write_init(const char *func_name){
    char * pid = getenv("__PID");
    if (pid == NULL || strlen(pid) == 0) {
        return NULL;
    }

    char *tmp_dir = getenv("MSV_TMP_DIR");
    if (tmp_dir == NULL || strlen(tmp_dir) == 0) {
        tmp_dir = "/tmp";
    }

    char log_file[1024];
    sprintf(log_file,"%s/%s_profile.log", tmp_dir, pid);
    // fprintf(stderr, "good pid\n");
    
    int included=0;
    FILE *log_r=fopen(log_file,"r");
    if (log_r==NULL)
        log_r=fopen(log_file,"w");
    else{
        char *line=NULL;
        size_t length=0;
        // fprintf(stderr, "before getline\n");
        while (getline(&line,&length,log_r)!=-1){
            line[strlen(line)-1]='\0';
            if (strcmp(line,func_name)==0){
                included=1;
                break;
            }
        }
        // fprintf(stderr, "after getline\n");
        free(line);
    }
    fclose(log_r);

    if (!included){
        FILE *log=fopen(log_file,"a");
        fprintf(log,"%s\n",func_name);
        fclose(log);
    }
    if (getenv("MSV_RUN_ORIGINAL")!=NULL){
        return NULL;
    }

    char *str=(char *)malloc(sizeof(char)*1000000);
    sprintf(str,"");
    return str;
}
extern "C" void __write_stat(char *str,const char *var_name,void *var_addr,int size){
    char * pid = getenv("__PID");
    if (pid == NULL || strlen(pid) == 0 || getenv("MSV_RUN_ORIGINAL") != NULL) {
        return;
    }

    if (str==NULL) return;
    long long v = 0;
    if (size<=8 && isGoodAddr(var_addr, size)) {
        memcpy(&v, var_addr, size);
    }
    else {
        v = MAGIC_NUMBER;
    }
    sprintf(str, "%s%s=%lld\n", str,var_name,v);
    // fprintf(stderr, "%s=%lld\n", name, v);
    // fprintf(stderr, "exit\n");
}

extern "C" void __stat_file_close(const char *func_name,char *str){
    char * pid = getenv("__PID");
    if (pid == NULL || strlen(pid) == 0 || getenv("MSV_RUN_ORIGINAL") != NULL) {
        return;
    }

    if (str==NULL) return;
    char *tmp_dir = getenv("MSV_TMP_DIR");
    if (tmp_dir == NULL || strlen(tmp_dir) == 0) {
        tmp_dir = "/tmp";
    }

    char tmp_file[1024];
    sprintf(tmp_file, "%s/%s_%s_profile.log", tmp_dir, pid, func_name);

    FILE *f;
    f = fopen(tmp_file, "w");
    fprintf(f,str);
    fclose(f);
    free(str);
}

extern "C" int __is_neg(const char *location,char *lid,int count, ...) {
    // fprintf(stderr, "fuck\n");
    if (!enable) return 0;
    char* is_neg = getenv("IS_NEG");
    if (!is_neg) return 0;
    if (strcmp(is_neg, "1") == 0) {
        char* tmp = getenv("NEG_ARG");
        if (tmp && (strcmp(tmp, "1") == 0)) {
            char* tmp_file = getenv("TMP_FILE");
            assert(tmp_file);
            // First time here, we need to read a tmp file to know
            // where we are
            if (!init) {
                // fprintf(stderr,"Initing 1!\n");
                init = true;
                FILE *f = fopen(tmp_file, "r");
                if (f == NULL) {
                    records_sz = 0;
                    current_cnt = 0;
                }
                else {
                    unsigned long n;
                    int ret = fscanf(f, "%lu", &n);
                    assert( ret == 1);
                    records_sz = 0;
                    for (unsigned long i = 0; i < n; i++) {
                        unsigned long v;
                        ret = fscanf(f, "%lu", &v);
                        assert( ret == 1);
                        records[records_sz ++ ] = v;
                    }
                    unsigned long tmp;
                    ret = fscanf(f, "%lu", &tmp);
                    fclose(f);
                    if (ret != 0) {
                        assert( tmp == 0);
                        assert( records_sz > 0);
                        long long i = records_sz - 1;
                        while (i >= 0) {
                            if (records[i] == 0)
                                break;
                            else
                                i--;
                        }
                        assert( i >= 0);
                        records[i] = 1;
                        records_sz = i + 1;
                        current_cnt = 0;
                    } else{
                        current_cnt = records_sz;
                    }
                }
            }

            int ret = 0;
            if (current_cnt < records_sz){
                ret = (int) records[current_cnt];
            }
            else {
                if (records_sz < MAXSZ){
                    records[records_sz++] = 0;
                }
            }
            current_cnt ++;

            // fprintf(stderr,"Current cnt, Record size: %d %d\n",current_cnt,records_sz);
            // We write back immediate
            FILE *f = fopen(tmp_file, "w");
            assert( f != NULL );
            fprintf(f, "%lu ", records_sz);
            // fprintf(stderr, "Size: %lu\n",records_sz);
            // fprintf(stderr, "Record: ");
            for (unsigned long i = 0; i < records_sz; i++) {
                fprintf(f, "%lu", records[i]);
                // fprintf(stderr, "%lu ",records[i]);
                if (i != records_sz - 1)
                    fprintf(f, " ");
            }
            // fprintf(stderr, "\n");
            fclose(f);

            return ret;
        }
        // we always return 1
        else {
            // fprintf(stderr,"Initing 0!\n");
            // First time here, we need to read a tmp file to know
            // where we are
            if (!init) {
                init = true;
                char* tmp_file = getenv("TMP_FILE");
                assert(tmp_file);
                FILE* f = fopen(tmp_file, "r");
                if (f != NULL) {
                    unsigned long n, tmp;
                    int ret = fscanf(f, "%lu", &n);
                    assert(ret == 1);
                    records_sz = 0;
                    for (unsigned long i = 0; i < n; i++) {
                        ret = fscanf(f, "%lu", &records[records_sz ++]);
                        assert( ret != 0);
                    }
                    ret = fscanf(f, "%lu", &tmp);
                    fclose(f);
                    if (ret == 0) {
                        records_sz = 0;
                        current_cnt = 0;
                    }
                    else {
                        assert(tmp == 0);
                        current_cnt = records_sz;
                    }
                }
                else {
                    records_sz = 0;
                    current_cnt = 0;
                }
            }
            records[records_sz ++] = 1;
            current_cnt ++;
            // fprintf(stderr,"Current cnt, Record size: %d %d\n",current_cnt,records_sz);

            char* tmp_file = getenv("TMP_FILE");
            assert(tmp_file);
            // We write back immediate
            FILE *f = fopen(tmp_file, "w");
            assert( f != NULL );
            fprintf(f, "%lu ", records_sz);
            // fprintf(stderr, "Size: %lu\n",records_sz);
            // fprintf(stderr, "Record: ");
            for (unsigned long i = 0; i < records_sz; i++) {
                fprintf(f, "%lu", records[i]);
                // fprintf(stderr, "%lu ",records[i]);
                if (i != records_sz - 1)
                    fprintf(f, " ");
            }
            fclose(f);
            // fprintf(stderr, "\n");
            return 1;
        }
    }
    else if (strcmp(is_neg, "RUN") == 0){
        char temp[20];
        strcpy(temp,location);
        char temp_location[40]="__";
        char *temp2;
        temp2=strtok(temp,"-");
        strcat(temp_location,temp2);
        strcat(temp_location,"_");
        strcat(temp_location,strtok(NULL,"-"));

        // If operator is ALL_1, return 1
        char operator_temp[60];
        strcpy(operator_temp,temp_location);
        strcat(operator_temp,"__OPERATOR");
        char variable_temp[60];
        strcpy(variable_temp,temp_location);
        strcat(variable_temp,"__VARIABLE");
        char constant_temp[60];
        strcpy(constant_temp,temp_location);
        strcat(constant_temp,"__CONSTANT");

        if (strcmp(getenv(operator_temp),"4")==0) {
            return 1;
        }

        int var=atoi(getenv(variable_temp));
        int oper=atoi(getenv(operator_temp));
        long long constant=(long long)atoi(getenv(constant_temp));
        long long value=0;
        long long value2=0;

        va_list ap;
        va_start(ap, count);
        for (unsigned long i = 0; i < (unsigned long)count; i++) {
            void* p = va_arg(ap, void*);
            unsigned long sz = va_arg(ap, unsigned long);
            assert( sz <= 8 );

            if (i==var){
                // We assume that all variables are signed
                // TODO: handle unsigned variables
                if (sz==8){
                    int64_t v = 0;
                    if (isGoodAddr(p, sz)) {
                        memcpy(&v, p, sz);
                    }
                    else {
                        v = MAGIC_NUMBER;
                    }
                    value=(long long) v;
                }
                else if (sz==4){
                    int32_t v = 0;
                    if (isGoodAddr(p, sz)) {
                        memcpy(&v, p, sz);
                    }
                    else {
                        v = MAGIC_NUMBER;
                    }
                    value=(long long) v;
                }
                else if (sz==2){
                    int16_t v = 0;
                    if (isGoodAddr(p, sz)) {
                        memcpy(&v, p, sz);
                    }
                    else {
                        v = MAGIC_NUMBER;
                    }
                    value=(long long) v;
                }
                else if (sz==1){
                    int8_t v = 0;
                    if (isGoodAddr(p, sz)) {
                        memcpy(&v, p, sz);
                    }
                    else {
                        v = MAGIC_NUMBER;
                    }
                    value=(long long) v;
                }

                if (oper<=4)
                    break;
            }

            // else if (oper>=5 && i==constant){
            //     // We assume that all variables are signed
            //     // TODO: handle unsigned variables
            //     if (sz==8){
            //         int64_t v = 0;
            //         if (isGoodAddr(p, sz)) {
            //             memcpy(&v, p, sz);
            //         }
            //         else {
            //             v = MAGIC_NUMBER;
            //         }
            //         value2=(long long) v;
            //     }
            //     else if (sz==4){
            //         int32_t v = 0;
            //         if (isGoodAddr(p, sz)) {
            //             memcpy(&v, p, sz);
            //         }
            //         else {
            //             v = MAGIC_NUMBER;
            //         }
            //         value2=(long long) v;
            //     }
            //     else if (sz==2){
            //         int16_t v = 0;
            //         if (isGoodAddr(p, sz)) {
            //             memcpy(&v, p, sz);
            //         }
            //         else {
            //             v = MAGIC_NUMBER;
            //         }
            //         value2=(long long) v;
            //     }
            //     else if (sz==1){
            //         int8_t v = 0;
            //         if (isGoodAddr(p, sz)) {
            //             memcpy(&v, p, sz);
            //         }
            //         else {
            //             v = MAGIC_NUMBER;
            //         }
            //         value2=(long long) v;
            //     }
            // }
        }

        int result;
        if (value==MAGIC_NUMBER) return 0;
        else{
            switch(oper){
                case 1: 
                    result = (value !=constant);
                    break;
                case 2: 
                    result = (value >constant);
                    break;
                case 3: 
                    result = (value <constant);
                    break;
                // case 5:
                //     result = (value ==value2);
                //     break;
                // case 6:
                //     result=(value!=value2);
                //     break;
                // case 7:
                //     result=(value>value2);
                //     break;
                // case 8:
                //     result=(value<value2);
                //     break;
                default: 
                    result = (value ==constant);
                    break;
            }

            return result;
        }
    }
    return 0;
}

extern "C" long long __mutate(const long long value,const char *oper_env,const char *const_env){
    int oper=__choose(oper_env);
    int constant=__choose(const_env);
    switch(oper){
        case 0: return constant; // assign
        case 1: return value + constant; // add
        case 2: return value - constant; // sub
        case 3: return value * constant; // mult
        case 4: return value / constant; // div
        default: return constant; // assign (default)
    }
}

extern "C" void *__var_select(unsigned int var_count,void *vars[]){
    char *var=getenv("__SELECT_VAR");
    if (var==NULL) return vars[0];
    else{
        int index=atoi(var);
        return vars[index];
    }
}

extern "C" long long __const_select(unsigned int const_count, ...){
    char *var=getenv("__SELECT_VAR");
    va_list ap;
    va_start(ap,const_count);

    if (var==NULL) return 0;
    else{
        int index=atoi(var);
        long long result=0;
        for (int i=0;i<index;i++){
            va_arg(ap,long long);
        }
        result=va_arg(ap,long long);
        return result;
    }
}

extern "C" void *__var_select_2(unsigned int var_count,void *vars[]){
    char *var=getenv("__SELECT_VAR_2");
    if (var==NULL) return vars[0];
    else{
        int index=atoi(var);
        return vars[index];
    }
}