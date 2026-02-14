# Introduction

This repository contains code that tries to replicate the performance
improvement of dense matrix-matrix multiplication discussed in the
first lecture of the [6.172 Performance Engineering of Software
Systems](https://ocw.mit.edu/courses/6-172-performance-engineering-of-software-systems-fall-2018/)
course at MIT. The video recording of prof. Charles Leiserson
discussing this topic is
[here](https://ocw.mit.edu/courses/6-172-performance-engineering-of-software-systems-fall-2018/resources/lecture-1-intro-and-matrix-multiplication/).
Note that the lecture is from 2018, so a few years have passed since
then.

Three languages are used for the implementation:

- Python
- Java
- C99

I started from the source code shown in the slides
[here](https://ocw.mit.edu/courses/6-172-performance-engineering-of-software-systems-fall-2018/resources/lecture-1-introduction-and-matrix-multiplication/). However,
substantial modifications were made, especially to the C code, so I do
not claim that my program is anywhere equivalent to that used in the
MIT course. As usual I take the blame for any error.

# Algorithms

The C program includes several implementations of the matrix-matrix
multiplication algorithm, with various levels of optimization.

The starting point is the _ijk_ algorithm, that refers to the classic
"textbook" program that corresponds to the following C code:

```C
/* Compute r = p * q */
void matmul(const float * restrict p, const float * restrict q, float * restrict r, int n)
{
  for (int i=0; i<n; i++)
    for (int j=0; j<n; j++)
      r[i*n + j] = 0.0f;

  for (int i=0; i<n; i++)
    for (int j=0; j<n; j++)
      for (int k=0; k<n; k++)
        r[i*n + j] += p[i*n + k] * q[k*n + j];
}
```

Upon that, several optimizations are applied.

The source code is split into several source files that are compiled
separately, possibly with different compiler flags as shown in the
table below.

| Source                   | Description                                                     | Compiler flags                                            |
|--------------------------|-----------------------------------------------------------------|-----------------------------------------------------------|
| `matmul_ijk.c`           | Unoptimized reference algorithm                                 | (none)                                                    |
| `matmul_opt.c`           | Compiler optimization                                           | `-O2`                                                     |
| `matmul_ikj_simd_auto.c` | _ikj_ nesting + compiler vectorization                          | `-O2 -ffast-math -ftree-vectorize -march=native`          |
| `matmul_ikj_simd.c`      | _ikj_ nesting + manual vectorization using SIMD extensions      | `-O2 -ffast-math -march=native`                           |
| `matmul_ikj_omp.c`       | _ikj_ nesting + parallel loops using OpenMP                     | `-O2 -fopenmp`                                            |
| `matmul_ikj_simd_omp.c`  | _ikj_ nesting + manual vectorization + parallel loops           | `-O2 -fopenmp -march=native -ffast-math`                  |
| `matmul_dac.c`           | Divide-and-conquer + compiler vectoriation + OpenMP tasking     | `-O2 -fopenmp -ftree-vectorize -march=native -ffast-math` |


# Compilation

To compile type:

```
make
```

The provided `Makefile` compiles the C and Java programs using the
appropriate compiler flags. The C program requires `gcc` and relies on
OpenMP support. The program also compiles with LLVM/clang using the
following command:

```
CC=clang make
```

# Execution

Run the Python program as:

```
./matmul-test.py [n]
```

where the optional parameter `n` is the matrix size (default 4096;
**WARNING** the Python program may take several hours with the default
matrix size!).

Run the Java program as:

```
java matmul_test [n]
```

Finally, run the C program as:

```
./matmul-test [n]
```

By default, the C program executes all algorithms; it is possible to
use the command-line flag `-a [keys]` to restrict execution to a
subset of the algorithms. `[keys]` is a sequence of characters that
represent the algorithms to run, according to the following table:

| Key | Algorithm                       |
|-----|---------------------------------|
| `a` | Serial ijk                      |
| `b` | Serial ijk + O2                 |
| `c` | Serial ikj + O2                 |
| `d` | Serial jik + O2                 |
| `e` | Serial jki + O2                 |
| `f` | Serial kij + O2                 |
| `g` | Serial kji + O2                 |
| `h` | ikj + O2 + auto SIMD            |
| `i` | ikj + O2 + manual SIMD          |
| `j` | ikj + O2 + manual SIMD + OpenMP |
| `k` | Parallel divide-and-conquer     |

Therefore, to execute the unoptimized _ijk_ algorithm (key `a`) and
the parallel divide-and-conquer (key `k`) on matrices of size $2048
\times 2048$, you can execute:

```
./matmul-test -a ak 2048
```

# Results

> [!NOTE]
> Modern processors have separate P(erformance) and E(fficient) cores
> that operate ad different clock rates. For example, on one of my
> systems (12th Gen Intel(R) Core(TM) i9-12900F processor) the OS
> reports 24 cores, of which the first 16 are P-cores where adjacent
> pairs are mapped to the same (hyperthreaded) physical core. To use
> the P-cores only, I use the `OMP_PLACES` and `OMP_NUM_THREADS`
> environment variables as follows:
>
> ```OMP_NUM_THREADS=16 OMP_PLACES="0:15" ./matmul-test```

> [!NOTE]
> These results are not intended to provide a reliable benchmark of
> the processor nor the compilers/interpreter used.

## Hardware/Software configuration

The following table reports the hardware/software configuration of the
machine used for the tests; the program has been executed on the bare
hardware.

| Feature             | Value                                      |
|---------------------|--------------------------------------------|
| Processor Type      | Intel(R) Core(TM) i9-12900F                |
| Clock frequency     | 5.1 GHz (P-core), 3.8 GHz (E-core)         |
| Processor chips     | 1                                          |
| Processing cores    | 8 P-cores + 8 E-cores                      |
| Threads per core    | 2 per P-core, 1 per E-core                 |
| Floating-point unit | 614.4 GFLOPS[^1]                           |
| SIMD extensions     | MMX, SSE, SSE2, SSE3, SSE4_1, SSE4_2, AVX2 |
| L1-dcache           | 640 KB total (16 instances)                |
| L1-icache           | 768 KB total (16 instances)                |
| L2-cache            | 14 MB total (10 instances)                 |
| L3-cache            | 30 MB total (one instance)                 |
| DRAM                | 64 GB                                      |
| OS                  | Ubuntu 24.04 Linux 6.14.0-37-generic       |
| Python              | 3.12.3                                     |
| Java                | openjdk 21.0.10 2026-01-20                 |
| gcc                 | 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04)      |
| Matrix size         | 4096                                       |

[^1]: Peak performance (GFLOPS) taken from [this
page](https://www.intel.com/content/www/us/en/support/articles/000005755/processors.html).

## Results

- _Time_ is the execution time, in seconds, average of five independent executions;
- _GFLOPS_ is an estimate of the number of floating-point operations, in billions ($10^9$, see below);
- _% of peak_ is the percentage of peak performance achieved by the algorithm; the peak performance of this machine is 614 GFLOPS;
- _Speedup_ is the relative speedup with respect with Python.

|                             |    Time (s) | GFLOPS | % of peak |  Speedup |
|-----------------------------|------------:|-------:|----------:|---------:|
| Python                      | 6711.86[^2] |   0.02 |     0.00% |     1.00 |
| Java                        |      322.02 |   0.43 |     0.07% |    20.84 |
| Serial ijk                  |      489.84 |   0.28 |     0.05% |    13.70 |
| Serial ijk + O2             |      322.20 |   0.41 |     0.07% |    20.20 |
| Serial ikj + O2             |       22.19 |   6.19 |     1.01% |   302.49 |
| Serial jik + O2             |      182.30 |   0.75 |     0.12% |    36.82 |
| Serial jki + O2             |      577.15 |   0.24 |     0.04% |    11.43 |
| Serial kij + O2             |       21.32 |   6.45 |     1.05% |   314.79 |
| Serial kji + O2             |      546.62 |   0.25 |     0.04% |    12.28 |
| ikj + auto SIMD             |       10.16 |  13.52 |     2.20% |   660.32 |
| ikj + manual SIMD           |       12.50 |  11.00 |     1.79% |   537.10 |
| ikj + manual SIMD + OpenMP  |        1.24 | 110.70 |    18.03% |  5405.80 |
| Parallel divide-and-conquer |        0.43 | 316.68 |    51.58% | 15465.08 |

[^2]: The execution time of the Python program has been estimated from
the execution time with $n=1024$ multiplied by $4^3$.

Each iteration of the innermost loop performs two floating-point
operations (one addition and one multiplication). Therefore, the total
number of floating-point operations is $2n^3$, where $n$ is the matrix
size. The GFLOPS are estimated as:

```math
\text{GFLOPS} = \frac{2n^3}{\text{Execution Time}}
```

## Discussion

- The most striking result is that Java is _1.5 times faster_ than
  unoptimized C code. This is in stark contrast with the results shown
  in the MIT lecture from 2018, where Java was two times slower. Java
  is still faster, by a small margin, than optimized code produced by
  `gcc` with the `-O2` flag.
- Different permutations of the loop nest show wide variations in the
  execution time. This is consistent with the MIT lecture, and is due
  to memory access patterns that are more or less cache-friendly.
- Compiler auto-vectorization produces surprisingly efficient code
  that is faster than hand-coded SIMD.
- The single-core SIMD version is only twice as fast as the
  unvectorized code. This is disappointing since we are using AVX2
  instructions with 256-bit registers that can hold 8 floats, so one
  would expect a speedup close to $8\times$.
