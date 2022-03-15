#!/usr/bin/env python3
from os import chdir, getcwd,system
import subprocess
from yaml import safe_load, dump
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper
import getopt
from sys import argv
import psutil
import multiprocessing as mp

def run_test(id,test,commands,timeout):
    test_splitted=test.split()
    final_commands=[]
    for command in commands:
        cur_cmd=[]
        for arg in command:
            if arg=='"@testcase@"':
                cur_cmd.append(test)
            else:
                cur_cmd.append(arg)
        final_commands.append(cur_cmd)
    
    failed=False
    for command in final_commands:
        proc=subprocess.Popen(command,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        try:
            so,se=proc.communicate(timeout=timeout)
            if proc.returncode!=0:
                failed=True
                break
        except:
            pid=proc.pid
            children=[]
            for child in psutil.Process(pid).children(True):
                if psutil.pid_exists(child.pid):
                    children.append(child)

            for child in children:
                child.kill()
            proc.kill()
            failed=True
            break
    
    if not failed:
        print(id)


if __name__ == "__main__":
    opts, args = getopt.getopt(argv[1:], "p:i:j:t:");
    profile_dir = "";
    temp_dir="my-test"
    max_parallel=1
    timeout=None
    for o, a in opts:
        if o == "-p":
            profile_dir = a;
        elif o=="-i":
            temp_dir=a
        elif o=='-j':
            max_parallel=int(a)
        elif o=='-t':
            timeout=int(a)

    src_dir = args[0]
    test_dir = args[1]
    work_dir = args[2]

    cur_dir = src_dir
    if (profile_dir != ""):
        cur_dir = profile_dir

    ori_dir = getcwd()
    chdir(cur_dir)

    vulcan_file=open('vulcan.yml','r')
    vulcan_object=safe_load(vulcan_file)
    vulcan_file.close()

    # Read test command
    commands=vulcan_object['test-command'].splitlines()
    test_commands=[]
    testcase_index=[]
    for i,command in enumerate(commands):
        test_command=command.split()
        for j,arg in enumerate(test_command):
            if arg=='"@testcase@"':
                testcase_index.append((i,j))
        test_commands.append(test_command)
    
    testcases=[]
    is_list=False
    # Read test list or case
    if 'test-list' in vulcan_object.keys():
        tests=vulcan_object['test-list'].splitlines()
        is_list=True
        for test in tests:
            testcases.append(test)
    else:
        tests=vulcan_object['test-case'].splitlines()
        for test in tests:
            testcases.append(test)

    result=[]
    if (len(args) > 3):
        ids = args[3:]
        # pool=mp.Pool(max_parallel)
        for i in ids:
            # print "Testing "+testcase;
            # result.append(pool.apply_async(run_test,(int(i),testcases[int(i)],test_commands,timeout,)))
            run_test(int(i),testcases[int(i)],test_commands,timeout)

        # pool.close()
        # pool.join()

    chdir(ori_dir)