/****************************************************************************
 *
 * common.h - Matrix-Matrix multiply, common declarations
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

#ifndef COMMON_H
#define COMMON_H

typedef void (* matmul_algo_t)( const float*, const float*, float*, int );
#define RESTRICT restrict

void matmul_ijk(const float*, const float*, float*, int);

void matmul_ijk_opt(const float*, const float*, float*, int);
void matmul_ikj_opt(const float*, const float*, float*, int);
void matmul_jik_opt(const float*, const float*, float*, int);
void matmul_jki_opt(const float*, const float*, float*, int);
void matmul_kij_opt(const float*, const float*, float*, int);
void matmul_kji_opt(const float*, const float*, float*, int);

void matmul_ikj_simd_auto(const float*, const float*, float*, int);

void matmul_ikj_simd(const float*, const float*, float*, int);

void matmul_ikj_omp_ijk(const float*, const float*, float*, int);

void matmul_ikj_simd_omp(const float*, const float*, float*, int);

void matmul_dac(const float*, const float*, float*, int);

#define MATMUL_SEQ_BODY(I1,I2,I3) \
{\
    for (int i=0; i<n; i++) {\
        for (int j=0; j<n; j++) {\
            r[i*n + j] = 0;\
        }\
    }\
\
    for (int I1=0; I1<n; I1++) {\
        for (int I2=0; I2<n; I2++) {\
            for (int I3=0; I3<n; I3++) {\
                r[i*n + j] += p[i*n + k] * q[k*n + j];\
            }\
        }\
    }\
}

#endif
