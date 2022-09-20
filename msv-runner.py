#!/usr/bin/env python3

import os
import sys
from yaml import safe_load, dump
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper
import getopt
from sys import argv
import subprocess

def create_revlog(src_name:str,msv_path:str):
    os.chdir('src')
    vulcan_file=open('vulcan.yml','r')
    vulcan_object=safe_load(vulcan_file)
    vulcan_file.close()

    # Get test list or test case
    if 'test-list' in vulcan_object.keys():
        tests=vulcan_object['test-list'].splitlines()
        is_list=True
    else:
        tests=vulcan_object['test-case'].splitlines()
    
    total_test=len(tests)

    # Build project, for decide fail and pass test
    os.system(f'{msv_path}/tools/msv-build.py .')

    fail_test=[]
    pass_test=[]
    # Run test
    for i in range(total_test):
        print(f'{msv_path}/tools/msv-test.py','.','../tests','..',f'{i}')
        proc=subprocess.run([f'{msv_path}/tools/msv-test.py','.','../tests','..',f'{i}'],stdout=subprocess.PIPE)
        try:
            output=proc.stdout.decode('utf-8')
            if f'{i}' in output and proc.returncode==0:
                pass_test.append(i)
            else:
                fail_test.append(i)
        except:
            fail_test.append(i)

    # Save revlog with fail and pass test
    os.chdir('..')
    revlog_file=open(f'{src_name}.revlog','w')
    revlog_file.write('-\n-\n')
    revlog_file.write(f'Diff Cases: Tot {len(fail_test)}\n')
    for fail in fail_test:
        revlog_file.write(f'{fail} ')
    revlog_file.write(f'\nPositive Cases: Tot {len(pass_test)}\n')
    for test in pass_test:
        revlog_file.write(f'{test} ')
    revlog_file.write('\nRegression Cases: Tot 0\n')
    revlog_file.close()

def create_repair_conf(work_dir:str,src_name:str,msv_path:str):
    revision_file=f'{work_dir}/{src_name}-workdir/{src_name}.revlog'
    src_dir=f'{work_dir}/{src_name}-workdir/{src_name}'
    test_dir=f'{work_dir}/{src_name}-workdir/tests'
    build_cmd=f'{msv_path}/tools/msv-build.py'
    test_cmd=f'{msv_path}/tools/msv-test.py'
    tools_dir=f'{msv_path}/tools'
    
    os.chdir('src')
    # Read vulcan.yml, for bugged_file
    vulcan_file=open('vulcan.yml','r')
    vulcan_object=safe_load(vulcan_file)
    vulcan_file.close()
    os.chdir('..')

    bugged_file=vulcan_object['test-candidates'].splitlines()[0]
    single_case_timeout='60'
    profile='profile'
    fixed_out_file='fixed_'

    with open('repair.conf','w') as file:
        file.write('revision_file='+revision_file+'\n')
        file.write('src_dir='+src_dir+'\n')
        file.write('test_dir='+test_dir+'\n')
        file.write('build_cmd='+build_cmd+'\n')
        file.write('test_cmd='+test_cmd+'\n')
        file.write('tools_dir='+tools_dir+'\n')
        file.write('localizer='+profile+'\n')
        file.write('bugged_file='+bugged_file+'\n')
        file.write('fixed_out_file='+fixed_out_file+'\n')
        file.write('single_case_timeout='+single_case_timeout+'\n')
    

def create_dir_structure(src_dir:str,work_dir:str,msv_path:str):
    """
        Generate MSV's directory structure.
        Generate {src_dir}-workdir in {work_dir}/.

        src_dir: root source(project) directory
        work_dir: directory that want to generate workdir
    """
    if not os.path.isabs(src_dir):
        src_dir=os.path.abspath(src_dir)
    if not os.path.isabs(work_dir):
        work_dir=os.path.abspath(work_dir)
    if not os.path.isabs(msv_path):
        msv_path=os.path.abspath(msv_path)

    # Create root workdir
    src_name=src_dir.split('/')[-1]
    orig_dir=os.getcwd()
    os.chdir(work_dir)
    if os.path.isdir(f'{src_name}-workdir'):
        os.system(f'rm -rf {src_name}-workdir')
    os.mkdir(f'{src_name}-workdir')
    os.system(f'cp -rf {src_dir} {src_name}-workdir')

    # Set directories
    os.chdir(f'{src_name}-workdir')
    os.system(f'cp -rf {src_name} src')
    os.mkdir('tests') # Not necessary, but for tester script

    create_revlog(src_name,msv_path)
    create_repair_conf(work_dir,src_name,msv_path)

    os.chdir(orig_dir)

def generate_meta_program(work_dir:str,src_dir:str,feature_para:str='',sbfl_path:str=''):
    if not os.path.isabs(src_dir):
        src_dir=os.path.abspath(src_dir)
    if not os.path.isabs(work_dir):
        work_dir=os.path.abspath(work_dir)
    src_name=src_dir.split('/')[-1]

    cmd=['prophet', '-r', f'{work_dir}/{src_name}-workdir', '-skip-verify', '-skip-profile', '-replace-ext', '-first-n-loc', '14', '-consider-all','-msv-ext']
    if sbfl_path!='':
        cmd.append('-use-sbfl')
        cmd.append(sbfl_path)
    proc=subprocess.run(cmd,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)

    output_file=open(f'{work_dir}/output.log','w')
    output_file.write('stdout:\n')
    try:
        output=proc.stdout.decode('utf-8')
        output_file.write(output+'\n')
    except:
        output_file.write(proc.stdout+'\n')

    output_file.close()

    if proc.returncode!=0:
        print(f'Meta-program generation failed with return code: {proc.returncode}',file=sys.stderr)
        print(f'Look {work_dir}/output.log for more details',file=sys.stderr)
        sys.exit(proc.returncode)


if __name__=='__main__':
    opts, args = getopt.getopt(argv[1:], "hrs:")
    run_prophet=False
    sbfl_path=''
    for o, a in opts:
        if o == "-h":
            print('usage: msv-runner.py <src_dir> <work_dir> <msv_path>')
            exit(0)
        elif o=='-r':
            run_prophet=True
        elif o=='-s':
            sbfl_path=a

    src_dir = args[0]
    work_dir = args[1]
    msv_path = args[2]

    create_dir_structure(src_dir,work_dir,msv_path)

    if run_prophet:
        generate_meta_program(work_dir,src_dir,sbfl_path=sbfl_path)