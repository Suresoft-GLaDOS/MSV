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
from Levenshtein import distance

TEST_CASES=[
    'Test64BitPortability',
    'TestAssert',
    'TestAstUtils',
    'TestAutoVariables',
    'TestBool',

    'TestBoost',
    'TestBoost',
    'TestBufferOverrun',
    'TestBughuntingChecks',
    'TestCharVar', # 10

    'TestClangImport',
    'TestClass',
    'TestCmdlineParser',
    'TestCondition',
    'TestConstructors',

    'TestCppcheck',
    'TestErrorLogger',
    'TestExceptionSafety',
    'TestExprEngine',
    'TestFileLister', # 20

    'TestFunctions',
    'TestGarbage',
    'TestIO',
    'TestImportProject',
    'TestIncompleteStatement',

    'TestLeakAutoVar',
    'TestLeakAutoVarRecursiveCountLimit',
    'TestLeakAutoVarStrcpy',
    'TestLeakAutoVarWindows',
    'TestLibrary', # 30

    'TestMathLib',
    'TestMemleak',
    'TestMemleakInClass',
    'TestMemleakInFunction',
    'TestMemleakNoVar',

    'TestMemleakStructMember',
    'TestNullPointer',
    'TestOptions',
    'TestOther',
    'TestPath', # 40

    'TestPathMatch',
    'TestPlatform',
    'TestPostfixOperator',
    'TestPreprocessor',
    'TestSamples',

    'TestSimplifyTemplate',
    'TestSimplifyTokens',
    'TestSimplifyTypedef',
    'TestSimplifyUsing',
    'TestSizeof', # 50

    'TestStl',
    'TestString',
    'TestSummaries',
    'TestSuppressions',
    'TestSymbolDatabase',

    'TestThreadExecutor',
    'TestTimer',
    'TestToken',
    'TestTokenList',
    'TestTokenRange', # 60

    'TestTokenizer',
    'TestType',
    'TestUninitVar',
    'TestUnusedFunctions',
    'TestUnusedPrivateFunction',

    'TestUnusedVar',
    'TestUtils',
    'TestVaarg',
    'TestValueFlow',
    'TestVarID', # 70
]
def num2testcase( case ):
    if case < 0 or case > len(TEST_CASES):
        return 'nothing'
    else:
        return TEST_CASES[case]

import psutil

def run_test(testcase,id,timeout):
    proc = subprocess.Popen(["./testrunner", f"{testcase}"],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    try:
        so,se=proc.communicate(timeout=timeout)
        if proc.returncode==0:
            print (id)
            if 'MSV_OUTPUT_DISTANCE_FILE' in environ:
                file_name=environ['MSV_OUTPUT_DISTANCE_FILE']
                with open(file_name,'w') as file:
                    file.write('0\n')
        else:
            if 'MSV_OUTPUT_DISTANCE_FILE' in environ:
                try:
                    output=se.decode('utf-8')
                    output=output.splitlines()
                    actual_output=''
                    is_actual=False
                    expect_output=''
                    is_expect=False
                    for line in output:
                        if 'Expected:' in line and not is_expect:
                            is_expect=True
                            is_actual=False
                        elif is_expect and 'Actual:' not in line:
                            expect_output+=line+'\n'
                        elif 'Actual:' in line and not is_actual:
                            is_actual=True
                            is_expect=False
                        elif is_actual and 'Actual:' not in line and not '_____' in line:
                            actual_output+=line+'\n'
                    dist=distance(expect_output,actual_output)
                    file_name=environ['MSV_OUTPUT_DISTANCE_FILE']
                    with open(file_name,'w') as file:
                        file.write(f'{dist}\n')
                except:
                    pass

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
        chdir(cur_dir);

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


