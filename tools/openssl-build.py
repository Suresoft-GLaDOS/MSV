#!/usr/bin/env python3

from os import chdir, getcwd
import subprocess
import getopt
from sys import argv, setcheckinterval, stderr

if __name__=="__main__":
    opts, args = getopt.getopt(argv[1:], "cd:hxj:");
    dryrun_src = "";
    compile_only = False;
    print_usage = False;
    config_only = False;
    parallel=1
    for o, a in opts:
        if o == "-x":
            config_only = True;
        if o == "-d":
            dryrun_src = a;
        elif o == "-c":
            compile_only = True;
        elif o == "-h":
            print_usage = True;
        elif o=="-j":
            parallel=int(a)

    orig_dir=getcwd()
    chdir(args[0])
    result=subprocess.run("./Configure enable-unit-test",shell=True)
    if result.returncode!=0:
        print("Fail at configure!",file=stderr)
        exit(1)

    if config_only:
        chdir(orig_dir)
        exit(0)
    
    result=subprocess.run('make clean',shell=True)
    if result.returncode!=0:
        print("Fail at clean!",file=stderr)
        exit(1)
    result=subprocess.run('make -j'+str(parallel)+' depend',shell=True)
    if result.returncode!=0:
        print("Fail at make dependencies!",file=stderr)
        exit(1)

    result=subprocess.run('make -j'+str(parallel)+'',shell=True)
    if result.returncode!=0:
        print("Fail at make!",file=stderr)
        exit(1)

    chdir(orig_dir)
    if dryrun_src != "":
        from tester_common import extract_arguments

        build_dir, build_args = extract_arguments(args[0], dryrun_src)
        if (len(args) > 1):
            out_file = open(args[1], "w")
            print(build_dir,file=out_file)
            print (build_args,file=out_file)
            out_file.close()
        else:
            print (build_dir)
            print (build_args)

