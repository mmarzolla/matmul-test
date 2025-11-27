/****************************************************************************
 *
 * matmul_dac.c - Parallel divide-and-conquer matrix-matrix multiply with OpenMP
 *
 * Adapted from "6.172 Performance Engineering of Software Systems" at MIT
 * <https://ocw.mit.edu/courses/6-172-performance-engineering-of-software-systems-fall-2018/>
 *
 * Transcribed with modifications by Moreno Marzolla <https://www.moreno.marzolla.name/>.
 * Last modified on 2025-10-15 by Moreno Marzolla.
 *
 ****************************************************************************/

#include <stdio.h>
#include <omp.h>
#include "common.h"

/* Base case of the recursion; we let the compiler
   optimize this. */
static void mm_base(float * RESTRICT C,
                    const float * RESTRICT A,
                    const float * RESTRICT B,
                    int n,
                    int b /* sub-matrix size */)
{
    for (int i=0; i<b; i++) {
        for (int k=0; k<b; k++) {
            for (int j=0; j<b; j++) {
                C[i*n+j] += A[i*n+k] * B[k*n+j];
            }
        }
    }
}

/* Main recursion with OpenMP tasking. */
static void mm_rec( float * RESTRICT C,
                    const float * RESTRICT A,
                    const float * RESTRICT B,
                    int n,
                    int b /* sub-matrix size */)
{
    const int THR = 64; /* tuning parameter */
    if (b <= THR) {
        mm_base(C, A, B, n, b);
    } else {
#define SUB(M,ro,co) ((M) + ((ro)*n+(co)) * ((b)/2) )
#pragma omp task
        mm_rec(SUB(C,0,0), SUB(A,0,0), SUB(B,0,0), n, b/2);
#pragma omp task
        mm_rec(SUB(C,0,1), SUB(A,0,0), SUB(B,0,1), n, b/2);
#pragma omp task
        mm_rec(SUB(C,1,0), SUB(A,1,0), SUB(B,0,0), n, b/2);
#pragma omp task
        mm_rec(SUB(C,1,1), SUB(A,1,0), SUB(B,0,1), n, b/2);

#pragma omp taskwait

#pragma omp task
        mm_rec(SUB(C,0,0), SUB(A,0,1), SUB(B,1,0), n, b/2);
#pragma omp task
        mm_rec(SUB(C,0,1), SUB(A,0,1), SUB(B,1,1), n, b/2);
#pragma omp task
        mm_rec(SUB(C,1,0), SUB(A,1,1), SUB(B,1,0), n, b/2);
#pragma omp task
        mm_rec(SUB(C,1,1), SUB(A,1,1), SUB(B,1,1), n, b/2);

#pragma omp taskwait
    }
}

void matmul_dac( const float * RESTRICT p,
                 const float * RESTRICT q,
                 float * RESTRICT r,
                 int n)
{
#pragma omp parallel
    { // begin of parallel region
#pragma omp for
        for (int i=0; i<n; i++) {
            for (int j=0; j<n; j++) {
                r[i*n+j] = 0.0f;
            }
        }

#pragma omp master
        mm_rec(r, p, q, n, n);
    } // end of parallel region
}
