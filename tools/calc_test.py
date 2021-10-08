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
from os import rmdir, system,mkdir,chdir,getcwd
import getopt
import shutil

if __name__ == "__main__":
    if len(argv) < 4:
        print "Usage: calc_test.py <src_dir> <test_dir> <work_dir> [cases]";
        exit(1);

    opts, args = getopt.getopt(argv[1:], "p:i:");
    profile_dir = "";
    temp_dir="__temp_test"
    for o, a in opts:
        if o == "-p":
            profile_dir = a;
        elif o=='-i':
            temp_dir=a
    
    mkdir(temp_dir);
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

            if (i != "0"):
                cmd = cur_dir +"/Calc "+in_arg+ " 1> __out";
            else:
                cmd = cur_dir + "/Calc 1> __out";
            ret = system(cmd);
            if (ret == 0):
                cmd = "diff __out " + str(i) + ".exp 1> /dev/null";
                ret = system(cmd);
                if (ret == 0):
                    print i,
            system("rm -rf __out");

        print;
        
    chdir(orig_dir)
    shutil.rmtree(temp_dir)
