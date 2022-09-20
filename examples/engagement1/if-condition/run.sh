#!/bin/bash
# ../../../build/src/prophet run.conf -consider-all -first-n-loc 200 -feature-para ../../../crawler/para-all.out
prophet -r . -consider-all -first-n-loc 200 -feature-para ../../../crawler/para-rext-all.out -skip-verify -skip-profile -replace-ext -msv-ext
cat fixed_test.c
