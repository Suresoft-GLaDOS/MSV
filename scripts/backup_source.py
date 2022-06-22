import benchmarks
import subprocess
import os

def read_buggy_file(benchmark):
    with open(f'/root/project/MSV-experiment/benchmarks/{benchmarks.get_subject(benchmark)}/{benchmarks.get_workdir(benchmark)}/repair.conf','r') as f:
        lines=f.readlines()
        for line in lines:
            line=line.strip()
            if line.startswith('bugged_file'):
                buggy_file=line.split('=')[1]
    
    return buggy_file

def copy_generated_file(benchmark):
    buggy_file=read_buggy_file(benchmark)
    result=subprocess.run(['cp','-rf',f'/root/project/MSV-experiment/benchmarks/{benchmarks.get_subject(benchmark)}/{benchmarks.get_workdir(benchmark)}/src/{buggy_file}',f'/root/project/MSV/scripts/meta-source/{benchmark}.c'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if result.returncode!=0:
        print(result.stdout.decode('utf-8'))
        exit(1)
    
    result=subprocess.run(['cp','-rf',f'/root/project/MSV-experiment/benchmarks/{benchmarks.get_subject(benchmark)}/{benchmarks.get_workdir(benchmark)}/switch-info.json',f'/root/project/MSV/scripts/meta-source/{benchmark}-switch-info.json'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if result.returncode!=0:
        print(result.stdout.decode('utf-8'))
        exit(1)
    
    result=subprocess.run(['cp','-rf',f'/root/project/MSV-experiment/benchmarks/{benchmarks.get_subject(benchmark)}/{benchmarks.get_workdir(benchmark)}/func-info.json',f'/root/project/MSV/scripts/meta-source/{benchmark}-func-info.json'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if result.returncode!=0:
        print(result.stdout.decode('utf-8'))
        exit(1)

if __name__=='__main__':
    for benchmark in benchmarks.BENCHMARKS:
        copy_generated_file(benchmark)