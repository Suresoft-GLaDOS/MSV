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
from php_tester import php_tester
from sys import argv,stderr
import getopt
from os import system
import os

if __name__ == "__main__":
    if len(argv) < 4:
        print ("Usage: php-tester.py <src_dir> <test_dir> <work_dir> [cases]")
        exit(1)

    opts, args = getopt.getopt(argv[1:], "p:i:t:j:")
    profile_dir = ""
    
    temp_dir=""
    timeout: float = 10.0
    max_cpu=1
    for o, a in opts:
        if o == "-p":
            profile_dir = a
        elif o=="-i":
            temp_dir=a
        elif o=='-t':
            timeout = float(a)
        elif o=='-j':
            max_cpu=int(a)

    src_dir = args[0]
    test_dir = args[1]
    work_dir = args[2]
    temp_dir='temp'
        
    if len(args) > 3:
        ids = args[3:]
        a = php_tester(work_dir, src_dir, test_dir,temp_dir,timeout=timeout,max_cpu=max_cpu)
        s = []
        for i in ids:
            s.append(int(i))
        ret = a.test(s, profile_dir)
                    
        for i in ret:
            print (i)
        # print "test"
        if len(ids) == 1 and len(ret) == 0:
            if "OUTIFFAIL" in os.environ:
                outf = work_dir + "/__cleantests/"
                if temp_dir!="":
                    outf+=temp_dir+"_tests/"
                outf += ids[0] + ".out"
                if os.path.exists(outf):
                    system("cp -rf " + outf + " " + os.environ["OUTIFFAIL"])
            if "EXPIFFAIL" in os.environ:
                expf = work_dir + "/__cleantests/"
                if temp_dir!="":
                    outf+=temp_dir+"_tests/"
                outf += ids[0] + ".exp"
                if os.path.exists(expf):
                    system("cp -rf " + expf + " " + os.environ["EXPIFFAIL"])
