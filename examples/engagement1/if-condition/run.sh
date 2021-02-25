#!/bin/bash
rm __fixed_test.c
prophet run.conf  -r . -consider-all -first-n-loc 200 -feature-para ../../../crawler/para-all.out

