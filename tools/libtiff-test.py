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
from os import rmdir, system, path, chdir, getcwd, environ,mkdir
import subprocess
import getopt
import multiprocessing as mp

def num2testcase( case ):
    if case=="1":
        return "ascii_tag"
    elif case=="2":
        return "long_tag"
    elif case=="3":
        return "short_tag"
    elif case=="4":
        return "strip_rw"
    elif case=="5":
        return "rewrite"
    elif case=="6":
        return "bmp2tiff_palette.sh"
    elif case=="7":
        return "bmp2tiff_rgb.sh"
    elif case=="8":
        return "gif2tiff.sh"
    elif case=="9":
        return "ppm2tiff_pbm.sh"
    elif case=="10":
        return "ppm2tiff_pgm.sh"
    elif case=="11":
        return "ppm2tiff_ppm.sh"
    elif case=="12":
        return "tiffcp-g3.sh"
    elif case=="13":
        return "tiffcp-g3-1d.sh"
    elif case=="14":
        return "tiffcp-g3-1d-fill.sh"
    elif case=="15":
        return "tiffcp-g3-2d.sh"
    elif case=="16":
        return "tiffcp-g3-2d-fill.sh"
    elif case=="17":
        return "tiffcp-g4.sh"
    elif case=="18":
        return "tiffcp-logluv.sh"
    elif case=="19":
        return "tiffcp-thumbnail.sh"
    elif case=="20":
        return "tiffdump.sh"
    elif case=="21":
        return "tiffinfo.sh"
    elif case=="22":
        return "tiffcp-split.sh"
    elif case=="23":
        return "tiffcp-split-join.sh"
    elif case=="24":
        return "tiff2ps-PS1.sh"
    elif case=="25":
        return "tiff2ps-PS2.sh"
    elif case=="26":
        return "tiff2ps-PS3.sh"
    elif case=="27":
        return "tiff2ps-EPS1.sh"
    elif case=="28":
        return "tiff2pdf.sh"
    elif case=="29":
        return "tiffcrop-doubleflip-logluv-3c-16b.sh"
    elif case=="30":
        return "tiffcrop-doubleflip-minisblack-1c-16b.sh"
    elif case=="31":
        return "tiffcrop-doubleflip-minisblack-1c-8b.sh"
    elif case=="32":
        return "tiffcrop-doubleflip-minisblack-2c-8b-alpha.sh"
    elif case=="33":
        return "tiffcrop-doubleflip-miniswhite-1c-1b.sh"
    elif case=="34":
        return "tiffcrop-doubleflip-palette-1c-1b.sh"
    elif case=="35":
        return "tiffcrop-doubleflip-palette-1c-4b.sh"
    elif case=="36":
        return "tiffcrop-doubleflip-palette-1c-8b.sh"
    elif case=="37":
        return "tiffcrop-doubleflip-rgb-3c-16b.sh"
    elif case=="38":
        return "tiffcrop-doubleflip-rgb-3c-8b.sh"
    elif case=="39":
        return "tiffcrop-extract-logluv-3c-16b.sh"
    elif case=="40":
        return "tiffcrop-extract-minisblack-1c-16b.sh"
    elif case=="41":
        return "tiffcrop-extract-minisblack-1c-8b.sh"
    elif case=="42":
        return "tiffcrop-extract-minisblack-2c-8b-alpha.sh"
    elif case=="43":
        return "tiffcrop-extract-miniswhite-1c-1b.sh"
    elif case=="44":
        return "tiffcrop-extract-palette-1c-1b.sh"
    elif case=="45":
        return "tiffcrop-extract-palette-1c-4b.sh"
    elif case=="46":
        return "tiffcrop-extract-palette-1c-8b.sh"
    elif case=="47":
        return "tiffcrop-extract-rgb-3c-16b.sh"
    elif case=="48":
        return "tiffcrop-extract-rgb-3c-8b.sh"
    elif case=="49":
        return "tiffcrop-extractz14-logluv-3c-16b.sh"
    elif case=="50":
        return "tiffcrop-extractz14-minisblack-1c-16b.sh"
    elif case=="51":
        return "tiffcrop-extractz14-minisblack-1c-8b.sh"
    elif case=="52":
        return "tiffcrop-extractz14-minisblack-2c-8b-alpha.sh"
    elif case=="53":
        return "tiffcrop-extractz14-miniswhite-1c-1b.sh"
    elif case=="54":
        return "tiffcrop-extractz14-palette-1c-1b.sh"
    elif case=="55":
        return "tiffcrop-extractz14-palette-1c-4b.sh"
    elif case=="56":
        return "tiffcrop-extractz14-palette-1c-8b.sh"
    elif case=="57":
        return "tiffcrop-extractz14-rgb-3c-16b.sh"
    elif case=="58":
        return "tiffcrop-extractz14-rgb-3c-8b.sh"
    elif case=="59":
        return "tiffcrop-R90-logluv-3c-16b.sh"
    elif case=="60":
        return "tiffcrop-R90-minisblack-1c-16b.sh"
    elif case=="61":
        return "tiffcrop-R90-minisblack-1c-8b.sh"
    elif case=="62":
        return "tiffcrop-R90-minisblack-2c-8b-alpha.sh"
    elif case=="63":
        return "tiffcrop-R90-miniswhite-1c-1b.sh"
    elif case=="64":
        return "tiffcrop-R90-palette-1c-1b.sh"
    elif case=="65":
        return "tiffcrop-R90-palette-1c-4b.sh"
    elif case=="66":
        return "tiffcrop-R90-palette-1c-8b.sh"
    elif case=="67":
        return "tiffcrop-R90-rgb-3c-16b.sh"
    elif case=="68":
        return "tiffcrop-R90-rgb-3c-8b.sh"
    elif case=="69":
        return "tiff2rgba-logluv-3c-16b.sh"
    elif case=="70":
        return "tiff2rgba-minisblack-1c-16b.sh"
    elif case=="71":
        return "tiff2rgba-minisblack-1c-8b.sh"
    elif case=="72":
        return "tiff2rgba-minisblack-2c-8b-alpha.sh"
    elif case=="73":
        return "tiff2rgba-miniswhite-1c-1b.sh"
    elif case=="74":
        return "tiff2rgba-palette-1c-1b.sh"
    elif case=="75":
        return "tiff2rgba-palette-1c-4b.sh"
    elif case=="76":
        return "tiff2rgba-palette-1c-8b.sh"
    elif case=="77":
        return "tiff2rgba-rgb-3c-16b.sh"
    elif case=="78":
        return "tiff2rgba-rgb-3c-8b.sh"
    elif case=='79':
        return 'libtiff-extra-test'
    else:
        return 'SOME';

import psutil

def run_test(testcase,id,env,timeout):
    if '.sh' in testcase:
        test_script=open(testcase,'r')
        lines=test_script.readlines()
        test_script.close()
        if lines[0]=='#!/bin/sh\n':
            lines[0]='#!/bin/bash\n'
        test_script=open(testcase,'w')
        test_script.writelines(lines)
        test_script.close()

    if 'libtiff-extra-test-2' == testcase:
        proc=subprocess.Popen(['bash', '-c', "../tools/tiff2pdf ../.dpp/00112-libtiff-heapoverflow-_TIFFmemcpy 2>&1 | tee /tmp/dpp_test_result && grep -q 0000000872 /tmp/dpp_test_result"],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    elif 'libtiff-extra-test-3' == testcase:
        proc=subprocess.Popen(['bash', '-c', "../tools/tiffcp -i ../.dpp/00119-libtiff-shift-long-tif_jpeg /tmp/foo |& tee /tmp/log && grep 'runtime error' /tmp/log"],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    else:
        proc = subprocess.Popen(["make", "check", "TESTS="+testcase],env = env,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    try:
        so,se=proc.communicate(timeout=timeout)
        if proc.returncode==0:
            print (id)
    except:
        pid=proc.pid
        children=[]
        for child in psutil.Process(pid).children(True):
            if psutil.pid_exists(child.pid):
                children.append(child)

        for child in children:
            child.kill()
        proc.kill()

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

    src_dir = args[0];
    test_dir = args[1];
    work_dir = args[2];

    if (len(args) > 3):
        ids = args[3:];
        cur_dir = src_dir;
        if (profile_dir != ""):
            cur_dir = profile_dir;

        ori_dir = getcwd();
        if path.exists(cur_dir + "/"+temp_dir):
            system("rm -rf " + cur_dir + "/"+temp_dir);
        system("cp -rf " + test_dir + " " + cur_dir + "/"+temp_dir);

        chdir(cur_dir + "/"+temp_dir);
        system("rm -rf o-*.tiff o-*.ps o-*.pdf")

        my_env = environ;
        my_env["GENEXPOUT"] = "0";
        my_env["CMPEXPOUT"] = "1";
        result=[]
        pool=mp.Pool(max_parallel)
        for i in ids:
            testcase = num2testcase(i)
            if testcase=='libtiff-extra-test':
                if 'libtiff-2-workdir' in work_dir:
                    testcase='libtiff-extra-test-2'
                elif 'libtiff-3-workdir' in work_dir:
                    testcase='libtiff-extra-test-3'
            run_test(testcase,int(i),my_env,timeout)
            # result.append(pool.apply_async(run_test,(testcase,int(i),my_env,timeout,)))

        print();
        pool.close()
        pool.join()

        system(f'killall --wait {cur_dir}/* > /dev/null 2>&1')
        chdir(ori_dir);
        subprocess.call('rm -rf '+cur_dir+'/'+temp_dir,shell=True)


