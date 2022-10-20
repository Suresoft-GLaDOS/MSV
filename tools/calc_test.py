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

if __name__ == "__main__":
    if len(argv) < 4:
        print ("Usage: calc_test.py <src_dir> <test_dir> <work_dir> [cases]")
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
    
    try:
        mkdir(temp_dir);
    except:
        pass
    orig_dir=getcwd()
    chdir(temp_dir)

    src_dir = args[0];
    test_dir = args[1];
    work_dir = args[2];
    if profile_dir == "":
        cur_dir = src_dir;
    else:
        cur_dir = profile_dir;
    if len(args) > 3:
        ids = args[3:];
        for i in ids:
            shutil.copyfile(test_dir + "/" + str(i) + ".in", "./" + str(i) + ".in")
            shutil.copyfile(test_dir + "/" + str(i) + ".exp", "./" + str(i) + ".exp")

            in_file=open(test_dir+"/"+i+".in")
            in_arg=in_file.readline()
            in_file.close()

            temp_env=environ.copy()
            system('rm -rf /tmp/'+str(i)+'_profile.log')

            if (i != "0"):
                cmd = cur_dir +"/prog "+in_arg
            else:
                cmd = cur_dir + "/prog"
            subp=subprocess.Popen(cmd,shell=True,env=temp_env,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
            (out,err)=subp.communicate()
            if (subp.returncode == 0):
                expected_output=open("./"+str(i)+".exp",'r')
                exp=expected_output.read()
                expected_output.close()
                out=out.decode('utf-8')

                if (exp == out):
                    print (i)
                else:
                    print("failed at {0}".format(i),file=stderr)

                system("rm -rf __out"+str(i))
        
    chdir(orig_dir)
    shutil.rmtree(temp_dir)
