#!/usr/bin/env python3

BENCHMARKS=[
"gmp-13421",
# "gmp-14167", # Build fail
"gzip-1204630c96e0c3",
"gzip-118a107f2d3ae5",
"gzip-39a362ae9d9b00",
"gzip-884ef6d16c6",
"gzip-f17cbd13a1d0a7",
"libtiff-tests-0d27dc0",
"libtiff-tests-1563270c^1-1563270c",
"libtiff-tests-5f42db",
"libtiff-tests-2e42d63f",
"libtiff-tests-6b6496",
"libtiff-tests-e0b51f3e",
# "libtiff-tests-bdba15c", # Meta-program fail
"libtiff-tests2-0a36d7f",
"libtiff-tests-eec7ec0",
"libtiff-tests2-207c78a4",
"libtiff-tests2-22da1d",
"libtiff-tests2-3edb9cd",
"libtiff-tests2-6f76e766b^1^1^1^1^1^1^1^1^1-6f76e766b",
"libtiff-tests2-371336d^1-371336d",
"libtiff-tests2-6746b87^1-6746b87",
"libtiff-tests2-c766cb7^1-c766cb7",
"libtiff-tests2-ccc10c7-ccc10c7^2",
"libtiff-tests2-e6d0c32",
"libtiff-tests2-ce4b7af^1-ce4b7af",
"libtiff-tests2-cf05a83-cf05a83^1^1^1",
"libtiff-tests2-e8a47d4^1-e8a47d4",
"libtiff-tests2-eec4c06^1-eec4c06",
"libtiff-tests2-f2d989d",
# "lighttpd-1795", # Meta-program fail
"lighttpd-1914",
"lighttpd-1807",
"lighttpd-1949",
"lighttpd-2226",
"lighttpd-2259",
"lighttpd-2331",
"lighttpd-2662",
# "lighttpd-2786", # Meta-program fail
"php-09273098521913a",
"php-09b990f",
"php-1056c57f",
"php-109b8e9",
"php-118695a",
"php-1d984a7",
"php-187eb23",
"php-1f49902",
"php-1a8b87d",
"php-2adf58",
"php-1e91069",
"php-2e206e07",
"php-3571c95",
"php-2e25ec9",
"php-2a6968",
"php-3acdca",
"php-453c954^1-453c954",
# "php-478e5d1", # No patch in search space
# "php-4756d7", # No patch in search space
"php-5a8c917",
"php-77ed819",
# "php-6f3148d", # No patch in search space
# "php-5bb0a44", # No patch in search space
"php-821d716",
"php-8ba00176",
"php-7f29372",
"php-8deb11c",
"php-991ba131",
"php-93f65cd",
"php-9bbc114",
# "php-a3a5157", # No patch in search space
"php-b84967d",
"php-b9b1fb1",
# "php-c2fe893", # No patch in search space
# "php-b83e24", # No patch in search space
"php-d1d61ce",
"php-ecb9d80",
"php-ee83270",
# "php-cebe2a213^1-cebe2a213", # No patch in search space
"php-f2329f1",
"php-f455f8^1-f455f8",
"php-f7b7b6aa9",
"python-69474-69470",
"python-69372-69368",
# "python-69710", # No patch in search space
"python-69224",
"php-efcb9a71",
"python-69784",
"python-69935",
"python-70023-70019",
"python-69946",
# "python-70101-70098", # No patch in search space
"python-70059-70056",
"wireshark-2d2815b^1-2d2815b",
"wireshark-9a2ba01",
# "wireshark-b51eba6", # Meta-program fail
"wireshark-464486e^1-464486e",
"php-bug-2011-10-15-0a1cc5f01c-05c5c8958e",
"php-bug-2011-10-16-1f78177e2b-d4ae4e79db",
"php-bug-2011-11-01-ceac9dc490-9b0d73af1d",
"php-bug-2011-11-02-c1d520d19d-9b86852d6e",
"php-bug-2011-11-02-de50e98a07-8d520d6296",
"php-bug-2011-11-11-fcbfbea8d2-c1e510aea8",
"php-bug-2011-11-15-236120d80e-fb37f3b20d",
"php-bug-2011-11-19-51a4ae6576-bc810a443d",
"php-bug-2011-12-10-74343ca506-52c36e60c4",
"php-bug-2012-01-01-80dd931d40-7c3177e5ab",
"php-bug-2012-01-27-544e36dfff-acaf9c5227",
"php-bug-2012-02-08-ff63c09e6f-6672171672",
"php-bug-2012-03-04-60dfd64bf2-34fe62619d",
"php-bug-2012-03-08-0169020e49-cdc512afb3",
"php-bug-2012-03-11-3954743813-d4f05fbffc"
]

import subprocess
import os
import random
import logging

CONTAINER_NAME=f'msv-{random.randint()}'
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
    result=subprocess.run(['docker','run','-d','--rm','--name',CONTAINER_NAME,'kyj1411/msv:test'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
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
        LOGGER.info(f'run {benchmark}...')
        result=subprocess.run(['docker','exec',CONTAINER_NAME,f'python3 /root/project/MSV/scripts/run.py search {benchmark}'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        if result.returncode!=0:
            LOGGER.error(result.stdout.decode('utf-8'))
            LOGGER.error('Fail to run benchmark {}!'.format(benchmark))
            exit(1)
        else:
            outputs=result.stdout.decode('utf-8').splitlines()
            if 'not found' not in outputs[-1]:
                total_correct_patches+=1
            if 'not found' not in outputs[-2]:
                total_plausible_patches+=1

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
    LOGGER.info(f'Found correct patches: {correct}/{total} ({correct/total})')
    LOGGER.info(f'Found plausible patches: {plausible}/{total} ({plausible/total})')


if __name__=='__main__':
    preprocess()
    total,correct,plausible=run()
    postprocess(total,correct,plausible)