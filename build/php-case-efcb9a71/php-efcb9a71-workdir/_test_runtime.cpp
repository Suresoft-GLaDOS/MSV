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

extern "C" int __is_neg(const char *location,int count, ...) {std::string locStr(location);
size_t split=locStr.find("-");
size_t switchNum=atoi(locStr.substr(0,split).c_str());
size_t caseNum=atoi(locStr.substr(split+1).c_str());

if(switchNum==0 && caseNum==44){
switch(__choose(std::string("__CONDITION_0_44").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<1+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 6);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<1+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 4);
}
case 3:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<1+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 5);
}
case 4:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<1+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 7);
}
case 5:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<1+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 32);
}
case 6:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 7:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 1);
}
case 8:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 2);
}
case 9:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<3+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 10:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<3+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 1);
}
case 11:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<4+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 12:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<4+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 1);
}
case 13:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<4+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 2);
}
case 14:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<5+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 6);
}
case 15:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<5+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 4);
}
case 16:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<5+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 5);
}
case 17:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<5+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 7);
}
case 18:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<5+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 32);
}
case 19:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<6+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 20:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<6+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 1);
}
}
}
if(switchNum==1 && caseNum==38){
switch(__choose(std::string("__CONDITION_1_38").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<6+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<9+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 3:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<10+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 4:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<16+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 5:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<17+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 6:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<18+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 7:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<19+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 8:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<20+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 9:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<22+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 10:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<24+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 11:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<25+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 12:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<26+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 13:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<27+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 14:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<28+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 15:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<29+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 16:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<30+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 17:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<31+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 18:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<32+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 19:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<35+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 20:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<36+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 21:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<37+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 22:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<38+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 23:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<39+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 24:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<42+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 6);
}
case 25:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<44+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 26:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<47+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 27:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<48+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 28:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<54+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 29:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<55+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 30:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<56+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 31:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<57+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 32:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<58+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 33:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<60+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 34:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<62+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 35:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<63+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 36:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<64+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 37:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<65+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 38:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<66+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 39:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<67+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 40:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<68+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 41:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<69+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 42:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<70+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 43:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<73+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 44:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<74+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 45:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<75+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 46:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<76+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 47:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<77+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
}
}
if(switchNum==2 && caseNum==3){
switch(__choose(std::string("__CONDITION_2_3").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<6+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<7+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 3:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<8+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 4:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<9+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 5:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<10+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 6:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<11+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 7:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<15+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 8:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<16+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 9:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<17+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 10:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<18+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 11:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<19+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 12:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<20+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 13:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<22+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 14:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<24+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 15:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<25+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 16:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<26+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 17:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<27+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 18:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<28+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 19:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<29+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 20:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<30+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 21:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<31+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 22:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<32+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 23:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<35+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 24:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<36+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 25:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<37+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 26:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<38+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 27:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<39+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 28:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<42+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 6);
}
case 29:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<44+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 30:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<45+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 31:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<46+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 32:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<47+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 33:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<48+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 34:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<49+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 35:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<53+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 36:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<54+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 37:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<55+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 38:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<56+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 39:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<57+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 40:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<58+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 41:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<60+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 42:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<62+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 43:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<63+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 44:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<64+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 45:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<65+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 46:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<66+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 47:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<67+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 48:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<68+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 49:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<69+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 50:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<70+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 51:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<73+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 52:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<74+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 53:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<75+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 54:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<76+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 55:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<77+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
}
}
if(switchNum==3 && caseNum==1){
switch(__choose(std::string("__CONDITION_3_1").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<6+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<9+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 3:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<10+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 4:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<16+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 5:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<17+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 6:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<18+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 7:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<19+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 8:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<20+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 9:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<22+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 10:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<24+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 11:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<25+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 12:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<26+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 13:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<27+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 14:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<28+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 15:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<29+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 16:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<30+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 17:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<31+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 18:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<32+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 19:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<35+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 20:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<36+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 21:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<37+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 22:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<38+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 23:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<39+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 24:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<42+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 6);
}
case 25:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<44+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 26:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<47+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 27:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<48+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 28:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<54+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 29:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<55+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 30:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<56+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 31:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<57+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 32:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<58+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 33:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<60+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 34:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<62+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 35:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<63+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 36:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<64+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 37:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<65+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 38:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<66+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 39:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<67+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 40:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<68+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 41:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<69+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 42:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<70+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 43:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<73+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 44:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<74+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 45:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<75+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 46:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<76+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 47:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<77+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
}
}
if(switchNum==3 && caseNum==2){
switch(__choose(std::string("__CONDITION_3_2").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<6+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<9+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 3:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<10+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 4:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<16+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 5:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<17+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 6:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<18+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 7:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<19+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 8:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<20+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 9:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<22+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 10:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<24+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 11:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<25+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 12:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<26+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 13:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<27+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 14:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<28+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 15:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<29+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 16:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<30+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 17:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<31+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 18:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<32+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 19:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<35+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 20:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<36+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 21:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<37+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 22:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<38+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 23:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<39+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 24:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<42+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 6);
}
case 25:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<44+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 26:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<47+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 27:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<48+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 28:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<54+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 29:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<55+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 30:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<56+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 31:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<57+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 32:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<58+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 33:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<60+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 34:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<62+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 35:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<63+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 36:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<64+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 37:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<65+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 38:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<66+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 39:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<67+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 40:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<68+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 41:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<69+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 42:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<70+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 43:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<73+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 44:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<74+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 45:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<75+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 46:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<76+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 47:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<77+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
}
}
if(switchNum==13 && caseNum==39){
switch(__choose(std::string("__CONDITION_13_39").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<3+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 5);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<4+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 0);
}
case 3:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<4+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 4:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<5+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 5:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<6+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 6:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<6+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 7:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<6+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 1);
}
case 8:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<6+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 2);
}
case 9:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<14+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 10:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<15+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 11:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<22+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 12:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<28+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 13:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<31+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 0);
}
case 14:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<31+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 15:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<32+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 5);
}
case 16:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<33+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
}
}
if(switchNum==14 && caseNum==46){
switch(__choose(std::string("__CONDITION_14_46").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<3+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 5);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<4+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 0);
}
case 3:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<4+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 4:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<5+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 5:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<6+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 6:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<6+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 7:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<6+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 1);
}
case 8:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<6+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 2);
}
case 9:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<14+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 10:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<15+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 11:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<22+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 12:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<28+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 13:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<31+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 0);
}
case 14:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<31+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 15:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<32+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 5);
}
case 16:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<33+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
}
}
if(switchNum==19 && caseNum==1){
switch(__choose(std::string("__CONDITION_19_1").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<1+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 3:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 4:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 5:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 3);
}
case 6:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 8);
}
case 7:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 9);
}
case 8:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 10);
}
case 9:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 32);
}
case 10:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 42);
}
case 11:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 43);
}
case 12:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 48);
}
case 13:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 64);
}
case 14:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 128);
}
case 15:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 256);
}
case 16:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 320);
}
case 17:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<3+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 18:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<3+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 19:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<3+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 33);
}
case 20:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<3+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 49);
}
}
}
if(switchNum==53 && caseNum==208){
switch(__choose(std::string("__CONDITION_53_208").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 6);
}
case 3:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 1);
}
case 4:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<2+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 6);
}
case 5:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<3+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 6:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<3+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 6);
}
case 7:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<3+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v!= 1);
}
case 8:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<4+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 9:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<8+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 10:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<9+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 11:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<9+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 6);
}
case 12:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<10+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 13:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<11+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 14:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<12+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 15:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<12+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 6);
}
case 16:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<13+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 17:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<14+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
case 18:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<15+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
case 19:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<16+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 20:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<17+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 1);
}
}
}
if(switchNum==61 && caseNum==1){
switch(__choose(std::string("__CONDITION_61_1").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<4+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<7+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
}
}
if(switchNum==62 && caseNum==49){
switch(__choose(std::string("__CONDITION_62_49").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<4+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<7+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
}
}
if(switchNum==62 && caseNum==51){
switch(__choose(std::string("__CONDITION_62_51").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<4+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<7+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
}
}
if(switchNum==62 && caseNum==53){
switch(__choose(std::string("__CONDITION_62_53").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<4+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<7+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
}
}
if(switchNum==63 && caseNum==45){
switch(__choose(std::string("__CONDITION_63_45").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<4+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<7+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
}
}
if(switchNum==64 && caseNum==45){
switch(__choose(std::string("__CONDITION_64_45").c_str())){
case 0:
return 1;
case 1:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<4+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 2);
}
case 2:{
va_list ap;
va_start(ap, count);
void *p;unsigned long sz;long long v = 0;
for (size_t __i=0;__i<7+1;__i++){
p = va_arg(ap, void*);
sz = va_arg(ap, unsigned long);
}
if (isGoodAddr(p, sz)) {memcpy(&v, p, sz);}
else return 0;
return (v== 0);
}
}
}
}

