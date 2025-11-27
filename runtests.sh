#!/bin/bash

# This script runs all programs several times, and dumps the output to
# different files for each run, and also to stdout. The Python program
# should use a smaller input size, since it takes forever to complete.
#
# Written by Moreno Marzolla on 2025-09-20
# Last modified on 2025-10-15 by Moreno Marzolla

PROBLEM_SIZE=4096;
NREP=5 # number of runs

for R in `seq 1 $NREP`; do
    FNAME=`printf "output%02d.txt" ${R}`
    {
        echo "# `uname -a`" ;
        echo "# `date`" ;
        echo "# problem size = $PROBLEM_SIZE" ;
        lscpu | grep -i -v "Vulnerability" | sed "s/^/# /" ;
        ./matmul-test.py 1024 ;
        java matmul_test $PROBLEM_SIZE ;
        ./matmul-test $PROBLEM_SIZE ;
    } | tee "$FNAME"
done
