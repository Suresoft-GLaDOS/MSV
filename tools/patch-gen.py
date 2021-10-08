#!/usr/bin/env python3

import json

def get_rules(work_dir):
    switch_file=open(work_dir+'/switch-info.json',"r")
    root_object=json.load(switch_file)
    switch_file.close()
    rules=dict()

    rule_root=root_object['rules']
    for file in rule_root:
        for line in file['lines']:
            for switch in line['switches']:
                current_switch=switch['switch']
                current_cases=[]

                for type in switch['types']:
                    for case in type:
                        current_cases.append(case)
                
                rules[current_switch]=current_cases
    
    return rules

def put_patches(work_dir,rules):
    print(json.dumps(rules))
    new_object=dict()
    counter=1
    for k,v in rules.items():
        for case in v:
            new_config={'__SWITCH'+str(k):case}
            new_patch=[new_config]
            new_object[str(counter)]=new_patch
            counter+=1
    
    patch_file=open(work_dir+'/patch-info.json','w')
    json.dump(new_object,fp=patch_file,indent=2)
    patch_file.close()

import getopt
from sys import argv

if __name__=='__main__':
    opts,args=getopt.getopt(argv[1:],'')
    work_dir=args[0]
    rules=get_rules(work_dir)
    put_patches(work_dir,rules)
