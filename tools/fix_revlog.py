#!/usr/bin/env python3
import os
import sys
import getopt


def remove_tests(revision_filename, original_result_filename, out_filename):
    neg_test = list()
    pos_test = list()
    with open(revision_filename, "r") as revision_file:
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
    pos_set = set(pos_test)
    with open(original_result_filename, "r") as original_result:
        rm_list = original_result.readlines()
        for rm in rm_list:
            tokens = rm.strip().split(",")
            if int(tokens[3]) == 0:
                target = int(tokens[2]) 
                if target in pos_set:
                    pos_set.remove(target)
    pos_test = list(pos_set)
    with open(out_filename, "w") as outfile:
        outfile.write("-\n-\n")
        outfile.write("Diff Cases: Tot " + str(len(neg_test)) + "\n")
        for nt in neg_test:
            outfile.write(str(nt) + " ")
        outfile.write("\n")
        outfile.write("Positive Cases: Tot " + str(len(pos_test)) + "\n")
        for pt in pos_test:
            outfile.write(str(pt) + " ")
        outfile.write("\n")
        outfile.write("Regression Cases: Tot 0\n")


def main(argv):
    opts, args = getopt.getopt(argv[1:], "hi:o:r:m:")
    revision_file = ""
    output_file = ""
    original_result_filename = ""
    mode = ""
    for o, a in opts:
        if o == "-i":
            revision_file = a
        elif o == "-o":
            output_file = a
        elif o == "-r":
            original_result_filename = a
        elif o == "-m":
            mode = a
        else:
            print("fix_revlog.py -i php-5a8c917.revlog -r out/original-result.csv -o out.revlog")
            exit(0)
    remove_tests(revision_file, original_result_filename, output_file)
    return 0


if __name__ == "__main__":
    main(sys.argv)
