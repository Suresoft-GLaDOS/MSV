#!/usr/bin/env python3
import getopt
from os import chdir, environ, getcwd, path
import subprocess
from sys import argv

from tester_common import extract_arguments


if __name__=="__main__":
    compile_only = False

    opts, args = getopt.getopt(argv[1:],'cd:hlj:p:r:x');
    dryrun_src = ""

    paraj = 1

    print_fix_log = False
    print_usage = False
    config_only = False
    for o, a in opts:
        if o == "-d":
            dryrun_src = a
        elif o == "-p":
            if a[0] == "/":
                deps_dir = a
            else:
                deps_dir = getcwd() + "/" + a
        elif o == "-x":
            config_only = True
        elif o == "-c":
            compile_only = True
        elif o == "-l":
            print_fix_log = True
        elif o == "-h":
            print_usage = True
        elif o == "-j":
            paraj = int(a)

    if (len(args) < 1) or (print_usage):
        print("Usage: zsh-build.py <directory> [-d src_file | -l] [-h]")
        exit(0)

    out_dir = args[0]
    # fetch from github if the directory does not exist
    if path.exists(out_dir):
        print("Working with existing directory: " + out_dir)
    else:
        print("Non-exists directory")
        exit(1)
    
    orig_dir=getcwd()
    chdir(out_dir)
    if not compile_only:
        result=subprocess.run(['./Util/preconfig'],shell=True)
        if result.returncode != 0:
            exit(1)

        result=subprocess.run(['./configure','--with-tcsetpgrp'],shell=True)
        if result.returncode != 0:
            exit(1)

        result=subprocess.run(['sed','-i','/^name=zsh\\/zpty/ s/link=no/link=static/','config.modules'])
        if result.returncode != 0:
            exit(1)

        result=subprocess.run(['make','clean'])
        if result.returncode != 0:
            exit(1)

        result=subprocess.run(['sed','-i','s|sleep 1;||','Test/Makefile'])
        if result.returncode != 0:
            exit(1)
        elif config_only:
            exit(0)
    
    # environ['PATH']=wrap_path+':'+environ['PATH']
    result=subprocess.run(['make',f'-j{paraj}'])
    chdir(orig_dir)
    if result.returncode != 0:
        exit(1)

    if dryrun_src != "":
        (builddir, buildargs) = extract_arguments(out_dir, dryrun_src)
        if len(args) > 1:
            out_file = open(args[1], "w")
            print(out_file, builddir)
            print(out_file, buildargs)
            out_file.close()
        else:
            print(builddir)
            print(buildargs)
