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

TEST_CASES=[
    'A01grammar.ztst',
    'A02alias.ztst',
    'A03quoting.ztst',
    'A04redirect.ztst',
    'A05execution.ztst',

    'A06assign.ztst',
    'A07control.ztst',
    'B01cd.ztst',
    'B02typeset.ztst',
    'B03print.ztst', # 10

    'B04read.ztst',
    'B05eval.ztst',
    'B06fc.ztst',
    'B07emulate.ztst',
    'B08shift.ztst',

    'B09hash.ztst',
    'B10getopts.ztst',
    'B11kill.ztst',
    'B12limit.ztst',
    'B13whence.ztst', # 20

    'C01arith.ztst',
    'C02cond.ztst',
    'C03traps.ztst',
    'C04funcdef.ztst',
    'C05debug.ztst',

    'D01prompt.ztst',
    'D02glob.ztst',
    'D03procsubst.ztst',
    'D04parameter.ztst',
    'D05array.ztst', # 30

    'D06subscript.ztst',
    'D07multibyte.ztst',
    'D08cmdsubst.ztst',
    'D09brace.ztst',
    'E01options.ztst',
    
    'E02xtrace.ztst',
    'E03posix.ztst',
    'P01privileged.ztst',
    'V01zmodload.ztst',
    'V02zregexparse.ztst', # 40

    'V03mathfunc.ztst',
    'V04features.ztst',
    'V05styles.ztst',
    'V06parameter.ztst',
    'V07pcre.ztst',

    'V08zpty.ztst',
    'V09datetime.ztst',
    'V10private.ztst',
    'V11db_gdbm.ztst',
    'V12zparseopts.ztst', # 50

    'V13zformat.ztst',
    'V14system.ztst',
    'W01history.ztst',
    'W02jobs.ztst',
    'X02zlevi.ztst',

    'X03zlebindkey.ztst',
    'X04zlehighlight.ztst',
    'Y01completion.ztst',
    'Y02compmatch.ztst',
    'Y03arguments.ztst', # 60

    'Z01is-at-least.ztst',
    'Z02zmathfunc.ztst',
    'Z03run-help.ztst',
]
def num2testcase( case ):
    if case < 0 or case > len(TEST_CASES):
        return 'nothing'
    else:
        return TEST_CASES[case]

import psutil

def run_test(testcase,id,timeout):
    my_env=environ.copy()
    my_env['ZTST_ext']='../Src/zsh'
    my_env['ZTST_srcdir']='.'
    my_env['ZTST_testlist']=testcase
    proc = subprocess.Popen(["../Src/zsh", "ztst.zsh",f'{testcase}'],stdout=subprocess.PIPE,stderr=subprocess.PIPE,env=my_env)
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
        chdir(cur_dir+'/Test');

        result=[]
        pool=mp.Pool(max_parallel)
        for i in ids:
            testcase = num2testcase(int(i));
            run_test(testcase,int(i),timeout)
            # result.append(pool.apply_async(run_test,(testcase,int(i),my_env,timeout,)))

        print();
        pool.close()
        pool.join()

        chdir(ori_dir);


