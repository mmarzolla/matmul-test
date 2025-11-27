/****************************************************************************
 *
 * matmul_ikj_simd_auto - Sequential matrix-matrix multiply, ikj, compiler-optimized + automatic SIMD
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

#include "common.h"

void matmul_ikj_simd_auto( const float * RESTRICT p, const float * RESTRICT q, float * RESTRICT r, int n)
{
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            r[i*n + j] = 0;
        }
    }

    for (int i=0; i<n; i++) {
        for (int k=0; k<n; k++) {
            for (int j=0; j<n; j++) {
                r[i*n + j] += p[i*n + k] * q[k*n + j];
            }
        }
    }
}
