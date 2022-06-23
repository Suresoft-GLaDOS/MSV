from sys import argv
import benchmarks
import download
from getopt import getopt
import subprocess

COMMANDS=('list','checkout','search')
MAIN_HELP="""Usage: python3 run.py <command> [args]
<command>: list, checkout or search
list: print list of all versions.
checkout: download and generate meta-program.
search: search generated meta-program, find patches."""

CHECKOUT_HELP="""Usage: python3 run.py checkout <benchmark|all>
Download benchmark and generate meta-program.
<benchmark|all>: benchmark to checkout. If you use 'all', checkout all benchmarks.
NOTE: checkout all may need a lot of times to download!"""

SEARCH_HELP="""Usage: python3 run.py search <benchmark|all>
Search generated meta-program.
Run searching with timeout 6 hour.
<benchmark|all>: benchmark to search. If you use 'all', search all benchmarks.
NOTE: search all may need a lot of times to run!

List of correct patches are in correct-patch.csv.
In second column, patch id is saved. Format is: switch-case[:operator[-variable-constant]].

Result of search is saved in /root/project/MSV-experiment/<version>-out.
Result is saved in <version>-out/msv-result.json.
pass_result: result of all tests. If value is true, it is plausible patch.
result: result of failing tests. If value is true, it passed failing tests.
config: information of patch. Map with correct-patch.csv above."""

def handle_list(is_help:bool=False):
    if is_help:
        print("""Print list of benchmarks with their indices.
List of benchmarks are saved in benchmarks.py.""")
    else:
        for i,version in enumerate(benchmarks.BENCHMARKS):
            print(f'{i}: {version}')

def handle_checkout(version:str):
    if version=='all':
        for i,v in enumerate(benchmarks.BENCHMARKS):
            download.download(i,v)
        return

    index=-1
    for i,v in enumerate(benchmarks.BENCHMARKS):
        if version==v:
            index=i
            break
    
    if index==-1:
        print(f'Unknown benchmark: {version}')
        print('Run "python3 run.py list" to get list of all benchmarks.')
        return
    
    download.download(index,version)

def handle_search(version:str):
    if version=='all':
        for i,v in enumerate(benchmarks.BENCHMARKS):
            result=subprocess.run(['python3','/root/project/MSV-search/msv-search.py',"-o",f"/root/project/MSV-experiment/{v}-out","-t","180000","-w",f"/root/project/MSV-experiment/{benchmarks.get_subject(v)}/{benchmarks.get_workdir(v)}","-p","/root/project/MSV","--use-pass-test","--use-exp-alpha", "--use-prophet-score","-T","21600",
                        "-m","guided","--",f"/root/project/MSV/tools/{benchmarks.get_subject(v)}-test.py",
                        f"/root/project/MSV-experiment/{benchmarks.get_subject(v)}/{benchmarks.get_workdir(v)}/src", f'{benchmarks.get_test_dir(v)}', f"/root/project/MSV-experiment/{benchmarks.get_subject(v)}/{benchmarks.get_workdir(v)}"],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
            if result.returncode!=0:
                print(result.stdout.decode('utf-8'))
                exit(1)
                
        return

    index=-1
    for i,v in enumerate(benchmarks.BENCHMARKS):
        if version==v:
            index=i
            break
    
    if index==-1:
        print(f'Unknown benchmark: {version}')
        print('Run "python3 run.py list" to get list of all benchmarks.')
        return
    
    result=subprocess.run(['python3','/root/project/MSV-search/msv-search.py',"-o",f"/root/project/MSV-experiment/{version}-out","-t","180000","-w",f"/root/project/MSV-experiment/{benchmarks.get_subject(version)}/{benchmarks.get_workdir(version)}","-p","/root/project/MSV","--use-pass-test","--use-exp-alpha", "--use-prophet-score","-T","21600",
                "-m","guided","--",f"/root/project/MSV/tools/{benchmarks.get_subject(version)}-test.py",
                f"/root/project/MSV-experiment/{benchmarks.get_subject(version)}/{benchmarks.get_workdir(version)}/src", f'{benchmarks.get_test_dir(version)}', f"/root/project/MSV-experiment/{benchmarks.get_subject(version)}/{benchmarks.get_workdir(version)}"],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if result.returncode!=0:
        print(result.stdout.decode('utf-8'))
        exit(1)

if __name__=='__main__':
    opts,args=getopt(argv[1:],'h',['help'])

    is_help=False
    for o,a in opts:
        if o=='-h' or o=='--help':
            is_help=True

    if len(args)==0:
        print(MAIN_HELP)
        exit(0)
    elif args[0] not in COMMANDS:
        print(f'Unknown command: {args[0]}')
        print(MAIN_HELP)
        exit(1)
    elif args[0]=='list':
        handle_list(is_help)
    elif args[0]=='checkout':
        if len(args)==1 or is_help:
            print(CHECKOUT_HELP)
        else:
            handle_checkout(args[1])
    elif args[0]=='search':
        if len(args)==1 or is_help:
            print(SEARCH_HELP)
        else:
            handle_search(args[1])
