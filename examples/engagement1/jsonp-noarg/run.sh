#!/bin/sh
msv -cpp -skip-verify -r . -consider-all -first-n-loc 200 -feature-para ../../../crawler/para-all.out
cat __fixed_reader.cpp
