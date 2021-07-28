#!/usr/bin/env python3
import os
import sys
import getopt
import matplotlib.pyplot as plt
from matplotlib.transforms import Bbox


def afl_plot(in_file: str, title: str, out_file: str='',ignore_iteration: bool = False,red="prophet",blue="random",green="guided") -> None:
    prophet_iter_list = list()
    random_iter_list = list()
    guided_iter_list = list()
    prophet_result_list = list()
    random_result_list=list()
    guided_result_list=list()
    index = 0

    max_success=0 # Just for graph design
    with open(in_file+"/"+red+".csv", "r") as csv:
        total = 0
        i=0
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
                prophet_iter_list.append(index)
                index += 1
            else:
                prophet_iter_list.append(i)
                i+=1
            prophet_result_list.append(total)
    
    temp_success=0
    with open(in_file+"/"+blue+".csv", "r") as csv:
        total = 0
        i=0
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
                random_iter_list.append(index)
                index += 1
            else:
                random_iter_list.append(i)
                i+=1
            random_result_list.append(total)
    if max_success<temp_success:
        max_success=temp_success

    temp_success=0
    with open(in_file+"/"+green+".csv", "r") as csv:
        total = 0
        i=0
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
                guided_iter_list.append(index)
                index += 1
            else:
                guided_iter_list.append(i)
                i+=1
            guided_result_list.append(total)
    if max_success<temp_success:
        max_success=temp_success

    print(f"total {total}")
    plt.plot(prophet_iter_list, prophet_result_list, 'r',random_iter_list,random_result_list,'b',guided_iter_list,guided_result_list,'g')
    plt.title(title)
    plt.xlabel("iteration")
    plt.ylabel("pass")
    plt.text(0,max_success,'Red: '+red+'\nBlue: '+blue+'\nGreen: '+green,bbox={'facecolor':'white'})
    plt.grid(True)
    if len(out_file) == 0:
        out_file = os.path.join(os.path.dirname(in_file), "plot.png")
    plt.savefig(out_file)


def get_average(in_dir: str, out_file: str) -> None:
    for id in os.listdir(in_dir):
        if os.path.isfile(id):
            with open(id, "r") as csv:
                lines = csv.readlines()


def main(argv):
    opts, args = getopt.getopt(argv[1:], "i:o:t:n")
    result_file = ""
    output_file = ""
    title = ""
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

    afl_plot(result_file, title, output_file, ignore)
    return 0



if __name__ == "__main__":
    main(sys.argv)
