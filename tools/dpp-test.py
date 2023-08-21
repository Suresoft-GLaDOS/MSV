#!/usr/bin/env python3
import getopt
import json
from os import chdir, getcwd
import subprocess
from sys import argv

DPP_TEST_FILE='DPP_TEST_INDEX'

def run_test(id,test_cmds,workdir,timeout):
    with open(f'{workdir}/{DPP_TEST_FILE}','w') as test_file:
        test_file.write(f'{id}\n')

    for cmd in test_cmds:
        cmd=cmd.split()
        result=subprocess.run(cmd,timeout=timeout,shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        if result.returncode!=0:
            return
    print(id)

if __name__ == "__main__":
    opts, args = getopt.getopt(argv[1:], "p:i:j:t:");
    profile_dir = "";
    temp_dir="my-test"
    max_parallel=1
    timeout=None
    for o, a in opts:
        if o == "-p":
            profile_dir = a
        elif o=="-i":
            temp_dir=a
        elif o=='-j':
            max_parallel=int(a)
        elif o=='-t':
            timeout=int(a)

    src_dir = args[0]
    test_dir = args[1]
    work_dir = args[2]

    if (len(args) > 3):
        meta_file=open(f'{work_dir}/meta.json', 'r')
        meta_root=json.load(meta_file)
        meta_file.close()

        test_commands=meta_root['common']['test']['commands'][0]['lines']
        ids = args[3:]
        cur_dir = src_dir
        if (profile_dir != ""):
            cur_dir = profile_dir

        ori_dir = getcwd()
        chdir(cur_dir)
        for id in ids:
            run_test(int(id),test_commands,work_dir,timeout)
        
        chdir(ori_dir)
