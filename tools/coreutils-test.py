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
from sys import argv
import getopt
from os import chdir, getcwd, system, path, environ, remove
import subprocess
import uuid
#import Levenshtein
from difflib import SequenceMatcher
import psutil

if __name__ == "__main__":
    opts, args = getopt.getopt(argv[1 :], "p:j:t:i:");
    profile_dir = "";
    timeout=None
    temp_dir='mytests'
    for o, a in opts:
        if o == "-p":
            profile_dir = a;
        elif o=='-j':
            pass
        elif o=='-t':
            timeout=float(a)
        elif o=='-i':
            temp_dir=a

    src_dir = args[0];
    test_dir = args[1];
    work_dir = args[2];

    if (len(args) > 3):
        ids = args[3 :];
        cur_dir = src_dir;
        if (profile_dir != ""):
            cur_dir = profile_dir;

        ori_dir = getcwd();
        chdir(cur_dir)
        my_env = environ;
        for i in ids:
            proc = subprocess.Popen([f'make check-TESTS --no-print-directory TESTS=$(make print-TESTS | cut -d " " -f {i})'],
                                    shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            try:
                so,se = proc.communicate(timeout=timeout)
                if (proc.returncode == 0):
                    print (i)
            except:
                pid = proc.pid
                children = []
                for child in psutil.Process(pid).children(True):
                    if psutil.pid_exists(child.pid):
                        children.append(child)
                for child in children:
                    child.kill()
                proc.kill()
        chdir(ori_dir);
