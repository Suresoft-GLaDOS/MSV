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
from os import environ, system, path, getcwd, chdir, remove
import subprocess
import getopt
import multiprocessing as mp
import uuid
from difflib import SequenceMatcher

def run_test(testcase):
    my_env = environ.copy()
    my_env["RUNTESTS"] = testcase;
    #print("Running test case: " + testcase, flush=True)
    out_file = f"__out_{testcase}"
    if "__PID" in my_env:
        out_file = f"__out_{my_env['__PID']}"
    if path.exists(out_file):
        remove(out_file)
    tmp_id = uuid.uuid4()
    tmp_exp_file = f"/tmp/{tmp_id}.exp"
    tmp_out_file = f"/tmp/{tmp_id}.out"
    my_env["MSV_TMP_EXP"] = tmp_exp_file
    my_env["MSV_TMP_OUT"] = tmp_out_file
    # print("exp: " + tmp_exp_file, flush=True)
    # print("out: " + tmp_out_file, flush=True)
    # print(f"perl -I {path.join(my_env['MSV_PATH'], 'benchmarks', 'lighttpd-script')} run-tests.pl 1>{out_file}")
    ret = subprocess.call([f"perl -I {path.join(my_env['MSV_PATH'], 'benchmarks', 'lighttpd-script')} run-tests.pl 1>{out_file} 2>/dev/null"], shell=True, env = my_env);
    #ret = subprocess.call([f"perl run-tests.pl 1>{out_file}  2>/dev/null"], shell=True, env = my_env);
    if "MSV_OUTPUT_DISTANCE_FILE" in my_env:
        exp = ""
        out = ""
        exp_file = path.join(my_env["MSV_PATH"], "benchmarks", "lighttpd-exp", str(testcase) + ".exp")
        try:
            if path.exists(tmp_exp_file):
                with open(tmp_exp_file, "r") as f1:
                    exp = f1.read()
                #remove(tmp_exp_file)
            if path.exists(tmp_out_file):
                with open(tmp_out_file, "r") as f2:
                    out = f2.read()
                #remove(tmp_out_file)
            with open(my_env["MSV_OUTPUT_DISTANCE_FILE"], "w") as f3:
                seqMatch = SequenceMatcher(None, exp, out)
                match = seqMatch.find_longest_match(0, len(exp), 0, len(out)).size
                dist = max(len(out) - match, len(exp) - match)
                f3.write(str(dist))
        except:
            pass
    if ret != 0:
        system(f"rm -rf {out_file}");
        return

    with open(out_file, "r") as fin:
        outs = fin.readlines();

    if ("Result: PASS\n" in outs):
        # print(f"cp {tmp_out_file} {exp_file}")
        # system(f"cp {tmp_out_file} {exp_file}")
        print (testcase, flush=True)
    system(f"rm -rf {out_file}");

if __name__ == "__main__":
    if len(argv) < 4:
        print ("Usage: lighttpd-py <src_dir> <test_dir> <work_dir> [cases]")
        exit(1);

    opts, args = getopt.getopt(argv[1:], "p:j:t:i:");
    profile_dir = "";
    max_parallel=1
    tempdir='my-tests'
    timeout=None
    for o, a in opts:
        if o == "-p":
            profile_dir = a;
        elif o=='-j':
            max_parallel=int(a)
        elif o=='-t':
            timeout=int(a)
        elif o=='-i':
            tempdir=a

    src_dir = args[0];
    test_dir = args[1];
    work_dir = args[2];
    if len(args) > 3:
        ids = args[3:];
        #ids = list(map(str, range(1, 215)))
        cur_dir = src_dir;
        if (profile_dir != ""):
            cur_dir = profile_dir;

        if (not path.exists(cur_dir+"/"+tempdir)):
            system("cp -rf " + test_dir + " " + cur_dir + "/"+tempdir);
            system("rm -f " + path.join(cur_dir, tempdir, "LightyTest.pm"))
            system("cp -f " + path.join(environ["MSV_PATH"], "benchmarks", "lighttpd-script", "LightyTest.pm") + " " + path.join(cur_dir, tempdir, "LightyTest.pm"));

        system("killall -9 lighttpd > /dev/null 2> /dev/null");

        ori_dir = getcwd();
        chdir(cur_dir + "/"+tempdir);

        ret = subprocess.call(["sh prepare.sh >/dev/null"], shell=True);
        if ret != 0:
            print ("Error on preparing")
            assert(0);

        result=[]
        pool=mp.Pool(max_parallel)
        for i in ids:
            testcase = str(i);
            result.append(pool.apply_async(run_test, (testcase, )))
        #print();
        pool.close()
        for r in result:
            r.wait(timeout)
        pool.join()

        subprocess.call(["sh cleanup.sh > /dev/null"], shell=True);

        chdir(ori_dir);
