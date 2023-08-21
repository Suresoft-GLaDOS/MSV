#!/usr/bin/env python3

BENCHMARKS=[
"gmp-13421",
"gmp-14167", # Build fail

"gzip-118a107f2d3ae5",
"gzip-39a362ae9d9b00",
"gzip-884ef6d16c6", # correct
"gzip-f17cbd13a1d0a7", # correct

"libtiff-tests-1563270c^1-1563270c",
"libtiff-tests-2e42d63f", # correct
"libtiff-tests-bdba15c", # Meta-program fail
"libtiff-tests2-0a36d7f", # correct
"libtiff-tests-eec7ec0", # correct
"libtiff-tests2-3edb9cd", # correct
"libtiff-tests2-371336d^1-371336d", # correct
"libtiff-tests2-e8a47d4^1-e8a47d4", # correct
"libtiff-tests2-eec4c06^1-eec4c06",

"lighttpd-1914", # correct
"lighttpd-1807",
"lighttpd-1949",
"lighttpd-2226",
"lighttpd-2259",
"lighttpd-2662", # correct
"lighttpd-2786", # Meta-program fail

"php-09273098521913a", # correct
"php-09b990f",
"php-1056c57f", # correct
"php-109b8e9",
"php-1d984a7", # correct
"php-1f49902",
"php-1a8b87d",
"php-2adf58", # correct
"php-1e91069", # correct
"php-2e206e07",
"php-2e25ec9",
"php-2a6968",
"php-3acdca", # correct
"php-453c954^1-453c954", # correct
"php-4756d7", # No patch in search space
"php-5a8c917", # correct
"php-77ed819",
"php-5bb0a44", # No patch in search space
"php-821d716", # correct
"php-8ba00176", # correct
"php-8deb11c",
"php-991ba131",
"php-93f65cd",
"php-b84967d", # correct
"php-b9b1fb1",
"php-c2fe893", # No patch in search space
"php-d1d61ce",
"php-ecb9d80",
"php-ee83270", # correct
"php-cebe2a213^1-cebe2a213", # No patch in search space
"php-f2329f1",
"php-f455f8^1-f455f8", # correct
"php-efcb9a71", # correct

"python-69474-69470",
"python-69372-69368",
"python-69710", # No patch in search space
"python-69224",
"python-69935",
"python-70023-70019",
"python-70101-70098", # No patch in search space
"python-70059-70056",

"wireshark-2d2815b^1-2d2815b",
"wireshark-9a2ba01",
"wireshark-b51eba6", # Meta-program fail
"wireshark-464486e^1-464486e",

"libtiff-tests-8a184dc",
'libtiff-tests-740a6187',
'libtiff-tests-e779b6cd',
'wireshark-6d801a205',
'wireshark-03d7987b',
]

import subprocess
import os
import random
import logging

CONTAINER_NAME=f'msv-{random.randint(1,100000000)}'
TOTAL_BENCHMARKS=111
LOGGER=logging.Logger(__name__)

def preprocess():
    """
        Pull container from docker hub, and run container
    """
    LOGGER.info('run docker pull...')
    result=subprocess.run(['docker','pull','kyj1411/msv:test'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if result.returncode!=0:
        LOGGER.error(result.stdout.decode('utf-8'))
        LOGGER.error('Fail to pull docker image from docker hub!')
        exit(1)

    LOGGER.info('run docker run...')
    result=subprocess.run(['docker','run','-itd','--name',CONTAINER_NAME,'--restart','always','kyj1411/msv:test'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if result.returncode!=0:
        LOGGER.error(result.stdout.decode('utf-8'))
        LOGGER.error('Fail to run docker image!')
        exit(1)

def run():
    """
        Run each benchmark, return total correct patches
    """
    total_correct_patches=0
    total_plausible_patches=0
    for benchmark in BENCHMARKS:
        LOGGER.info(f'checkout {benchmark}...')
        result=subprocess.run(['docker','exec',CONTAINER_NAME,f'python3 /root/project/MSV/scripts/run.py checkout {benchmark}'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        if result.returncode!=0:
            LOGGER.error(result.stdout.decode('utf-8'))
            LOGGER.error('Fail to run benchmark {}!'.format(benchmark))
            exit(1)

        LOGGER.info(f'run {benchmark}...')
        result=subprocess.run(['docker','exec',CONTAINER_NAME,f'python3 /root/project/MSV/scripts/run.py search {benchmark}'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        if result.returncode!=0:
            LOGGER.error(result.stdout.decode('utf-8'))
            LOGGER.error('Fail to run benchmark {}!'.format(benchmark))
            exit(1)
        else:
            outputs=result.stdout.decode('utf-8').splitlines()
            if 'Correct patch found' in outputs[-1]:
                total_correct_patches+=1
            # total=outputs[-2].split(' ')[-1]
            # if total!='0':
            #     total_plausible_patches+=1

    LOGGER.info('All benchmarks executed!')
    return TOTAL_BENCHMARKS,total_correct_patches,total_plausible_patches

def postprocess(total,correct,plausible):
    """
        Print summary
    """
    LOGGER.info('run docker stop...')
    result=subprocess.run(['docker','stop',CONTAINER_NAME],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if result.returncode!=0:
        LOGGER.error(result.stdout.decode('utf-8'))
        LOGGER.error('Fail to stop docker container!')
        exit(1)
    LOGGER.info('run docker rm...')
    result=subprocess.run(['docker','rm',CONTAINER_NAME],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if result.returncode!=0:
        LOGGER.error(result.stdout.decode('utf-8'))
        LOGGER.error('Fail to remove docker container!')
        exit(1)
    
    LOGGER.info('======== Result Summary ========')
    LOGGER.info(f'Found correct patches: {correct}/{total} ({str(correct/total)[:5]}%)')
    # LOGGER.info(f'Found plausible patches: {plausible}/{total} ({str(plausible/total)[:5]}%)')


if __name__=='__main__':
    preprocess()
    total,correct,plausible=run()
    postprocess(total,correct,plausible)