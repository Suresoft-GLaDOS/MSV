#!/bin/bash
prophet run.conf -cpp -skip-verify -vl 4 -r . -consider-all -first-n-loc 200 -feature-para ../../../crawler/para-all.out
cat __fixed_bresenham.cpp
