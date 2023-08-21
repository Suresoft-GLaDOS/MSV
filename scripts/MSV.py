from multiprocessing import Pool
import subprocess
import os
from sys import stderr
from time import time
import signal
import benchmarks

START_TIME=time()

def current_time():
    return int(time()-START_TIME)

def run(workdir):
    print(f'run {workdir}')

    if os.path.isfile(f'{workdir}/__backup.log'):
        f=open(f'{workdir}/__backup.log')
        lines=f.readlines()
        f.close()

        for i,file in enumerate(lines):
            os.system(f'mv {workdir}/__backup{i} {workdir}/src/{file}')
        os.system(f'rm -rf {workdir}/__backup.log')
    subp=None
    para_file='/root/project/MSV/crawler/para-rext-'
    if "new-php" in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/new-php-build.py", "-p", "/root/project/MSV/benchmarks/php-deps", f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        para_file+='php.out'
    elif "php" in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/php-build.py", "-p", "/root/project/MSV/benchmarks/php-deps", f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        para_file+='php.out'
    elif 'gzip' in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/gzip-build.py", f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        para_file+='all.out'
    elif 'gmp' in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/gmp-build.py",f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        para_file+='all.out'
    elif 'libtiff' in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/libtiff-build.py", "-p","/root/project/MSV/benchmarks/libtiff-deps",f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        para_file+='libtiff.out'
    elif 'lighttpd' in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/lighttpd-build.py", "-p","/root/project/MSV/benchmarks/lighttpd-deps",f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        para_file+='all.out'
    elif 'python' in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/python-build.py", f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        para_file+='python.out'
    elif 'wireshark' in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/wireshark-build.py", f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        para_file+='wireshark.out'


    if subp.returncode!=0:
        print(f'{workdir}: Fail at init building!',file=stderr)
        out=''
        err=''
        try:
            out=subp.stdout.decode('utf-8')
            with open(f'{id}.log','w') as f:
                f.write('stdout: '+out)
        except:
            with open(f'{id}.log','w') as f:
                f.write('stdout: '+subp.stdout)
        return (subp.returncode,out,'Fail at init building: '+err)
    
    print(f'Generate meta-program {workdir}')
    # if 'f2d9' in workdir or '3713' in workdir or 'cf05' in workdir:
    #     subp=subprocess.run(["prophet", "-r", f"{workdir}", "-replace-ext", "-skip-verify" , "-first-n-loc", "100",'-skip-profile','-feature-para',f'{para_file}','-msv-ext','-skip-add-stmt','-skip-replace-var','-skip-replace-func'],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    # elif '0d27' in workdir:
    #     subp=subprocess.run(["prophet", "-r", f"{workdir}", "-replace-ext", "-skip-verify" , "-first-n-loc", "100",'-skip-profile','-feature-para',f'{para_file}','-msv-ext','-skip-add-stmt','-skip-replace-var','-skip-replace-func'],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    # elif '2e42' in workdir or '6b64' in workdir:
    #     subp=subprocess.run(["prophet", "-r", f"{workdir}", "-replace-ext", "-skip-verify" , "-first-n-loc", "100",'-skip-profile','-feature-para',f'{para_file}','-msv-ext','-skip-add-stmt','-skip-replace-var'],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    # elif '1f49' in workdir or '453c' in workdir or '0927' in workdir or '3edb' in workdir or '187e' in workdir:
    #     subp=subprocess.run(["prophet", "-r", f"{workdir}", "-replace-ext", "-skip-verify" , "-first-n-loc", "100",'-skip-profile','-feature-para',f'{para_file}','-msv-ext','-skip-add-stmt'],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    # else:
    subp=subprocess.run(["prophet", "-r", f"{workdir}", "-replace-ext", "-skip-verify" , "-first-n-loc", "1",'-skip-profile','-feature-para',f'{para_file}','-msv-ext','-consider-all'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    print(f'[{current_time()}] Finish run {workdir} with {subp.returncode}')
    id=workdir.split('/')[-1]
    out=''
    try:
        out=subp.stdout.decode('utf-8')
        with open(f'{id}.log','w') as f:
            f.write('stdout: '+out)
    except:
        with open(f'{id}.log','w') as f:
            f.write('stdout: '+subp.stdout)
    return (subp.returncode,out,err)

if __name__=='__main__':
    pool=Pool(processes=5)
    result=[]
    signal.signal(signal.SIGHUP,signal.SIG_IGN)

    #for benchmark in benchmarks.BENCHMARKS:
    benchmark='gzip-f17cbd13a1d0a7'
    result.append(pool.apply_async(run,(f"/root/project/MSV-experiment/benchmarks/{benchmarks.get_subject(benchmark)}/{benchmarks.get_workdir(benchmark)}",)))

    pool.close()
    pool.join()
