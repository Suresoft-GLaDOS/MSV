#!/usr/bin/env python3

import sys

if len(sys.argv)!=4:
    print("Syntax: ./afl-demo.py <afl script> <work directory> <correct switch-case>")
    exit(1)

script=sys.argv[1]
work_dir=sys.argv[2]
correct_answer=sys.argv[3]

correct_switch=correct_answer.split("-")[0]
correct_case=correct_answer.split("-")[1]

import json
info_dir=work_dir+"/switch-info.json"
info_file=open(info_dir,"r")
info=json.load(info_file)
info_file.close()

switch_num=info["switch_num"]
case_num=info["case_num"]

import random
total_cases=0
for i in case_num:
    total_cases+=i

test_random=[]
for i in range(4):
    test_random.append(random.randint(0,total_cases-1))

test_cases=[]
for i in test_random:
    current=i
    index=0
    for j in case_num:
        if current<j:
            test_cases.append(str(index)+"-"+str(current))
            break
        else:
            index+=1
            current-=j
test_cases.append(str(correct_switch)+"-"+str(correct_case))
for i in test_cases:
    print(i)

