import benchmarks
import os
import subprocess

def download(i,benchmark):
    orig_dir=os.getcwd()
    subject=benchmarks.get_subject(benchmark)
    if subject not in os.listdir('/root/project/MSV-experiment/benchmarks'):
        subprocess.run(['mkdir',f'/root/project/MSV-experiment/benchmarks/{subject}'])
    os.chdir(f'/root/project/MSV-experiment/benchmarks/{subject}')

    if f'{benchmark}.tar.gz' not in os.listdir(f'/root/project/MSV-experiment/benchmarks/{subject}'):
        result=subprocess.run(['wget',f'https://www.cs.toronto.edu/~fanl/program_repair/scenarios/{benchmarks.BENCHMARKS_URL[i]}.tar.gz'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        if result.returncode!=0:
            print(result.stdout.decode('utf-8'))
            exit(1)
    
        result=subprocess.run(['tar','-xf',f'{benchmarks.BENCHMARKS_URL[i]}.tar.gz'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        if result.returncode!=0:
            print(result.stdout.decode('utf-8'))
            exit(1)
        
        if benchmark=='libtiff-tests-e0b51f3e':
            result=subprocess.run(['mv',f'/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests-e0b51f3e/libtiff-tests-e0b51f3e-workdir/__backup0','/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests-e0b51f3e/libtiff-tests-e0b51f3e-workdir/src/tools/tools/tiffcp.c'])
            result=subprocess.run(['rm',f'/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests-e0b51f3e/libtiff-tests-e0b51f3e-workdir/__backup.log'])
        elif benchmark=='python-69372-69368':
            result=subprocess.run(['mv',f'/root/project/MSV-experiment/benchmarks/python/python-case-69372-69368/python-69372-69368-workdir/__backup0','/root/project/MSV-experiment/benchmarks/python/python-case-69372-69368/python-69372-69368-workdir/src/Python/peephole.c'])
            result=subprocess.run(['rm',f'/root/project/MSV-experiment/benchmarks/python/python-case-69372-69368/python-69372-69368-workdir/__backup.log'])

    
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
    
    if 'meta-source' not in os.listdir('/root/project/MSV/scripts'):
        subprocess.run(['mkdir',f'/root/project/MSV/scripts/meta-source'])
    if f'{benchmark}.c' in os.listdir('/root/project/MSV/scripts/meta-source'):
        with open(f'/root/project/MSV-experiment/benchmarks/{benchmarks.get_subject(benchmark)}/{benchmarks.get_workdir(benchmark)}/repair.conf','r') as f:
            lines=f.readlines()
            for line in lines:
                line=line.strip()
                if line.startswith('bugged_file'):
                    buggy_file=line.split('=')[1]
                    result=subprocess.run(['cp','-rf',f'/root/project/MSV/scripts/meta-source/{benchmark}.c',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/src/{buggy_file}'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
                    break

    os.chdir(orig_dir)

for i,benchmark in enumerate(benchmarks.BENCHMARKS):
    download(i,benchmark)