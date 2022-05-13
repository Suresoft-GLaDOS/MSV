#!/usr/bin/env python3
import getopt
from os import chdir, environ, getcwd, path, system
import subprocess
from sys import argv, stderr
import multiprocessing as mp
from Levenshtein import distance
import signal
import psutil

def run_test(id,timeout,workdir):
    subp=subprocess.Popen([f'{workdir}/php-run-tests {id}'],stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True)
    try:
        so,se=subp.communicate(timeout=timeout,input='some temp inputs')
        if subp.returncode==0:
            print(id)
            if 'MSV_OUTPUT_DISTANCE_FILE' in environ:
                file_name=environ['MSV_OUTPUT_DISTANCE_FILE']
                with open(file_name,'w') as file:
                    file.write('0\n')
        else:
            if 'MSV_OUTPUT_DISTANCE_FILE' in environ:
                    output=so.decode('utf-8')
                    output=output.splitlines()
                    for line in output:
                        if 'FAIL' or 'PASS' in line:
                            test_file=line[6:]
                            test_file=test_file.replace('.phpt','')
                            output_file=test_file+'.out'
                            expect_file=test_file+'.exp'

                            output=b''
                            if path.isfile(output_file):
                                with open(output_file,'rb') as f:
                                    output=f.read()
                            expect=b''
                            if path.isfile(expect_file):
                                with open(expect_file,'rb') as f:
                                    expect=f.read()
                            
                            dist=distance(output,expect)
                            file_name=environ['MSV_OUTPUT_DISTANCE_FILE']
                            with open(file_name,'w') as file:
                                file.write(f'{dist}\n')
                            break
                            

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
    timeout: float = 1000.0
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
    signal.signal(signal.SIGTTIN,signal.SIG_IGN)

    if (len(args) > 3):
        ids = args[3:]
        cur_dir = src_dir
        if (profile_dir != ""):
            cur_dir = profile_dir

        ori_dir = getcwd()
        chdir(work_dir)
        # Modify php tester script in Manybugs
        subprocess.run(['sed','-i','s|/usr/bin/php|./sapi/cli/php|g','php-run-tests.c'])
        subprocess.run(['sed','-i','-e','s|int killed = system("killall php &> /dev/null");||','php-run-tests.c'])
        subprocess.run(['sed','-i','-e','s|if (killed == 0) {||','php-run-tests.c'])
        subprocess.run(['sed','-i','-e','s|system("echo A php process was killed > A-PHP-Process-Was-Killed");    }||','php-run-tests.c'])
        subprocess.run(['gcc','-o','php-run-tests','php-run-tests.c'],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        chdir(cur_dir)
        result=[]
        pool=mp.Pool(max_cpu)
        for i in ids:
            # pool.apply_async(run_test,(int(i),timeout,))
            run_test(int(i),timeout,work_dir)

        pool.close()
        pool.join()
        system(f'killall --wait {cur_dir}/sapi/cli/php > /dev/null 2>&1')
        chdir(ori_dir);
