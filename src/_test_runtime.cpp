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
#include <klee/klee.h>
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

int table_miss = 1;

struct entry_s {
    char *key;
    int value;
    struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
    int size;
    struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;


/* Create a new hashtable. */
hashtable_t *ht_create( int size ) {

    hashtable_t *hashtable = NULL;
    int i;

    if( size < 1 ) return NULL;

    /* Allocate the table itself. */
    if( ( hashtable = (hashtable_t *)malloc( sizeof( hashtable_t ) ) ) == NULL ) {
        return NULL;
    }

    /* Allocate pointers to the head nodes. */
    if( ( hashtable->table = (entry_t **)malloc( sizeof( entry_t * ) * size ) ) == NULL ) {
        return NULL;
    }
    for( i = 0; i < size; i++ ) {
        hashtable->table[i] = NULL;
    }

    hashtable->size = size;

    return hashtable;
}

/* Hash a string for a particular hash table. */
int ht_hash( hashtable_t *hashtable, char *key ) {

    unsigned long int hashval;
    int i = 0;

    /* Convert our string to an integer */
    while( hashval < ULONG_MAX && i < strlen( key ) ) {
        hashval = hashval << 8;
        hashval += key[ i ];
        i++;
    }

    return hashval % hashtable->size;
}

/* Create a key-value pair. */
entry_t *ht_newpair( char *key, int value ) {
    entry_t *newpair;

    if( ( newpair = (entry_t *)malloc( sizeof( entry_t ) ) ) == NULL ) {
        return NULL;
    }

    if( ( newpair->key = strdup( key ) ) == NULL ) {
        return NULL;
    }

    newpair->value = value;

    newpair->next = NULL;

    return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set( hashtable_t *hashtable, char *key, int value) {
    int bin = 0;
    entry_t *newpair = NULL;
    entry_t *next = NULL;
    entry_t *last = NULL;

    bin = ht_hash( hashtable, key );

    next = hashtable->table[ bin ];

    while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
        last = next;
        next = next->next;
    }

    /* There's already a pair.  Let's replace that string. */
    if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {

        next->value = value;

        /* Nope, could't find it.  Time to grow a pair. */
    } else {
        newpair = ht_newpair( key, value );

        /* We're at the start of the linked list in this bin. */
        if( next == hashtable->table[ bin ] ) {
            newpair->next = next;
            hashtable->table[ bin ] = newpair;

            /* We're at the end of the linked list in this bin. */
        } else if ( next == NULL ) {
            last->next = newpair;

            /* We're in the middle of the list. */
        } else  {
            newpair->next = next;
            last->next = newpair;
        }
    }
}

/* Retrieve a key-value pair from a hash table. */
int ht_get( hashtable_t *hashtable, char *key ) {
    int bin = 0;
    entry_t *pair;

    bin = ht_hash( hashtable, key );

    /* Step through the bin, looking for our value. */
    pair = hashtable->table[ bin ];
    while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
        pair = pair->next;
    }

    /* Did we actually find anything? */
    if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
        table_miss = 1;
        return 0;

    } else {
        table_miss = 0;
        return pair->value;
    }

}

/*
  End of hashtable implementation
 */

#define MAX_NAME_LENGTH 1000

hashtable_t *choice_instances;
hashtable_t *output_instances;
hashtable_t *selector_instances;

void init_tables() {
    choice_instances = ht_create(65536);
    output_instances = ht_create(65536);
    selector_instances = ht_create(65536);
}

extern "C" int __trident_choice(char* lid, char* typestr,
                     int* rvals, char** rvals_ids, int rvals_size,
                     int** lvals, char** lvals_ids, int lvals_size){
    if (!choice_instances)
        init_tables();
    int previous = ht_get(choice_instances, lid);
    int instance;
    if (table_miss) {
        instance = 0;
    } else {
        instance = previous + 1;
    }
    for (int i=0; i<rvals_size; i++) {
        char name[MAX_NAME_LENGTH];
        sprintf(name, "choice!rvalue!%s!%d!%s", lid, instance, rvals_ids[i]);
        int klee_var;
        klee_make_symbolic(&klee_var, sizeof(klee_var), name);
//        klee_print_expr(rvals_ids[i], rvals[i]);
        char print_text[MAX_NAME_LENGTH];
        sprintf(print_text, "[program-var] %s", rvals_ids[i]);
        klee_print_expr(print_text, rvals[i]);
        klee_assume(klee_var == rvals[i]);
        sprintf(print_text, "[angelic-var] %s", name);
        klee_print_expr(print_text, klee_var);
    }

    bool condition = true;
    char selector_name[MAX_NAME_LENGTH];
    sprintf(selector_name, "choice!lvalue!selector!%s", lid);

    int selector = ht_get(selector_instances, lid);
    if (table_miss) {
        klee_make_symbolic(&selector, sizeof(selector), selector_name);
        ht_set(selector_instances, lid, selector);
    }
    for (int i = 0; i < lvals_size; i++) {
        char next_name[MAX_NAME_LENGTH];
        sprintf(next_name, "choice!lvalue!%s!%d!%s", lid, instance, lvals_ids[i]);
        int prev_lvalue = *lvals[i];
        int next_klee_var;
        klee_make_symbolic(&next_klee_var, sizeof(next_klee_var), next_name);
        *lvals[i] = next_klee_var;
        condition = condition & ((prev_lvalue == next_klee_var) | (selector == i));
    }
    klee_assume(condition);
    char name[MAX_NAME_LENGTH];
    sprintf(name, "choice!angelic!%s!%s!%d", typestr, lid, instance);
    int angelic;
    klee_make_symbolic(&angelic, sizeof(angelic), name);
    return angelic;
}


extern "C" int __trident_output(char* id, char* typestr, int value){
    if (getenv("IS_NEG")==NULL || strcmp(getenv("IS_NEG"),"SE")!=0)
        return value;
    if (!output_instances)
        init_tables();
    int previous = ht_get(output_instances, id);
    int instance;
    if (table_miss) {
      instance = 0;
    } else {
      instance = previous + 1;
    }
    ht_set(output_instances, id, instance);
    char name[MAX_NAME_LENGTH];
    sprintf(name, "output!%s!%s!%d", typestr, id, instance);
    int klee_var;
    klee_make_symbolic(&klee_var, sizeof(klee_var), name);
    klee_assume(klee_var == value);
    klee_print_expr(name, klee_var);
    return value;
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
    else if (strcmp(is_neg, "0") == 0)
        return 0;
    else if ((strcmp(is_neg, "RECORD1") == 0) || (strcmp(is_neg, "RECORD0") == 0)) {
        // We print out everything into the TMP_FILE
        char* tmp_file = getenv("TMP_FILE");
        assert(tmp_file);
        if (init == false) {
            init = true;
            FILE *f;
            records_sz = 0;
            //fprintf(stderr, "here0\n");
            if (strcmp(is_neg, "RECORD1") == 0) {
                char* neg_arg = getenv("NEG_ARG");
                f = fopen(neg_arg, "r");
                assert(f != NULL);
                unsigned long n;

                int ret = fscanf(f, "%lu", &n);
                assert( ret == 1);
                for (unsigned long i = 0; i < n; i++) {
                    unsigned long tmp;
                    ret = fscanf(f, "%lu", &tmp);
                    assert( ret == 1);
                    records[records_sz ++] = tmp;
                }
                ret = fscanf(f, "%lu", &current_cnt);
                fclose(f);
                if (ret == 0) {
                    current_cnt = 0;
                }
            }
        }
        
        // fprintf(stderr, "here1\n");
        va_list ap;
        va_start(ap, count);
        FILE *f = fopen(tmp_file, "a");
        fprintf(f, "%lu", (unsigned long)count);
        // fprintf(stderr, "count %d cnt %lu\n", count, current_cnt);
        for (unsigned long i = 0; i < (unsigned long)count; i++) {
            void* p = va_arg(ap, void*);
            unsigned long sz = va_arg(ap, unsigned long);
            assert( sz <= 8 );
            long long v = 0;
            if (isGoodAddr(p, sz)) {
                memcpy(&v, p, sz);
            }
            else {
                v = MAGIC_NUMBER;
            }
            fprintf(f, " %lld", v);
            // fprintf(stderr, "i %lu %lld\n", i, v);
        }
        fprintf(f, "\n");
        fclose(f);

        if (strcmp(is_neg, "RECORD1") == 0) {
            if (current_cnt>=records_sz){
                records[current_cnt]=0;
                records_sz++;
            }
            // We write back with additional int to note the end
            char* neg_arg = getenv("NEG_ARG");
            f = fopen(neg_arg, "w");
            assert( f != NULL );
            fprintf(f, "%lu ", records_sz);
            // fprintf(stderr, "size: %d\n",records_sz);
            for (unsigned long i = 0; i < records_sz; i++) {
                fprintf(f, "%lu", records[i]);
                // fprintf(stderr, "record: %d\n",records[i]);
                if (i != records_sz - 1)
                    fprintf(f, " ");
            }
            // fprintf(f, "%lu", current_cnt + 1);
            // fprintf(stderr, "count: %d\n",current_cnt+1);
            fclose(f);

            // assert( current_cnt < records_sz);
            //fprintf(stderr, "fuck you %lu\n", records[current_cnt]);
            return records[current_cnt++];
        }
        else
            return 0;
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

        char* tmp_file = getenv("TMP_FILE");
        if (tmp_file==NULL || strcmp(tmp_file,"")==0){
            if (strcmp(getenv(operator_temp),"4")==0) {
                return 1;
            }

            int var=atoi(getenv(variable_temp));
            int oper=atoi(getenv(operator_temp));
            int constant=atoi(getenv(constant_temp));
            long long value=0;

            va_list ap;
            va_start(ap, count);
            for (unsigned long i = 0; i < (unsigned long)count; i++) {
                void* p = va_arg(ap, void*);
                unsigned long sz = va_arg(ap, unsigned long);
                assert( sz <= 8 );

                if (i==var){
                    if (isGoodAddr(p, sz)) {
                        memcpy(&value, p, sz);
                    }
                    else {
                        value = MAGIC_NUMBER;
                    }
                    break;
                }
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
                    default: 
                        result = (value ==constant);
                        break;
                }

                return result;
            }
        }
        
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
                fclose(f);
                current_cnt = records_sz;
            }
        }
        

        if (strcmp(getenv(operator_temp),"4")==0) {
            if (records_sz < MAXSZ){
                records[records_sz++] = 1;
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


            return 1;
        }

        int var=atoi(getenv(variable_temp));
        int oper=atoi(getenv(operator_temp));
        int constant=atoi(getenv(constant_temp));
        long long value=0;

        va_list ap;
        va_start(ap, count);
        for (unsigned long i = 0; i < (unsigned long)count; i++) {
            void* p = va_arg(ap, void*);
            unsigned long sz = va_arg(ap, unsigned long);
            assert( sz <= 8 );

            if (i==var){
                if (isGoodAddr(p, sz)) {
                    memcpy(&value, p, sz);
                }
                else {
                    value = MAGIC_NUMBER;
                }
                break;
            }
        }

        if (value==MAGIC_NUMBER) return 0;
        else{
            switch(oper){
                case 1: 
                    if (records_sz < MAXSZ){
                        records[records_sz++] = (value !=constant);
                    }
                    break;
                case 2: 
                    if (records_sz < MAXSZ){
                        records[records_sz++] = (value >constant);
                    }
                    break;
                case 3: 
                    if (records_sz < MAXSZ){
                        records[records_sz++] = (value <constant);
                    }
                    break;
                default: 
                    if (records_sz < MAXSZ){
                        records[records_sz++] = (value ==constant);
                    }
                    break;
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
            fclose(f);

            return records[records_sz-1];
        }

    }
    else if (strcmp(is_neg, "SE") == 0){
        va_list ap;
        va_start(ap, count);
        int lval[100];
        char *lval_id[100];
        for (unsigned long i = 0; i < (unsigned long)count; i++) {
            void* p = va_arg(ap, void*);
            unsigned long sz = va_arg(ap, unsigned long);
            long long value;
            
            if (isGoodAddr(p, sz)) {
                memcpy(&value, p, sz);
            }
            else {
                value = MAGIC_NUMBER;
            }

            lval[i]=(int)value;
            char id[5];
            sprintf(id,"x%lu",i);
            lval_id[i]=id;
        }
        int *rval[1];
        char *rval_id[1];
        // return __trident_choice(lid,"bool",lval,lval_id,count,rval, rval_id, 0);
        int ret;
        klee_make_symbolic(&ret,sizeof(int),"ret");
        return ret;
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