#!/usr/bin/env python
import os
import sys
import subprocess


def main(argv):
    print("run-afl.py!!!")
    arg_dict = dict()
    for i in range(len(argv)):
        if argv[i] == "-w":
            arg_dict["w"] = argv[i + 1]
        elif argv[i] == "-t":
            arg_dict["t"] = argv[i + 1]
    conf_dict = dict()
    with open(os.path.join(arg_dict["w"], "repair.conf")) as conf_file:
        for line in conf_file.readlines():
            line = line.strip()
            key = line.split("=")[0]
            value = line.split("=")[1]
            conf_dict[key] = value
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
    os.chdir(os.path.join(arg_dict["w"], "src"))
    afl_cmd = ["afl-fuzz", "-o", "out", "-m", "none", "-d", "-n", "-t", str(timeout * 1000), "-w", arg_dict["w"]]
    php_cmd = ["--", "timeout", str(timeout - 1), "./sapi/cli/php", "./run-tests.php", "-p", "./sapi/cli/php"]
    for test in neg_test:
        os.system("rm -rf ./out")
        test_cmd = afl_cmd + php_cmd + \
            [os.path.join(arg_dict["w"], "tests", "{0:05d}.phpt".format(test))]
        print(test_cmd)
        p = subprocess.Popen(test_cmd, stdout=subprocess.PIPE)
        (out, err) = p.communicate()
        if out == None:
            print("none")
            out = ""
        print("out:")
        print(out)
        lines = out.split("\n")
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
                    print("PASS!")
                elif (tokens[0] == "Fatal") or (tokens[0] == "FAIL"):
                    exit(1)
    exit(0)


if __name__ == "__main__":
    main(sys.argv)
