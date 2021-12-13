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

    return out_lines[id-1]

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
    chdir(src_dir
    )
    if profile_dir == "":
        cur_dir = src_dir;
    else:
        cur_dir = profile_dir;
    if len(args) > 3:
        ids = args[3:]
        target=ids.copy()
        processes=[]
        current_test=[]
        is_finished=[]
        for i in range(max_cpu):
            if len(target)==0:
                break

            cmd=['make','TESTS="'+str(get_test_name(int(target[0])))+'"', 'test']
            processes.append(subprocess.Popen(cmd, stdout=subprocess.PIPE,stderr=subprocess.PIPE))
            current_test.append(target[0])
            is_finished.append(False)
            del target[0]
        ret = set();

        finish_all=0
        while True:
            if len(target)==0 and finish_all>=len(processes):
                break

            for i in range(len(processes)):
                if processes[i].poll()==None:
                    continue
                elif processes[i].poll()!=None and is_finished[i]==True:
                    continue

                (out, err) = processes[i].communicate();
                if (processes[i].returncode==0):
                    ret.add(current_test[i])
                
                if current_test[i] not in ret:
                    print("Fail at {0}".format(current_test[i]),file=stderr)
                
                if len(target)>0:
                    cmd=['make','TESTS="'+str(get_test_name(int(target[0])))+'"', 'test']
                    processes[i]=subprocess.Popen(cmd, stdout=subprocess.PIPE,stderr=subprocess.PIPE)
                    current_test[i]=target[0]
                    del target[0]
                else:
                    is_finished[i]=True
                    finish_all+=1
                    continue
        
        for i in ret:
            print(i)

    chdir(orig_dir)
