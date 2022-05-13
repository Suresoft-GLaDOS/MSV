#!/usr/bin/env python3
import getopt
from os import chdir, getcwd, path,environ
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
        print("Usage: openssl-build.py <directory> [-d src_file | -l] [-h]")
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

    paths=environ['PATH'].split(':')
    wrap_path=''
    for path in paths.copy():
        if 'MSV/wrap' in path:
            wrap_path=path
            paths.remove(path)
            break
    config_path=''
    for path in paths:
        config_path+=path+':'
    config_path=config_path[:-1]
    environ['PATH']=config_path
    if not compile_only:
        result=subprocess.run(['./config'],shell=True)
        if result.returncode != 0:
            exit(1)
        elif config_only:
            exit(0)
    
    environ['PATH']=wrap_path+':'+environ['PATH']
    result=subprocess.run(['make',f'-j{paraj}'])
    if result.returncode!=0:
        exit(result.returncode)

    result=subprocess.run(['make','test'])
    chdir(orig_dir)

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

    exit(result.returncode)

