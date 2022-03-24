import getopt
from os import chdir, getcwd, system
import subprocess
from sys import argv, stderr
import multiprocessing as mp

import psutil

def run_test(id,timeout):
    result=subprocess.run(['gcc','-o','php-run-tests','php-run-tests.c'],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    if result.returncode!=0:
        print(result.stderr,file=stderr)
        return
    
    subp=subprocess.Popen(['./php-run-tests',f'{id}'],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    try:
        so,se=subp.communicate(timeout=timeout)
        if subp.returncode==0:
            print(id)
    except:
        pid=subp.pid
        children=[]
        for child in psutil.Process(pid).children(True):
            if psutil.pid_exists(child.pid):
                children.append(child)

        for child in children:
            child.kill()
        subp.kill()

        print('timeout!',file=stderr)

if __name__ == "__main__":
    if len(argv) < 4:
        print ("Usage: new-php-test.py <src_dir> <test_dir> <work_dir> [cases]")
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

    if (len(args) > 3):
        ids = args[3:]
        cur_dir = src_dir
        if (profile_dir != ""):
            cur_dir = profile_dir

        ori_dir = getcwd()
        chdir(cur_dir)
        result=[]
        pool=mp.Pool(max_cpu)
        for i in ids:
            run_test(int(i),timeout)

        pool.close()
        pool.join()
        system(f'killall --wait {cur_dir}/sapi/cli/php > /dev/null 2>&1')
        chdir(ori_dir);
