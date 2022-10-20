#!/usr/bin/env python3

from sys import argv
from Levenshtein import distance

file1=argv[0]
file2=argv[1]

f1=open(file1,'r')
str1=f1.read()
f1.close()

f2=open(file2,'r')
str2=f2.read()
f2.close()

print(str(distance(str1,str2)))