/****************************************************************************
 *
 * matmul_ikj_simd.c - Matrix-Matrix multiply, SIMD
 *
 * Copyright (C) 2025 by Moreno Marzolla <https://www.moreno.marzolla.name/>
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
 ****************************************************************************/

#include <immintrin.h>
#include <assert.h>
#include "common.h"

void matmul_ikj_simd( const float *p, const float *q, float *r, int n)
{
    const size_t VLEN = sizeof(__m256)/sizeof(float);

    assert(n % VLEN == 0); /* `n` must be multiple of `VLEN` */

    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j += VLEN) {
            __m256 *ptr = (__m256*)&r[i*n + j];
            *ptr = _mm256_setzero_ps();
        }
    }

    for (int i=0; i<n; i++) {
        for (int k=0; k<n; k++) {
            const __m256 pv = _mm256_set1_ps(p[i*n + k]);
            for (int j=0; j<n; j += VLEN) {
                __m256 rv = _mm256_load_ps( &r[i*n + j] );
                const __m256 qv = _mm256_load_ps( &q[k*n + j] );
                rv = _mm256_fmadd_ps(pv, qv, rv);
                _mm256_store_ps( &r[i*n + j], rv );
                // r[i*n + j] += p[i*n + k] * q[k*n + j];
            }
        }
    }
}
