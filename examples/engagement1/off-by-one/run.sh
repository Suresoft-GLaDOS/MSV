#!/bin/sh
# ../../../build/src/prophet run.conf -consider-all -first-n-loc 200 -feature-para ../../../crawler/para-all.out
msv -r . -consider-all -first-n-loc 200 -feature-para ../../../crawler/para-rext-all.out -skip-verify -skip-profile -replace-ext -msv-ext
cat fixed_prog.c
