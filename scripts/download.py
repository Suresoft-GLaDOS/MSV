import benchmarks
import os
import subprocess

def download(benchmark):
    orig_dir=os.getcwd()
    subject=benchmarks.get_subject(benchmark)
    os.chdir(f'/root/project/MSV-experiment/benchmarks/{subject}')

    if f'{benchmark}.tar.gz' not in os.listdir(f'/root/project/MSV-experiment/benchmarks/{subject}'):
        result=subprocess.run(['wget',f'https://www.cs.toronto.edu/~fanl/program_repair/scenarios/{benchmark}.tar.gz'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        if result.returncode!=0:
            print(result.stdout.decode('utf-8'))
            exit(1)
    
    result=subprocess.run(['tar','-xf',f'{benchmark}.tar.gz'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if result.returncode!=0:
        print(result.stdout.decode('utf-8'))
        exit(1)
    
    result=subprocess.run(['cp','-rf',f'/root/project/MSV-experiment/conf/{subject}/{benchmark}-repair.conf',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/repair.conf'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if result.returncode!=0:
        print(result.stdout.decode('utf-8'))
        exit(1)
    
    for file in os.listdir('/root/project/MSV/scripts/low-fl'):
        if benchmark in file:
            result=subprocess.run(['cp','-rf',f'/root/project/MSV/scripts/low-fl/{file}',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/profile_localization.res'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
            if result.returncode!=0:
                print(result.stdout.decode('utf-8'))
                exit(1)
            break

    os.chdir(orig_dir)

for benchmark in benchmarks.BENCHMARKS:
    download(benchmark)