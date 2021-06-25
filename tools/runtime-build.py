#!/usr/bin/env python3
from sys import argv
import subprocess

if len(argv)<3:
    print("Usage: runtime-build.py <work dir> <prophet src dir>")
    exit(1)

work_dir=argv[1]
src_dir=argv[2]
print(work_dir)
print(src_dir)

mv_cmd="mv "+src_dir+"/.libs/libtest_runtime.so.0.0.0 "+src_dir+"/.libs/libtest_runtime_bak.so.0.0.0"
mv_result=subprocess.call(mv_cmd,shell=True)
assert mv_result==0

compile_cmd="/usr/bin/g++ -c -fPIC "+work_dir+"/_test_runtime.cpp"
compile_result=subprocess.call(compile_cmd,shell=True)
assert compile_result==0

build_cmd="/usr/bin/g++ -shared -o "+work_dir+"/libtest_runtime.so "+work_dir+"/_test_runtime.o"
build_result=subprocess.call(build_cmd,shell=True)
assert build_result==0

copy_cmd="cp "+work_dir+"/libtest_runtime.so "+src_dir+"/.libs/libtest_runtime.so.0.0.0"
copy_result=subprocess.call(copy_cmd,shell=True)
assert copy_result==0