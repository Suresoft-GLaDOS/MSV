#!/bin/bash
rm __fixed_bresenham.cpp
prophet run.conf -cpp -skip-verify -skip-profile-build -r . -consider-all -first-n-loc 200 -feature-para ../../../crawler/para-all.out

