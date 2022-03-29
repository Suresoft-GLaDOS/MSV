#!/usr/bin/env python3
import getopt
import json
from os import chdir, getcwd, path
import subprocess
from sys import argv
from typing import List

from tester_common import extract_arguments


if __name__=="__main__":
    compile_only = False

    opts, args = getopt.getopt(argv[1:],'cd:hlj:p:r:x');
    dryrun_src = ""

    paraj = 0

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
        print("Usage: dpp-build.py <directory> [-d src_file | -l] [-h]")
        exit(0)

    out_dir = args[0]
    # fetch from github if the directory does not exist
    if path.exists(out_dir):
        print("Working with existing directory: " + out_dir)
    else:
        print("Non-exists directory")
        exit(1)

    # Build it
    orig_dir=getcwd()
    chdir(out_dir)
    meta_file=open(f'{out_dir}/meta.json', 'r')
    meta_root=json.load(meta_file)
    meta_file.close()

    build_commands:List[str]=meta_root['common']['build']['commands'][0]['lines']
    for command in build_commands:
        words=command.split()
        while '@DPP_CMAKE_GEN_COMPILATION_DB@' in words:
            words.remove('@DPP_CMAKE_GEN_COMPILATION_DB@')
        while '@DPP_CMAKE_COMPILATION_DB_TOOL@' in words:
            words.remove('@DPP_CMAKE_COMPILATION_DB_TOOL@')
        while '@DPP_COMPILATION_DB_TOOL@' in words:
            words.remove('@DPP_COMPILATION_DB_TOOL@')
        for i,word in enumerate(words.copy()):
            if '@DPP_PARALLEL_BUILD@' in word:
                word.replace('@DPP_PARALLEL_BUILD@',str(paraj))

        result=subprocess.run(words,stderr=subprocess.PIPE,stdout=subprocess.PIPE)
        if result.returncode != 0:
            print(result.stderr.decode('utf-8'))
            chdir(orig_dir)
            exit(1)
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
