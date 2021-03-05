#!/bin/bash
rm __fixed_*
prophet run.conf -cpp -skip-verify -r . -consider-all -first-n-loc 200 -feature-para ../../../crawler/para-all.out

