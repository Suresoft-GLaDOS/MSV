#!/usr/bin/env python3
# Copyright (C) 2016 Fan Long, Martin Rianrd and MIT CSAIL 
# Prophet
# 
# This file is part of Prophet.
# 
# Prophet is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Prophet is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Prophet.  If not, see <http://www.gnu.org/licenses/>.
from sys import argv, stderr
from os import rmdir, system,mkdir,chdir,getcwd,environ
import getopt
import shutil
import subprocess

def get_test_name(id):
    list_process=subprocess.Popen('make list-tests',shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    (list_out,list_err)=list_process.communicate()
    list_out=list_out.decode('utf-8')
    out_lines=list_out.splitlines()
    for line in out_lines.copy():
        if line[0:4]!='test':
            out_lines.remove(line)

    return out_lines[id-1]

import psutil

def run_test(testcase,id,timeout):
    proc = subprocess.Popen(["make", "test",f'TESTS={testcase}'],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    try:
        so,se=proc.communicate(timeout=timeout)
        if proc.returncode==0:
            print (id)
    except:
        pid=proc.pid
        children=[]
        for child in psutil.Process(pid).children(True):
            if psutil.pid_exists(child.pid):
                children.append(child)

        for child in children:
            child.kill()
        proc.kill()

if __name__ == "__main__":
    if len(argv) < 4:
        print ("Usage: openssl-test.py <src_dir> <test_dir> <work_dir> [cases]")
        exit(1);

    opts, args = getopt.getopt(argv[1:], "p:i:t:j:");
    profile_dir = "";
    temp_dir="__temp_test"
    timeout=None
    max_cpu=1
    for o, a in opts:
        if o == "-p":
            profile_dir = a;
        elif o=='-i':
            temp_dir=a
        elif o=='-t':
            timeout=int(a)
        elif o=='-j':
            max_cpu=int(a)
    
    orig_dir=getcwd()

    src_dir = args[0];
    test_dir = args[1];
    work_dir = args[2];
    if profile_dir == "":
        cur_dir = src_dir;
    else:
        cur_dir = profile_dir;

    if len(args) > 3:
        ids = args[3:]
        chdir(cur_dir)
        system(f'cp -rf {environ["MSV_PATH"]}/tools/openssl-helper.c {cur_dir}/test/testutil/tests.c')
        for i in ids:
            testcase=get_test_name(int(i))
            run_test(testcase,int(i),timeout)

    chdir(orig_dir)
