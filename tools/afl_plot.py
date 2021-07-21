#!/usr/bin/env python3
import os
import sys
import getopt
import matplotlib.pyplot as plt


def afl_plot(in_file: str, out_file: str, strategy: str, ignore_iteration: bool = False) -> None:
    iter_list = list()
    result_list = list()
    index = 0
    with open(in_file, "r") as csv:
        total = 0
        for line in csv.readlines():
            tokens = line.strip().split(",")
            iter = int(tokens[0])
            sw = int(tokens[1])
            cs = int(tokens[2])
            result = tokens[3] == "1"                
            if result:
                total += 1
            if ignore_iteration:
                iter_list.append(index)
                index += 1
            else:
                iter_list.append(iter)
            result_list.append(total)

    print(f"total {total}")
    plt.plot(iter_list, result_list, 'r')
    plt.title(strategy)
    plt.xlabel("iteration")
    plt.ylabel("pass")
    if len(out_file) == 0:
        out_file = os.path.join(os.path.dirname(in_file), "plot.png")
    plt.savefig(out_file)


def main(argv):
    opts, args = getopt.getopt(argv[1:], "i:o:s:n")
    result_file = ""
    output_file = ""
    strategy = ""
    ignore = False
    for o, a in opts:
        if o == "-i":
            result_file = a
        elif o == "-o":
            output_file = a
        elif o == "-s":
            strategy = a
        elif o == "-n":
            ignore = True

    afl_plot(result_file, output_file, strategy, ignore)
    return 0



if __name__ == "__main__":
    main(sys.argv)
