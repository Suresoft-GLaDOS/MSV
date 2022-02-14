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

cases = [
    "helin-segv",
    "hufts",
    "memcpy-abuse",
    "mixed",
    "null-suffix-clobber",
    "stdin",
    "trailing-nul",
    "zdiff",
    "zgrep-f",
    "zgrep-signal",
    "znew-k"];

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

        if (not path.exists(cur_dir + "/"+temp_dir)):
            if path.exists(f"{environ['MSV_PATH']}/benchmarks/gzip-tests"):
                system("cp -rf " +
                       f"{environ['MSV_PATH']}/benchmarks/gzip-tests " 
                       + f"{cur_dir}/{temp_dir}");
            else:
                system("cp -rf " + test_dir + " " + cur_dir + "/"+temp_dir);

        if (not path.exists(cur_dir + "/build-aux/test-driver")):
            if (path.exists(test_dir + "/test-driver")):
                system("cp -rf " + test_dir + "/test-driver " + cur_dir + "/build-aux/test-driver");    

        ori_dir = getcwd();
        chdir(cur_dir + "/"+temp_dir);
        my_env = environ;
        for i in ids:
            case_str = cases[int(i) - 1];
            tmp_id = uuid.uuid4();
            tmp_exp_file = f"/tmp/{tmp_id}.exp";
            tmp_out_file = f"/tmp/{tmp_id}.out"
            print("exp: " + tmp_exp_file)
            print("out: " + tmp_out_file)
            environ["MSV_TMP_EXP"] = tmp_exp_file;
            environ["MSV_TMP_OUT"] = tmp_out_file;
            #ret = subprocess.call(["make", log_file], timeout=timeout, shell = True);
            ret = subprocess.call(["./" + case_str + " 1>/dev/null 2>/dev/null"], shell = True,timeout=timeout);
            if "MSV_OUTPUT_DISTANCE_FILE" in environ:
                with open(environ["MSV_OUTPUT_DISTANCE_FILE"], "w") as f:
                    exp = ""
                    out = ""
                    if path.exists(tmp_exp_file):
                        with open(tmp_exp_file, "rb") as f1:
                            exp = f1.read();
                            #print("exp: " + exp)
                        #remove(tmp_exp_file);
                    if path.exists(tmp_out_file):
                        with open(tmp_out_file, "rb") as f2:
                            out = f2.read();
                            #print("out: " + out)
                        #remove(tmp_out_file);
                    seqMatch = SequenceMatcher(None, exp, out)
                    match = seqMatch.find_longest_match(0, len(exp), 0, len(out)).size
                    dist = max(len(out) - match, len(exp) - match)
                    #dist = Levenshtein.distance(exp, out)
                    f.write(str(dist));
            if (ret == 0):
                print (i)
        chdir(ori_dir);
