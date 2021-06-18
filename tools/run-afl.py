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
    if parallel_count==1:
        afl_cmd = ["afl-fuzz", "-o", arg_dict['w']+"/out", "-m", "none", "-d", "-n", "-t",
               str(timeout * 1000), "-w", arg_dict["w"]]
        afl_cmd += ["--", conf_dict['tools_dir']+'/php-test.py', arg_dict['w']+'/src', arg_dict['w']+'/tests', arg_dict['w']]
        (fuzzer_out[0],fuzzer_err[0]) = subprocess.Popen(afl_cmd, stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    else:
        for i in range(0,parallel_count):
            afl_cmd = ["afl-fuzz", "-o", arg_dict['w']+"/out", "-m","none","-g","53","-d", "-n", "-t",
                str(timeout * 1000), "-w", arg_dict["w"]]
            if i==0:
                afl_cmd.append('-M')
            else:
                afl_cmd.append('-S')
            afl_cmd.append(fuzzer_name+str(i+1))
            afl_cmd += ["--", conf_dict['tools_dir']+'/php-test.py', arg_dict['w']+'/src', arg_dict['w']+'/tests', arg_dict['w']]
            print("Run afl-fuzz "+fuzzer_name+str(i+1))
            fuzzer.append(subprocess.Popen(afl_cmd, stdout=subprocess.PIPE,stderr=subprocess.PIPE))

            # Wait for init fuzzer
            time.sleep(2)

    for i in range(0,parallel_count):
        print('waiting for fuzzer '+str(i+1)+'...')
        (out,err)=fuzzer[i].communicate()
        if out == None:
            print("none")
            out = ""
        # print("out:")
        out=str(out,'utf-8')
        print(out)
        lines = out.splitlines()
        # print(lines)
        # lines=out.split(['\\n'])
        test_section = False
        for line in lines:
            tokens = line.split()
            if len(tokens) == 0:
                continue
            if (len(tokens) > 2) and (tokens[0] == "Running") and (tokens[1] == "selected") and (tokens[2] == "tests."):
                test_section = True
            elif (tokens[0][0:6] == "======") and (test_section == True):
                test_section = False
            elif (test_section == True):
                if (tokens[0] == "PASS") or ((len(tokens) > 3) and tokens[3] == "PASS"):
                    print(str(i)+": PASS!")
                elif (tokens[0] == "Fatal") or (tokens[0] == "FAIL"):
                    print(str(i)+": Fail!")
    exit(0)


if __name__ == "__main__":
    main(sys.argv)
