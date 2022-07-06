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
        print(f'{benchmark} not downloaded, download it!')
        result=subprocess.run(['wget',f'https://www.cs.toronto.edu/~fanl/program_repair/scenarios/{benchmarks.BENCHMARKS_URL[i]}.tar.gz'])
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

    else:
        print(f'{benchmark} is already downloaded, skip!')
        
    if '__backup.log' in os.listdir(f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}'):
        with open(f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/__backup.log','r') as f:
            lines=f.readlines()
            for i,line in enumerate(lines):
                result=subprocess.run(['mv',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/__backup{i}',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/src/{line.strip()}'])

    print(f'Clean build {benchmark}!')
    if subject=='php':
        result=subprocess.run([f'/root/project/MSV/tools/{subject}-build.py','-p','/root/project/MSV/benchmarks/php-deps',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/src'])
    elif subject=='libtiff':
        result=subprocess.run([f'/root/project/MSV/tools/{subject}-build.py','-p','/root/project/MSV/benchmarks/libtiff-deps',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/src'])
    elif subject=='lighttpd':
        result=subprocess.run([f'/root/project/MSV/tools/{subject}-build.py','-p','/root/project/MSV/benchmarks/lighttpd-deps',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/src'])
    else:
        result=subprocess.run([f'/root/project/MSV/tools/{subject}-build.py',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/src'])
    if result.returncode!=0:
        exit(1)
    print('Clean build success')

    result=subprocess.run(['cp','-rf',f'/root/project/MSV-experiment/conf/{subject}/{benchmark}-repair.conf',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/repair.conf'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if result.returncode!=0:
        print(result.stdout.decode('utf-8'))
        exit(1)
    
    for file in os.listdir('/root/project/MSV/scripts/low-fl'):
        print(file)
        if benchmark in file:
            print(benchmark)
            result=subprocess.run(['cp','-rf',f'/root/project/MSV/scripts/low-fl/{file}',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/profile_localization.res'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
            if result.returncode!=0:
                print(result.stdout.decode('utf-8'))
                exit(1)
            break
    
    if 'meta-source' not in os.listdir('/root/project/MSV/scripts'):
        subprocess.run(['mkdir',f'/root/project/MSV/scripts/meta-source'])
    if f'{benchmark}.c' in os.listdir('/root/project/MSV/scripts/meta-source'):
        if '__backup.log' in os.listdir(f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}'):
            with open(f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/__backup.log','r') as f:
                lines=f.readlines()
                for i,line in enumerate(lines):
                    result=subprocess.run(['mv',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/__backup{i}',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/src/{line.strip()}'])
                    result=subprocess.run(['rm','-rf',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/__backup.log'])

        with open(f'/root/project/MSV-experiment/benchmarks/{benchmarks.get_subject(benchmark)}/{benchmarks.get_workdir(benchmark)}/repair.conf','r') as f:
            lines=f.readlines()
            for line in lines:
                line=line.strip()
                if line.startswith('bugged_file'):
                    buggy_file=line.split('=')[1]
                    result=subprocess.run(['cp','-rf',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/src/{buggy_file}',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/__backup0'])
                    with open(f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/__backup.log','w') as f:
                        f.write(f'{buggy_file}\n')

                    result=subprocess.run(['cp','-rf',f'/root/project/MSV/scripts/meta-source/{benchmark}-switch-info.json',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/switch-info.json'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
                    if result.returncode!=0:
                        print(result.stdout.decode('utf-8'))
                        exit(1)
                    result=subprocess.run(['cp','-rf',f'/root/project/MSV/scripts/meta-source/{benchmark}-func-info.json',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/func-info.json'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
                    if result.returncode!=0:
                        print(result.stdout.decode('utf-8'))
                        exit(1)

                    result=subprocess.run(['cp','-rf',f'/root/project/MSV/scripts/meta-source/{benchmark}.c',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/src/{buggy_file}'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
                    os.chdir(f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(benchmark)}/src')
                    print(f'{benchmark} meta-program applied, build...')
                    env=os.environ.copy()
                    env['PATH']='/root/project/MSV/wrap:'+env['PATH']
                    env['COMPILE_CMD']='clang'
                    result=subprocess.run(['make'],env=env)
                    print(f'Build finish!')
                    if result.returncode!=0:
                        print(result.stdout.decode('utf-8'))
                        exit(1)
                    break

    os.chdir(orig_dir)

def download_new_php(i:int,id:str):
    orig_dir=os.getcwd()
    subject=benchmarks.get_subject(id)
    if subject not in os.listdir('/root/project/MSV-experiment/benchmarks'):
        subprocess.run(['mkdir',f'/root/project/MSV-experiment/benchmarks/{subject}'])
    os.chdir(f'/root/project/MSV-experiment/benchmarks/{subject}')

    if f'{id}.tar.gz' not in os.listdir(f'/root/project/MSV-experiment/benchmarks/{subject}'):
        print(f'{id} not downloaded, download it!')
        result=subprocess.run(['wget',f'https://repairbenchmarks.cs.umass.edu/ManyBugs/scenarios/{benchmarks.BENCHMARKS[i]}.tar.gz'])
        if result.returncode!=0:
            print(result.stdout.decode('utf-8'))
            exit(1)
    
        result=subprocess.run(['tar','-xf',f'{benchmarks.BENCHMARKS[i]}.tar.gz'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        if result.returncode!=0:
            print(result.stdout.decode('utf-8'))
            exit(1)

        buggy_rev=id.split('-')[-2].strip()
        with open(f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.BENCHMARKS[i]}/bugged-program.txt','r') as f:
            lines=f.readlines()
            for line in lines:
                buggy_file=line.strip()
                result=subprocess.run(['cp','-rf',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.BENCHMARKS[i]}/diffs/{buggy_file}-{buggy_rev}',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.BENCHMARKS[i]}/php/{buggy_file}'])

        result=subprocess.run(['mkdir',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}'])
        result=subprocess.run(['cp','-rf',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.BENCHMARKS[i]}/php',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/src'])
        result=subprocess.run(['cp','-rf',f'/root/project/MSV/benchmarks/php-test',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/tests'])
        result=subprocess.run(['mkdir',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/__cleantests'])

        print(f'Clean build {id}!')
        if subject=='php':
            result=subprocess.run([f'/root/project/MSV/tools/{subject}-build.py','-p','/root/project/MSV/benchmarks/php-deps',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/src'])
        elif subject=='libtiff':
            result=subprocess.run([f'/root/project/MSV/tools/{subject}-build.py','-p','/root/project/MSV/benchmarks/libtiff-deps',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/src'])
        elif subject=='lighttpd':
            result=subprocess.run([f'/root/project/MSV/tools/{subject}-build.py','-p','/root/project/MSV/benchmarks/lighttpd-deps',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/src'])
        else:
            result=subprocess.run([f'/root/project/MSV/tools/{subject}-build.py',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/src'])
        if result.returncode!=0:
            exit(1)
        print('Clean build success')
    else:
        print(f'{id} is already downloaded, skip!')

    result=subprocess.run(['cp','-rf',f'/root/project/MSV-experiment/conf/{subject}/{id}-repair.conf',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/repair.conf'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if result.returncode!=0:
        print(result.stdout.decode('utf-8'))
        exit(1)
    result=subprocess.run(['cp','-rf',f'/root/project/MSV-experiment/conf/{subject}/{id}.revlog',f'/root/project/MSV-experiment/benchmarks/{subject}/{id}/{id}.revlog'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if result.returncode!=0:
        print(result.stdout.decode('utf-8'))
        exit(1)
    
    for file in os.listdir('/root/project/MSV/scripts/low-fl'):
        if id in file:
            result=subprocess.run(['cp','-rf',f'/root/project/MSV/scripts/low-fl/{file}',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/profile_localization.res'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
            if result.returncode!=0:
                print(result.stdout.decode('utf-8'))
                exit(1)
            break
    
    if f'{id}.c' in os.listdir('/root/project/MSV/scripts/meta-source'):
        if '__backup.log' in os.listdir(f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}'):
            with open(f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/__backup.log','r') as f:
                lines=f.readlines()
                for i,line in enumerate(lines):
                    result=subprocess.run(['mv',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/__backup{i}',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/src/{line.strip()}'])
                    result=subprocess.run(['rm','-rf',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/__backup.log'])

        with open(f'/root/project/MSV-experiment/benchmarks/{benchmarks.get_subject(id)}/{benchmarks.get_workdir(id)}/repair.conf','r') as f:
            lines=f.readlines()
            for line in lines:
                line=line.strip()
                if line.startswith('bugged_file'):
                    buggy_file=line.split('=')[1]
                    result=subprocess.run(['cp','-rf',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/src/{buggy_file}',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/__backup0'])
                    with open(f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/__backup.log','w') as f:
                        f.write(f'{buggy_file}\n')

                    result=subprocess.run(['cp','-rf',f'/root/project/MSV/scripts/meta-source/{id}-switch-info.json',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/switch-info.json'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
                    if result.returncode!=0:
                        print(result.stdout.decode('utf-8'))
                        exit(1)
                    result=subprocess.run(['cp','-rf',f'/root/project/MSV/scripts/meta-source/{id}-func-info.json',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/func-info.json'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
                    if result.returncode!=0:
                        print(result.stdout.decode('utf-8'))
                        exit(1)

                    result=subprocess.run(['cp','-rf',f'/root/project/MSV/scripts/meta-source/{id}.c',f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/src/{buggy_file}'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
                    os.chdir(f'/root/project/MSV-experiment/benchmarks/{subject}/{benchmarks.get_workdir(id)}/src')
                    print(f'{id} meta-program applied, build...')
                    env=os.environ.copy()
                    env['PATH']='/root/project/MSV/wrap:'+env['PATH']
                    env['COMPILE_CMD']='clang'
                    result=subprocess.run(['make'],env=env)
                    print(f'Build finish!')
                    if result.returncode!=0:
                        print(result.stdout.decode('utf-8'))
                        exit(1)
                    break

    os.chdir(orig_dir)


if __name__=='__main__':
    for i,benchmark in enumerate(benchmarks.BENCHMARKS):
        if benchmark.startswith('php-bug'):
            download_new_php(i,benchmark)
        else:
            download(i,benchmark)
