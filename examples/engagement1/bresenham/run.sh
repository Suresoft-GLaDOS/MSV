#!/bin/sh
rm __fixed_*
msv -cpp -skip-verify -r . -consider-all -first-n-loc 200 -feature-para ../../../crawler/para-all.out
