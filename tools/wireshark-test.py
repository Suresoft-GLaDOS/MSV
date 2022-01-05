#!/usr/bin/env python
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
from sys import argv
import getopt
from os import chdir, getcwd, system, path, environ
import subprocess
import multiprocessing as mp

def run_test(case_str,id):
    ret = subprocess.call(["./mytest.sh " + case_str + " 1>/dev/null 2>/dev/null"], shell = True);
    if (ret == 0):
        print (id)

if __name__ == "__main__":
    opts, args = getopt.getopt(argv[1 :], "p:t:i:j:");
    profile_dir = "";
    max_parallel=1
    timeout=None
    for o, a in opts:
        if o == "-p":
            profile_dir = a;
        elif o=='-t':
            timeout=int(a)
        elif o=='-j':
            max_parallel=int(a)
        elif o=='i':
            pass

    src_dir = args[0];
    test_dir = args[1];
    work_dir = args[2];

    if (len(args) > 3):
        ids = args[3 :];
        cur_dir = src_dir;
        if (profile_dir != ""):
            cur_dir = profile_dir;
        if (not path.exists(cur_dir + "/oldtest")):
            system("mv " + cur_dir + "/test " + cur_dir + "/oldtest");
            system("cp -rf " + test_dir + " " + cur_dir + "/test");
            system("cp -rf " + test_dir + "/mytest.sh " + cur_dir + "/mytest.sh");
            system("cp -rf " + test_dir + "/services " + cur_dir + "/services");

        ori_dir = getcwd();
        chdir(cur_dir);
        my_env = environ;
        result=[]
        pool=mp.Pool(max_parallel)
        for i in ids:
            case_str = str(i)
            result.append(pool.apply_async(run_test,(case_str,int(i),)))
        chdir(ori_dir);
        pool.close()
        for r in result:
            r.wait(timeout)
        pool.join()

