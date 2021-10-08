#!/usr/bin/env python3
import os
import sys
import subprocess
import json
import time

def main(argv):
    print("run-afl.py!!!")
    arg_dict = dict()
    parallel_count=1
    for i in range(len(argv)):
        if argv[i] == "-w":
            arg_dict["w"] = argv[i + 1]
        elif argv[i] == "-t":
            arg_dict["t"] = argv[i + 1]
        elif argv[i]=='-j':
            parallel_count=int(argv[i+1])
        elif argv[i] == '-k':
            arg_dict['k'] = argv[i + 1]
    
    conf_dict = dict()
    with open(os.path.join(arg_dict["w"], "repair.conf")) as conf_file:
        for line in conf_file.readlines():
            line = line.strip()
            key = line.split("=")[0]
            value = line.split("=")[1]
            conf_dict[key] = value
    switch_json = dict()
    with open(os.path.join(arg_dict['w'], 'switch-info.json')) as json_file:
        switch_json = json.load(json_file)

    # switches = switch_json["case_cluster"]
    priority = switch_json["priority"]
    switch_num = switch_json["switch_num"]
    print(switch_num, type(switch_num))

    neg_test = list()
    pos_test = list()
    with open(conf_dict["revision_file"]) as revision_file:
        line = revision_file.readline()
        line = revision_file.readline()
        line = revision_file.readline()
        line = revision_file.readline()
        for test in line.strip().split():
            neg_test.append(int(test))
        line = revision_file.readline()
        line = revision_file.readline()
        for test in line.strip().split():
            pos_test.append(int(test))
    
    os.environ["AFL_SKIP_CPUFREQ"] = "1"
    os.environ["AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES"] = "1"
    os.environ["AFL_NO_UI"] = "1"
    os.environ["AFL_FAST_CAL"] = "1"
    timeout = (int(arg_dict["t"]) // 1000) + 1
    # os.chdir(os.path.join(arg_dict["w"], "src"))

    fuzzer_name='fuzzer'
    fuzzer=[]
    fuzzer_out=[]
    fuzzer_err=[]
    os.system("rm -rf "+arg_dict['w']+"/out")
    os.environ['__SWITCH53'] = '186'
    cmd_list = ['/root/project/prophet-gpl/build/php-case-efcb9a71/php-efcb9a71-workdir/src/sapi/cli/php',
        '/root/project/prophet-gpl/build/php-case-efcb9a71/php-efcb9a71-workdir/src/run-tests.php',
                '-p', '/root/project/prophet-gpl/build/php-case-efcb9a71/php-efcb9a71-workdir/src/sapi/cli/php',
                '/root/project/prophet-gpl/build/php-case-efcb9a71/php-efcb9a71-workdir/tests/01442.phpt'
        ]
    #cmd_list = ['/root/project/prophet-gpl/build/php-case-efcb9a71/php-efcb9a71-workdir/src/sapi/cli/php',
    #            '/root/project/prophet-gpl/build/php-case-efcb9a71/php-efcb9a71-workdir/tests/01442.php'
    #]
    if 'k' in arg_dict:
        prog_list = []
        for i in range(30):
            prog = subprocess.Popen(["/root/project/prophet-gpl/tools/php-test.py", "-s", "67", "-c", "53-186", ".", "../tests", "..",
                                     arg_dict['k']], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            #prog = subprocess.Popen(
            #    cmd_list, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            prog_list.append(prog)
            #time.sleep(0.001)
        for i in range(30):
            (out, err) = prog_list[i].communicate()
            print(i, "out:", str(out))
        exit(0)
    
    for p in pos_test:
        if p <= parallel_count:
            continue
        prog = subprocess.Popen(["/root/project/prophet-gpl/tools/php-test.py", "-s", "67", "-c", "53-186", "-i", "mavis", ".", "../tests", "..",
                         str(p)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        (out, err) = prog.communicate()
        print(p, "out:", str(out))
    exit(0)


if __name__ == "__main__":
    main(sys.argv)
