#!/usr/bin/env python3
import os
import sys
import getopt
import matplotlib.pyplot as plt
from matplotlib.transforms import Bbox


def afl_plot(in_file: str, title: str, out_file: str = '', ignore_iteration: bool = False,
             red: str = "prophet", blue: str = "random", green: str = "guided",correct_patch=()) -> None:
    prophet_iter_list = list()
    random_iter_list = list()
    guided_iter_list = list()
    prophet_result_list = list()
    random_result_list = list()
    guided_result_list = list()
    index = 0

    max_success = 0  # Just for graph design
    with open(in_file+"/"+red+".csv", "r") as csv:
        total = 0
        i = 0
        for line in csv.readlines():
            tokens = line.strip().split(",")
            iter = int(tokens[0])
            tm = int(tokens[1])
            sw = int(tokens[2])
            cs = int(tokens[3])
            result = tokens[4] == "1"
            if len(tokens)==6:
                oper=tokens[5]
            elif len(tokens)==8:
                oper=tokens[5]
                var=tokens[6]
                const=tokens[7]
            else:
                oper=None
            
            if result:
                total += 1
                max_success += 1
            if ignore_iteration:
                prophet_iter_list.append(index)
                index += 1
            else:
                prophet_iter_list.append(i)
                i += 1
            prophet_result_list.append(total)

            prophet_correct=[]
            if len(correct_patch)==2 and oper==None and correct_patch[0]==sw and correct_patch[1]==cs:
                prophet_correct.append(prophet_iter_list[len(prophet_iter_list)-1])
                prophet_correct.append(total)
            elif len(correct_patch)==3 and oper==4 and correct_patch[0]==sw and correct_patch[1]==cs:
                prophet_correct.append(prophet_iter_list[len(prophet_iter_list)-1])
                prophet_correct.append(total)
            elif len(correct_patch)>3 and correct_patch[0]==sw and correct_patch[1]==cs and correct_patch[2]==oper and correct_patch[3]==var and correct_patch[4]==const:
                prophet_correct.append(prophet_iter_list[len(prophet_iter_list)-1])
                prophet_correct.append(total)

        csv.close()

    temp_success = 0
    with open(in_file+"/"+blue+".csv", "r") as csv:
        total = 0
        i = 0
        for line in csv.readlines():
            tokens = line.strip().split(",")
            iter = int(tokens[0])
            tm = int(tokens[1])
            sw = int(tokens[2])
            cs = int(tokens[3])
            result = tokens[4] == "1"
            if len(tokens)==6:
                oper=tokens[5]
            elif len(tokens)==8:
                oper=tokens[5]
                var=tokens[6]
                const=tokens[7]
            else:
                oper=None

            if result:
                total += 1
                temp_success += 1
            if ignore_iteration:
                random_iter_list.append(index)
                index += 1
            else:
                random_iter_list.append(i)
                i += 1
            random_result_list.append(total)

            random_correct=[]
            if len(correct_patch)==2 and oper==None and correct_patch[0]==sw and correct_patch[1]==cs:
                random_correct.append(random_iter_list[len(random_iter_list)-1])
                random_correct.append(total)
            elif len(correct_patch)==3 and oper==4 and correct_patch[0]==sw and correct_patch[1]==cs:
                random_correct.append(random_iter_list[len(random_iter_list)-1])
                random_correct.append(total)
            elif len(correct_patch)>3 and correct_patch[0]==sw and correct_patch[1]==cs and correct_patch[2]==oper and correct_patch[3]==var and correct_patch[4]==const:
                random_correct.append(random_iter_list[len(random_iter_list)-1])
                random_correct.append(total)

        csv.close()
    if max_success < temp_success:
        max_success = temp_success

    temp_success = 0
    with open(in_file+"/"+green+".csv", "r") as csv:
        total = 0
        i = 0
        for line in csv.readlines():
            tokens = line.strip().split(",")
            iter = int(tokens[0])
            tm = int(tokens[1])
            sw = int(tokens[2])
            cs = int(tokens[3])
            result = tokens[4] == "1"
            if len(tokens)==6:
                oper=tokens[5]
            elif len(tokens)==8:
                oper=tokens[5]
                var=tokens[6]
                const=tokens[7]
            else:
                oper=None

            if result:
                total += 1
                temp_success += 1
            if ignore_iteration:
                guided_iter_list.append(index)
                index += 1
            else:
                guided_iter_list.append(i)
                i += 1
            guided_result_list.append(total)

            guided_correct=[]
            if len(correct_patch)==2 and oper==None and correct_patch[0]==sw and correct_patch[1]==cs:
                guided_correct.append(guided_iter_list[len(guided_iter_list)-1])
                guided_correct.append(total)
            elif len(correct_patch)==3 and oper==4 and correct_patch[0]==sw and correct_patch[1]==cs:
                guided_correct.append(guided_iter_list[len(guided_iter_list)-1])
                guided_correct.append(total)
            elif len(correct_patch)>3 and correct_patch[0]==sw and correct_patch[1]==cs and correct_patch[2]==oper and correct_patch[3]==var and correct_patch[4]==const:
                guided_correct.append(guided_iter_list[len(guided_iter_list)-1])
                guided_correct.append(total)

        csv.close()
    if max_success < temp_success:
        max_success = temp_success

    print(f"total {total}")
    plt.plot(prophet_iter_list, prophet_result_list, 'r', random_iter_list,
             random_result_list, 'b', guided_iter_list, guided_result_list, 'g',[prophet_correct[0]],[prophet_correct[1]],'r*',
             [random_correct[0]],[random_correct[1]],'b*',[guided_correct[0]],[guided_correct[1]],'g*',)
    plt.title(title)
    plt.xlabel("iteration")
    plt.ylabel("pass")
    plt.text(0, max_success, 'Red: '+red+'\nBlue: '+blue +
             '\nGreen: '+green, bbox={'facecolor': 'white'})
    plt.grid(True)
    if len(out_file) == 0:
        out_file = os.path.join(os.path.dirname(in_file), "plot.png")
    plt.savefig(out_file)


def afl_plot_one(in_file: str, title: str, out_file: str, ignore_iteration: bool = False,correct_patch=()) -> None:

    iter_list = list()
    result_list = list()
    index = 0
    plt.clf()

    with open(in_file, "r") as csv:
        total = 0
        for line in csv.readlines():
            tokens = line.strip().split(",")
            iter = int(tokens[0])
            tm = int(tokens[1])
            sw = int(tokens[2])
            cs = int(tokens[3])
            result = tokens[4] == "1"
            if len(tokens)==6:
                oper=tokens[5]
            elif len(tokens)==8:
                oper=tokens[5]
                var=tokens[6]
                const=tokens[7]
            else:
                oper=None

            if result:
                total += 1
            if ignore_iteration:
                iter_list.append(index)
                index += 1
            else:
                iter_list.append(iter)
            result_list.append(total)

            prophet_correct=[]
            if len(correct_patch)==2 and oper==None and correct_patch[0]==sw and correct_patch[1]==cs:
                prophet_correct.append(iter_list[len(iter_list)-1])
                prophet_correct.append(total)
            elif len(correct_patch)==3 and oper==4 and correct_patch[0]==sw and correct_patch[1]==cs:
                prophet_correct.append(iter_list[len(iter_list)-1])
                prophet_correct.append(total)
            elif len(correct_patch)>3 and correct_patch[0]==sw and correct_patch[1]==cs and correct_patch[2]==oper and correct_patch[3]==var and correct_patch[4]==const:
                prophet_correct.append(iter_list[len(iter_list)-1])
                prophet_correct.append(total)

        csv.close()

    print(f"total {total}")
    plt.plot(iter_list, result_list, 'r',[prophet_correct[0]],[prophet_correct[1]],'r*')

    plt.title(title)
    plt.xlabel("iteration")
    plt.ylabel("pass")
    plt.grid(True)
    if len(out_file) == 0:
        out_file = os.path.join(os.path.dirname(in_file), "plot.png")
    plt.savefig(out_file)


def afl_plot_two(in_file: str, title: str, out_file: str = '', ignore_iteration: bool = False,
             red: str = "prophet", blue: str = "random", correct_patch=()) -> None:
    prophet_iter_list = list()
    random_iter_list = list()
    prophet_result_list = list()
    random_result_list = list()
    index = 0

    max_success = 0  # Just for graph design
    with open(in_file+"/"+red+".csv", "r") as csv:
        total = 0
        i = 0
        for line in csv.readlines():
            tokens = line.strip().split(",")
            iter = int(tokens[0])
            tm = int(tokens[1])
            sw = int(tokens[2])
            cs = int(tokens[3])
            result = tokens[4] == "1"
            if len(tokens)==6:
                oper=tokens[5]
            elif len(tokens)==8:
                oper=tokens[5]
                var=tokens[6]
                const=tokens[7]
            else:
                oper=None
            
            if result:
                total += 1
                max_success += 1
            if ignore_iteration:
                prophet_iter_list.append(index)
                index += 1
            else:
                prophet_iter_list.append(i)
                i += 1
            prophet_result_list.append(total)

            prophet_correct=[]
            if len(correct_patch)==2 and oper==None and correct_patch[0]==sw and correct_patch[1]==cs:
                prophet_correct.append(prophet_iter_list[len(prophet_iter_list)-1])
                prophet_correct.append(total)
            elif len(correct_patch)==3 and oper==4 and correct_patch[0]==sw and correct_patch[1]==cs:
                prophet_correct.append(prophet_iter_list[len(prophet_iter_list)-1])
                prophet_correct.append(total)
            elif len(correct_patch)>3 and correct_patch[0]==sw and correct_patch[1]==cs and correct_patch[2]==oper and correct_patch[3]==var and correct_patch[4]==const:
                prophet_correct.append(prophet_iter_list[len(prophet_iter_list)-1])
                prophet_correct.append(total)

        csv.close()

    temp_success = 0
    with open(in_file+"/"+blue+".csv", "r") as csv:
        total = 0
        i = 0
        for line in csv.readlines():
            tokens = line.strip().split(",")
            iter = int(tokens[0])
            tm = int(tokens[1])
            sw = int(tokens[2])
            cs = int(tokens[3])
            result = tokens[4] == "1"
            if len(tokens)==6:
                oper=tokens[5]
            elif len(tokens)==8:
                oper=tokens[5]
                var=tokens[6]
                const=tokens[7]
            else:
                oper=None

            if result:
                total += 1
                temp_success += 1
            if ignore_iteration:
                random_iter_list.append(index)
                index += 1
            else:
                random_iter_list.append(i)
                i += 1
            random_result_list.append(total)

            random_correct=[]
            if len(correct_patch)==2 and oper==None and correct_patch[0]==sw and correct_patch[1]==cs:
                random_correct.append(random_iter_list[len(random_iter_list)-1])
                random_correct.append(total)
            elif len(correct_patch)==3 and oper==4 and correct_patch[0]==sw and correct_patch[1]==cs:
                random_correct.append(random_iter_list[len(random_iter_list)-1])
                random_correct.append(total)
            elif len(correct_patch)>3 and correct_patch[0]==sw and correct_patch[1]==cs and correct_patch[2]==oper and correct_patch[3]==var and correct_patch[4]==const:
                random_correct.append(random_iter_list[len(random_iter_list)-1])
                random_correct.append(total)

        csv.close()
    if max_success < temp_success:
        max_success = temp_success

    print(f"total {total}")
    plt.plot(prophet_iter_list, prophet_result_list, 'r', random_iter_list,
             random_result_list, 'b',[prophet_correct[0]],[prophet_correct[1]],'r*',
             [random_correct[0]],[random_correct[1]],'b*')
    plt.title(title)
    plt.xlabel("iteration")
    plt.ylabel("pass")
    plt.text(0, max_success, 'Red: '+red+'\nBlue: '+blue, bbox={'facecolor': 'white'})
    plt.grid(True)
    if len(out_file) == 0:
        out_file = os.path.join(os.path.dirname(in_file), "plot.png")
    plt.savefig(out_file)


def afl_barchart(in_file: str, title: str, out_file: str, ignore_iteration: bool = False) -> None:
    iter_list = list()
    result_list = list()
    switch_list = list(range(81))
    pass_list = list()
    fail_list = list()
    plt.clf()
    for sw in switch_list:
        pass_list.append(0)
        fail_list.append(0)
    index = 0
    with open(in_file, "r") as csv:
        total = 0
        for line in csv.readlines():
            tokens = line.strip().split(",")
            iter = int(tokens[0])
            tm = int(tokens[1])
            sw = int(tokens[2])
            cs = int(tokens[3])
            result = tokens[4] == "1"
            if len(tokens)==6:
                oper=tokens[5]
            elif len(tokens)==8:
                oper=tokens[5]
                var=tokens[6]
                const=tokens[7]
            else:
                oper=None

            if result:
                pass_list[sw] += 1
                total += 1
            else:
                fail_list[sw] += 1
            if ignore_iteration:
                iter_list.append(index)
                index += 1
            else:
                iter_list.append(iter)
            result_list.append(total)
        csv.close()
    print(f"total {total}")
    plt.bar(switch_list, pass_list, color="b")
    plt.bar(switch_list, fail_list, color="r", bottom=pass_list)
    plt.title(title)
    plt.xlabel("switch")
    plt.ylabel("pass(blue)/fail(red)")
    if len(out_file) == 0:
        out_file = os.path.join(os.path.dirname(in_file), "plot.png")
    plt.savefig(out_file)


def get_average(in_dir: str, out_file: str) -> None:
    for id in os.listdir(in_dir):
        if os.path.isfile(id):
            with open(id, "r") as csv:
                lines = csv.readlines()


def main(argv):
    opts, args = getopt.getopt(argv[1:], "hi:o:t:nm:")
    result_file = ""
    output_file = ""
    title = ""
    ignore = False
    mode = ""
    for o, a in opts:
        if o == "-i":
            result_file = a
        elif o == "-o":
            output_file = a
        elif o == "-t":
            title = a
        elif o == "-n":
            ignore = True
        elif o == "-m":
            mode = a
        else:
            print("afl_plot.py -i input_dir -t title -o output.png -m mode")
            exit(0)

    if mode == "one":
        afl_plot_one(result_file, title, output_file, ignore)
    elif mode == "bar":
        afl_barchart(result_file, title, output_file, ignore)
    else:
        afl_plot(result_file, title, output_file, ignore)
    return 0


if __name__ == "__main__":
    main(sys.argv)
