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
        subp=subprocess.run(["/root/project/MSV/tools/new-php-build.py", "-p", "/root/project/MSV/benchmarks/php-deps", f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        para_file+='php.out'
    elif "php" in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/php-build.py", "-p", "/root/project/MSV/benchmarks/php-deps", f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        para_file+='php.out'
    elif 'gzip' in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/gzip-build.py", f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        para_file+='all.out'
    elif 'gmp' in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/gmp-build.py",f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        para_file+='all.out'
    elif 'libtiff' in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/libtiff-build.py", "-p","/root/project/MSV/benchmarks/libtiff-deps",f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        para_file+='libtiff.out'
    elif 'lighttpd' in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/lighttpd-build.py", "-p","/root/project/MSV/benchmarks/lighttpd-deps",f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        para_file+='all.out'
    elif 'python' in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/python-build.py", f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        para_file+='python.out'
    elif 'wireshark' in workdir:
        subp=subprocess.run(["/root/project/MSV/tools/wireshark-build.py", f"{workdir}/src"],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        para_file+='wireshark.out'


    if subp.returncode!=0:
        print(f'{workdir}: Fail at init building!',file=stderr)
        out=''
        err=''
        try:
            out=subp.stdout.decode('utf-8')
            err=subp.stderr.decode('utf-8')
        except:
            pass
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
    subp=subprocess.run(["prophet", "-r", f"{workdir}", "-replace-ext", "-skip-verify" , "-first-n-loc", "200",'-skip-profile','-feature-para',f'{para_file}','-msv-ext'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
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

pool=Pool(processes=50)
result=[]
signal.signal(signal.SIGHUP,signal.SIG_IGN)

for benchmark in benchmarks.BENCHMARKS:
    result.append(pool.apply_async(run,(f"/root/project/MSV-experiment/benchmarks/{benchmarks.get_subject(benchmark)}/{benchmarks.get_workdir(benchmark)}",)))
# # gmp
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/gmp/gmp-case-13421/gmp-13421-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/gmp/gmp-case-14167/gmp-14167-workdir",))) # Correct

# # gzip
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/gzip/gzip-case-118a107f2d3ae5/gzip-118a107f2d3ae5-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/gzip/gzip-case-1204630c96e0c3/gzip-1204630c96e0c3-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/gzip/gzip-case-39a362ae9d9b00/gzip-39a362ae9d9b00-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/gzip/gzip-case-884ef6d16c6/gzip-884ef6d16c6-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/gzip/gzip-case-f17cbd13a1d0a7/gzip-f17cbd13a1d0a7-workdir",))) # Correct

# # libtiff
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests-0d27dc0/libtiff-tests-0d27dc0-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests-1563270c^1-1563270c/libtiff-tests-1563270c^1-1563270c-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests-2e42d63f/libtiff-tests-2e42d63f-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests-5f42db/libtiff-tests-5f42db-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests-6b6496/libtiff-tests-6b6496-workdir",))) # 11

# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests-bdba15c/libtiff-tests-bdba15c-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests-e0b51f3e/libtiff-tests-e0b51f3e-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests-eec7ec0/libtiff-tests-eec7ec0-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-0a36d7f/libtiff-tests2-0a36d7f-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-207c78a4/libtiff-tests2-207c78a4-workdir",))) # 16

# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-22da1d/libtiff-tests2-22da1d-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-371336d^1-371336d/libtiff-tests2-371336d^1-371336d-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-3edb9cd/libtiff-tests2-3edb9cd-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-6746b87^1-6746b87/libtiff-tests2-6746b87^1-6746b87-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-6f76e766b^1^1^1^1^1^1^1^1^1-6f76e766b/libtiff-tests2-6f76e766b^1^1^1^1^1^1^1^1^1-6f76e766b-workdir",))) # 21

# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-c766cb7^1-c766cb7/libtiff-tests2-c766cb7^1-c766cb7-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-ccc10c7-ccc10c7^2/libtiff-tests2-ccc10c7-ccc10c7^2-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-ce4b7af^1-ce4b7af/libtiff-tests2-ce4b7af^1-ce4b7af-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-cf05a83-cf05a83^1^1^1/libtiff-tests2-cf05a83-cf05a83^1^1^1-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-e6d0c32/libtiff-tests2-e6d0c32-workdir",))) # 26

# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-e8a47d4^1-e8a47d4/libtiff-tests2-e8a47d4^1-e8a47d4-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-eec4c06^1-eec4c06/libtiff-tests2-eec4c06^1-eec4c06-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/libtiff/libtiff-case-tests2-f2d989d/libtiff-tests2-f2d989d-workdir",))) # 29

# # lighttpd
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/lighttpd/lighttpd-case-1795/lighttpd-1795-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/lighttpd/lighttpd-case-1807/lighttpd-1807-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/lighttpd/lighttpd-case-1914/lighttpd-1914-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/lighttpd/lighttpd-case-1949/lighttpd-1949-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/lighttpd/lighttpd-case-2226/lighttpd-2226-workdir",)))

# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/lighttpd/lighttpd-case-2259/lighttpd-2259-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/lighttpd/lighttpd-case-2331/lighttpd-2331-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/lighttpd/lighttpd-case-2662/lighttpd-2662-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/lighttpd/lighttpd-case-2786/lighttpd-2786-workdir",)))

# # php
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-09273098521913a/php-09273098521913a-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-09b990f/php-09b990f-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-1056c57f/php-1056c57f-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-109b8e9/php-109b8e9-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-118695a/php-118695a-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-187eb23/php-187eb23-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-1a8b87d/php-1a8b87d-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-1d984a7/php-1d984a7-workdir",))) # Correct

# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-1e91069/php-1e91069-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-1f49902/php-1f49902-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-2a6968/php-2a6968-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-2adf58/php-2adf58-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-2e206e07/php-2e206e07-workdir",)))

# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-2e25ec9/php-2e25ec9-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-3571c95/php-3571c95-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-3acdca/php-3acdca-workdir",))) # Correct
# # result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-453c954^1-453c954/php-453c954^1-453c954-workdir",))) # Correct?
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-4756d7/php-4756d7-workdir",))) # 44

# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-478e5d1/php-478e5d1-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-5a8c917/php-5a8c917-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-5bb0a44/php-5bb0a44-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-6f3148d/php-6f3148d-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-77ed819/php-77ed819-workdir",)))

# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-7f29372/php-7f29372-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-821d716/php-821d716-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-8ba00176/php-8ba00176-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-8deb11c/php-8deb11c-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-93f65cd/php-93f65cd-workdir",))) # 54

# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-991ba131/php-991ba131-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-9bbc114/php-9bbc114-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-a3a5157/php-a3a5157-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-b83e24/php-b83e24-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-b84967d/php-b84967d-workdir",))) # Correct

# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-b9b1fb1/php-b9b1fb1-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-c2fe893/php-c2fe893-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-cebe2a213^1-cebe2a213/php-cebe2a213^1-cebe2a213-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-d1d61ce/php-d1d61ce-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-ecb9d80/php-ecb9d80-workdir",))) # 64

# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-ee83270/php-ee83270-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-efcb9a71/php-efcb9a71-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-f2329f1/php-f2329f1-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-f455f8^1-f455f8/php-f455f8^1-f455f8-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/php/php-case-f7b7b6aa9/php-f7b7b6aa9-workdir",))) # 69

# # python
# # result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/python/python-case-69224/python-69224-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/python/python-case-69372-69368/python-69372-69368-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/python/python-case-69474-69470/python-69474-69470-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/python/python-case-69710/python-69710-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/python/python-case-69784/python-69784-workdir",)))

# # result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/python/python-case-69935/python-69935-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/python/python-case-69946/python-69946-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/python/python-case-70023-70019/python-70023-70019-workdir",)))
# # result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/python/python-case-70059-70056/python-70059-70056-workdir",))) # Correct
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/python/python-case-70101-70098/python-70101-70098-workdir",)))

# # wireshark
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/wireshark/wireshark-case-2d2815b^1-2d2815b/wireshark-2d2815b^1-2d2815b-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/wireshark/wireshark-case-9a2ba01/wireshark-9a2ba01-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/wireshark/wireshark-case-464486e^1-464486e/wireshark-464486e^1-464486e-workdir",)))
# result.append(pool.apply_async(run,("/root/project/MSV-experiment/benchmarks/wireshark/wireshark-case-b51eba6/wireshark-b51eba6-workdir",)))

pool.close()
pool.join()