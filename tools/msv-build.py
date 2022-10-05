#!/usr/bin/env python3
import subprocess
from os import chdir, environ, getcwd, system
from yaml import safe_load, dump
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper
import getopt
from sys import argv
from tester_common import extract_arguments, extract_arguments_cmake

if __name__=="__main__":
    compile_only = False
    config_only = False

    opts, args = getopt.getopt(argv[1:],'cd:hj:lp:r:xs:')
    dryrun_src = ""
    paraj = 0
    print_fix_log = False
    print_usage = False
    switch_id=0
    subdir='.'
    for o, a in opts:
        if o == "-d":
            dryrun_src = a
        elif o == "-c":
            compile_only = True
        elif o == "-x":
            config_only = True
        elif o == "-l":
            print_fix_log = True
        elif o == "-h":
            print_usage = True
        elif o == "-j":
            paraj = int(a)
        elif o=='-s':
            subdir=a

    if (len(args) < 1) or (print_usage):
        print("Usage: msv-build.py <directory> [-r revision | -d src_file | -l] [-h]")
        exit(1)
    out_dir = args[0]

    # Read infos from vulcan.yml
    org_dir=getcwd()
    chdir(out_dir)
    vulcan_file=open('vulcan.yml','r')
    vulcan_object=safe_load(vulcan_file)
    vulcan_file.close()

    is_cmake=False
    build_commands=vulcan_object['test-build-command'].splitlines()

    # Divide configure and compile commands
    config_commands=[]
    compile_commands=[]
    is_compile=False
    for command in build_commands:
        if command.startswith('cmake'):
            is_cmake=True
            subprocess.run(['rm','-rf','CMakeCache.txt','**/CMakeFiles'],stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
        if command.startswith('make') or ' make' in command:
            is_compile=True

        if not is_compile:
            config_commands.append(command)
        else:
            compile_commands.append(command)
    
    # Set environment variable, for cmake
    temp_env=environ.copy()
    temp_env['CC']='gcc'
    temp_env['CXX']='g++'

    # Run configure commands
    if not compile_only:
        for command in config_commands:
            ret=subprocess.run(command,shell=True,env=temp_env)
            if ret.returncode!=0:
                exit(ret.returncode)

    if config_only:
        exit(0)

    # Run make and post-make
    for command in compile_commands:
        ret=subprocess.run(command,shell=True,env=temp_env)
        if ret.returncode!=0:
            exit(ret.returncode)

    chdir(org_dir)
    if dryrun_src != "":
        if is_cmake:
            (builddir,buildargs)=extract_arguments_cmake(out_dir, dryrun_src,subdir)
        else:
            (builddir, buildargs) = extract_arguments(out_dir, dryrun_src,subdir)
        if len(args) > 1:
            out_file = open(args[1], "w")
            out_file.write(builddir+'\n'+buildargs+'\n')
            out_file.close()
        else:
            print(builddir)
            print(buildargs)
