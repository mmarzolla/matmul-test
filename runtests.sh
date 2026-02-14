#!/bin/bash

# This script runs all programs and dumps the output to a file and
# also to stdout. The Python program uses a smaller input size, since
# it takes forever to complete.
#
# Written by Moreno Marzolla on 2025-09-20
# Last modified on 2026-02-14 by Moreno Marzolla

PROBLEM_SIZE=4096;
NREP=5 # number of experiments for the C version

{
    echo "# `uname -a`" ;
    echo "# `date`" ;
    echo "# problem size = $PROBLEM_SIZE" ;
    lscpu | grep -i -v "Vulnerability" | sed "s/^/# /" ;
    ./matmul-test.py 1024 ;
    java matmul_test $PROBLEM_SIZE ;
    ./matmul-test -r $NREP $PROBLEM_SIZE ;
} | tee "results.txt"
