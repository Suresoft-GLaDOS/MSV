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
from os import system, path, chdir, getcwd, environ
from tester_common import extract_arguments, get_fix_revisions
from php_tester import switch_to
import subprocess
import getopt

if __name__=="__main__":
    php_deps_dir = getcwd() + "/php-deps"

    compile_only = False
    config_only = False

    opts, args = getopt.getopt(argv[1:],'cd:hj:lp:r:x')
    dryrun_src = ""
    revision = ""
    paraj = 0
    print_fix_log = False
    print_usage = False
    switch_id=0
    for o, a in opts:
        if o == "-d":
            dryrun_src = a
        elif o == "-c":
            compile_only = True
        elif o == "-x":
            config_only = True
        elif o == "-p":
            php_deps_dir = a
        elif o == "-l":
            print_fix_log = True
        elif o == "-r":
            revision= a
        elif o == "-h":
            print_usage = True
        elif o == "-j":
            paraj = int(a)

    if (len(args) < 1) or (print_usage):
        print("Usage: new-php-build.py <directory> [-r revision | -d src_file | -l] [-h]")
        exit(1)
    out_dir = args[0]
    # fetch from github if the directory does not exist

    if print_fix_log:
        ret = get_fix_revisions(out_dir)
        for fix_r, previous_r, comment in ret:
            print("Fix Revision: " + fix_r)
            print("Previous Revision: " + previous_r)
            print("Comment:")
            print(comment)
    else:
        orig_dir=getcwd()
        chdir(out_dir)
        my_env = environ
        my_env["PATH"] = php_deps_dir + "/bison-2.2-build/bin:" + my_env["PATH"]
        my_env["PATH"] = php_deps_dir + "/autoconf-2.13:" + my_env["PATH"]
        my_env["PATH"] = php_deps_dir + "/flex-2.5.4-build/bin:" + my_env["PATH"]

        if not compile_only:
            subprocess.run(['make','clean'])
            subprocess.run(['./buildconf','--force'])
            p = subprocess.Popen(["./configure", "-with-libxml-dir=" + php_deps_dir + "/libxml2-2.7.2-build","-enable-zip"], env = my_env)
            (out, err) = p.communicate()
            subprocess.run(['make','clean'])

        if not config_only:
            ret = subprocess.call(["rm", "-rf", "ext/phar/phar.php"], env = my_env)
            assert( ret == 0)
            if paraj == 0:
                ret = subprocess.call(["make"], env = my_env)
            else:
                ret = subprocess.call(["make", "-j", str(paraj)], env = my_env)
        chdir(orig_dir)

        if dryrun_src != "":
            (builddir, buildargs) = extract_arguments(out_dir, dryrun_src)
            if len(args) > 1:
                out_file = open(args[1], "w")
                out_file.write(builddir)
                out_file.write("\n")
                out_file.write(buildargs)
                out_file.write("\n")
                # print(out_file, builddir)
                # print(out_file, buildargs)
                out_file.close()
            # else:
            #     print(builddir)
            #     print(buildargs)
