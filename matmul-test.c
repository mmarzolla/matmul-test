/****************************************************************************
 *
 * matmul-test.c - Dense matrix-matrix product, scaffolding
 *
 * Copyright (C) 2025, 2026 by Moreno Marzolla <https://www.moreno.marzolla.name/>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * --------------------------------------------------------------------------
 *
 * Run with:
 *
 *      ./matmul-test [-a algos] [-h] [problem_size]
 *
 * Where:
 * - `algos` ia a string of characters denoting the algorithms
 *   to test; use the `-h` option to see the list of keys. Default
 *   is to test all algorithms.
 * - `problem_size` is the side of the matrix.
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <omp.h>
#include "common.h"

/* Fills n x n square matrix m with random values */
static void fill( float* m, int n )
{
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            m[i*n + j] = rand() / (double)RAND_MAX;
        }
    }
}

/* Return the sum of the content of matrix `m`; since our random
   matrices contain only elements with values in [0,1], all values of
   the produce will be positive. */
static double mat_sum(const float *m, int n)
{
    double result = 0.0;
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            result += m[i*n + j];
        }
    }
    return result;
}

struct {
    matmul_algo_t algo;
    char *description;
} matmul_algos[] = { {matmul_ijk, "Serial ijk"},
                     {matmul_ijk_opt, "Serial ijk + O2"},
                     {matmul_ikj_opt, "Serial ikj + O2"},
                     {matmul_jik_opt, "Serial jik + O2"},
                     {matmul_jki_opt, "Serial jki + O2"},
                     {matmul_kij_opt, "Serial kij + O2"},
                     {matmul_kji_opt, "Serial kji + O2"},
                     {matmul_ikj_simd_auto, "ikj + O2 + auto SIMD"},
                     {matmul_ikj_simd, "ikj + O2 + manual SIMD"},
                     {matmul_ikj_simd_omp, "ikj + O2 + manual SIMD + OpenMP"},
                     {matmul_dac, "Parallel divide-and-conquer"},
                     {NULL, NULL} };

void print_help( const char *exec_name )
{
    printf("Usage: %s [-a algos] [-h] [problem_size]\n\n", exec_name);
    printf("\talgos is the list of keys of the algorithms to test (default: all);\n\tsee below.\n\n");
    printf("[key] algo:\n\n");
    for (int i=0; matmul_algos[i].algo != NULL; i++) {
        printf("\t[%c] %s\n", 'a' + i, matmul_algos[i].description);
    }
}

int main( int argc, char *argv[] )
{
    /* Alignement for AVX256 data types (256 / 8 = 32 bytes). */
    const size_t MEM_ALIGNEMENT = 32;
    int opt, err;
    const char *algostr = NULL;
    int n = 4096;
    float *p, *q, *r;

    while ((opt = getopt(argc, argv, "a:h")) != -1) {
        switch (opt) {
        case 'a':
            algostr = optarg;
            break;
        case 'h':
        default:
            print_help(argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (optind < argc) {
        n = atoi(argv[optind]);
    }

    const size_t MAT_SIZE = n * n * sizeof(*p);

    err = posix_memalign((void**)&p, MEM_ALIGNEMENT, MAT_SIZE); assert(err == 0);
    err = posix_memalign((void**)&q, MEM_ALIGNEMENT, MAT_SIZE); assert(err == 0);
    err = posix_memalign((void**)&r, MEM_ALIGNEMENT, MAT_SIZE); assert(err == 0);

    fill(p, n);
    fill(q, n);

    printf("\nMatrix-Matrix multiplication (C) %d x %d\n", n, n);
#ifdef RESTRICT
    printf("Using 'restrict' keyword\n\n");
#else
    printf("NOT using 'restrict' keyword\n\n");
#endif
    printf("Key Algorithm                             Time (s)     Gflops Check\n");
    printf("--- ----------------------------------- ---------- ---------- ------------------\n");
    for (int i=0; matmul_algos[i].algo != NULL; i++) {

        /* skip if algostr is provided and does not contain the key of
           this algorithm. */
        if (algostr && !strchr(algostr, 'a' + i))
            continue;

        printf("[%c] %-35s ", 'a'+i, matmul_algos[i].description); fflush(stdout);
        /* Purge data from cache; `__builtin___clear_cache()` is a
           built-in function that acts as a portable interface for the
           appropriate low-level OS function, if available. The
           (void*) casts are required to make the clang compiler
           happy. */
        __builtin___clear_cache((void*)p, (void*)(p + MAT_SIZE));
        __builtin___clear_cache((void*)q, (void*)(q + MAT_SIZE));
        __builtin___clear_cache((void*)r, (void*)(r + MAT_SIZE));
        const double tstart = omp_get_wtime();
        matmul_algos[i].algo(p, q, r, n);
        const double elapsed = omp_get_wtime() - tstart;
        const double Gflops = 2.0 * (n/1000.0) * (n/1000.0) * (n/1000.0) / elapsed;
        printf("%10.3f %10.3f %.5f\n", elapsed, Gflops, mat_sum(r,n));
    }
    printf("\n");

    free(p);
    free(q);
    free(r);

    return EXIT_SUCCESS;
}
