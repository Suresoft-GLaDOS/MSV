#!/usr/bin/env python3
import os
import sys
import getopt
import matplotlib.pyplot as plt
from matplotlib.transforms import Bbox


def afl_plot(in_file: str, out_file: str, title: str, ignore_iteration: bool = False) -> None:
    iter_list = list()
    prophet_result_list = list()
    random_result_list=[]
    guided_result_list=[]
    index = 0

    max_success=0 # Just for graph design
    with open(in_file+"/prophet.csv", "r") as csv:
        total = 0
        for line in csv.readlines():
            tokens = line.strip().split(",")
            iter = int(tokens[0])
            sw = int(tokens[1])
            cs = int(tokens[2])
            result = tokens[3] == "1"                
            if result:
                total += 1
                max_success+=1
            if ignore_iteration:
                iter_list.append(index)
                index += 1
            else:
                iter_list.append(iter)
            prophet_result_list.append(total)
    
    temp_success=0
    with open(in_file+"/random.csv", "r") as csv:
        total = 0
        for line in csv.readlines():
            tokens = line.strip().split(",")
            iter = int(tokens[0])
            sw = int(tokens[1])
            cs = int(tokens[2])
            result = tokens[3] == "1"                
            if result:
                total += 1
                temp_success+=1
            if ignore_iteration:
                iter_list.append(index)
                index += 1
            else:
                iter_list.append(iter)
            random_result_list.append(total)
    if max_success<temp_success:
        max_success=temp_success

    max_success=0
    with open(in_file+"/guided.csv", "r") as csv:
        total = 0
        for line in csv.readlines():
            tokens = line.strip().split(",")
            iter = int(tokens[0])
            sw = int(tokens[1])
            cs = int(tokens[2])
            result = tokens[3] == "1"                
            if result:
                total += 1
                max_success+=1
            if ignore_iteration:
                iter_list.append(index)
                index += 1
            else:
                iter_list.append(iter)
            guided_result_list.append(total)
    if max_success<temp_success:
        max_success<temp_success

    print(f"total {total}")
    plt.plot(iter_list, prophet_result_list, 'r',iter_list,random_result_list,'b',iter_list,guided_result_list,'g')
    plt.title(title)
    plt.xlabel("iteration")
    plt.ylabel("pass")
    plt.text(0,max_success,'Red: prophet\nBlue: random\nGreen: guided',Bbox={'facecolor':'white'})
    plt.grid(True)
    if len(out_file) == 0:
        out_file = os.path.join(os.path.dirname(in_file), "plot.png")
    plt.savefig(out_file)


def main(argv):
    opts, args = getopt.getopt(argv[1:], "i:o:t:n")
    result_file = ""
    output_file = ""
    strategy = ""
    ignore = False
    for o, a in opts:
        if o == "-i":
            result_file = a
        elif o == "-o":
            output_file = a
        elif o == "-t":
            title = a
        elif o == "-n":
            ignore = True

    afl_plot(result_file, output_file, title, ignore)
    return 0



if __name__ == "__main__":
    main(sys.argv)
