#!/bin/env python3

############################################################################
#
# matmul-test.py - Sequential, non-optimized matrix-matrix multiply
#
# Adapted from "6.172 Performance Engineering of Software Systems" at MIT
# <https://ocw.mit.edu/courses/6-172-performance-engineering-of-software-systems-fall-2018/>
#
# Transcribed with modifications by Moreno Marzolla <https://www.moreno.marzolla.name/>
# Last modified on 2026-02-14 by Moreno Marzolla.
#
############################################################################

import sys, random
from time import time

n = 4096

if len(sys.argv) > 1:
    n = int(sys.argv[1])

p = [[random.random() for row in range(n)]
     for col in range(n)]
q = [[random.random() for row in range(n)]
     for col in range(n)]
r = [[0 for row in range(n)]
     for col in range(n)]

print("# Matrix-Matrix multiplication, Python version");
print("# n %d" % n);

start = time()
for i in range(n):
    for j in range(n):
        for k in range(n):
            r[i][j] += p[i][k] * q[k][j]
elapsed = time() - start

print("Python, %.3f" % elapsed)
