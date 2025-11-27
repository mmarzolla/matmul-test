/****************************************************************************
 *
 * matmul_opt.c - Sequential matrix-matrix multiply, loop permutation, compiler-optimized
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

void matmul_ijk_opt( const float * RESTRICT p, const float * RESTRICT q, float * RESTRICT r, int n)
{
    MATMUL_SEQ_BODY(i,j,k);
}

void matmul_ikj_opt( const float * RESTRICT  p, const float * RESTRICT q, float * RESTRICT r, int n)
{
    MATMUL_SEQ_BODY(i,k,j);
}

void matmul_jik_opt( const float * RESTRICT p, const float * RESTRICT q, float * RESTRICT r, int n)
{
    MATMUL_SEQ_BODY(j,i,k);
}

void matmul_jki_opt( const float * RESTRICT p, const float * RESTRICT q, float * RESTRICT r, int n)
{
    MATMUL_SEQ_BODY(j,k,i);
}

void matmul_kij_opt( const float * RESTRICT p, const float * RESTRICT q, float * RESTRICT r, int n)
{
    MATMUL_SEQ_BODY(k,i,j);
}

void matmul_kji_opt( const float * RESTRICT p, const float * RESTRICT q, float * RESTRICT r, int n)
{
    MATMUL_SEQ_BODY(k,j,i);
}
