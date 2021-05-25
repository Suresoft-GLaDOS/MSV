#!/usr/bin/env python
import os
import sys
import subprocess


def main(argv):
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

    os.chdir(os.path.join(arg_dict["w"], "src"))
    os.system("rm -rf ./out")
    afl_cmd = ["afl-fuzz", "-o", "out", "-m", "none", "-d", "-n", "-t", arg_dict["t"], "-w", arg_dict["w"],
               "--", "./sapi/cli/php", "./run-tests.php", "-p", "./sapi/cli/php"]
    for test in neg_test:
        test_cmd = afl_cmd + ["{0:05d}.phpt".format(test)]
        print(test_cmd)
    exit(0)


if __name__ == "__main__":
    main(sys.argv)
