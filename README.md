# Introduction

This repository contains code that tries to replicate the performance
<<<<<<< HEAD
improvement of dense matrix-matrix multiplication discussed in the
first lecture of the [6.172 Performance Engineering of Software
Systems](https://ocw.mit.edu/courses/6-172-performance-engineering-of-software-systems-fall-2018/)
course at MIT. The video recording of prof. Charles Leiserson
discussing this topic is
[here](https://ocw.mit.edu/courses/6-172-performance-engineering-of-software-systems-fall-2018/resources/lecture-1-intro-and-matrix-multiplication/).
Note that the lecture is from 2018, so a few years have passed since
then.
=======
improvement of the dense matrix-matrix multiplication program
discussed in the first lecture of the [6.172 Performance Engineering
of Software
Systems](https://ocw.mit.edu/courses/6-172-performance-engineering-of-software-systems-fall-2018/)
course at MIT. The video recording of prof. Charles Leiserson is
[here](https://ocw.mit.edu/courses/6-172-performance-engineering-of-software-systems-fall-2018/resources/lecture-1-intro-and-matrix-multiplication/);
the lecture is from Fall 2018, so a few years have passed since then.
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53

Three languages are used for the implementation:

- Python
- Java
- C99

<<<<<<< HEAD
I started from the source code shown in the slides
[here](https://ocw.mit.edu/courses/6-172-performance-engineering-of-software-systems-fall-2018/resources/lecture-1-introduction-and-matrix-multiplication/). However,
substantial modifications were made, especially to the C code, so I do
not claim that my program is anywhere equivalent to that used in the
MIT course. As usual I take the blame for any error.
=======
I started from the source code shown in the slides that can be found
[here](https://ocw.mit.edu/courses/6-172-performance-engineering-of-software-systems-fall-2018/resources/lecture-1-introduction-and-matrix-multiplication/). However,
substantial modifications were made, especially to the C code, so I do
not claim that my program is anywhere equivalent to that used in the
MIT course.
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53

# Algorithms

The C program includes several implementations of the matrix-matrix
multiplication algorithm, with various levels of optimization.

The starting point is the _ijk_ algorithm, that refers to the classic
<<<<<<< HEAD
"textbook" program corresponding to the following C code:
=======
"textbook" matmul program; it corresponds to the following C code:
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53

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

<<<<<<< HEAD
Upon that, several optimizations are applied with more or less
increasing level of effectiveness.

The source code is split into several files that are compiled
separately, possibly with different compiler flags as shown in the
table below.
=======
The program is split into several source files that are compiled
separately with different flags.
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53

| Source                  | Description                                                     | Compiler flags                                            |
|-------------------------|-----------------------------------------------------------------|-----------------------------------------------------------|
| `matmul_ijk.c`          | Unoptimized reference algorithm                                 | (none)                                                    |
<<<<<<< HEAD
| `matmul_opt.c`          | Compiler optimization, all permutations of the nested loops     | `-O2`                                                     |
=======
| `matmul__opt.c`         | Compiler optimization, all permutations of the nested loops     | `-O2`                                                     |
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53
| `matmul_ikj_simd_auto.c`  | _ikj_ nesting + compiler vectorization                          | `-O2 -ffast-math -ftree-vectorize -march=native`          |
| `matmul_ikj_simd.c`     | _ikj_ nesting + manual vectorization using AVX2 SIMD extensions | `-O2 -ffast-math -mavx2 -mfma`                            |
| `matmul_ikj_omp.c`      | _ikj_ nesting + parallel loops using OpenMP                     | `-O2 -fopenmp`                                            |
| `matmul_ikj_simd_omp.c` | _ikj_ nesting + manual vectorization + parallel loops           | `-O2 -fopenmp -mavx2 -mfma -ffast-math`                   |
<<<<<<< HEAD
| `matmul_dac.c`          | Divide-and-conquer + compiler vectoriation + OpenMP tasking     | `-O2 -fopenmp -ftree-vectorize -march=native -ffast-math` |
=======
| `matmul_dac.c`          | Divide-and-conquer + compiler vectorization + OpenMP            | `-O2 -fopenmp -ftree-vectorize -march=native -ffast-math` |
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53


# Compilation

To compile type:

```
make
```

The provided `Makefile` compiles the C and Java programs using the
<<<<<<< HEAD
appropriate compiler flags. The C program requires `gcc` and relies on
OpenMP support. The program also compiles with LLVM/clang using the
following command:

```
CC=clang make
```
=======
appropriate compiler flags. The C program requires `gcc`, and requires
OpenMP support.
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53

# Execution

Run the Python program as:

```
./matmul-test.py [n]
```

<<<<<<< HEAD
where the optional parameter `n` is the matrix size (default 4096;
**WARNING** the Python program requires several hours with the default
matrix size!).
=======
where the optional parameter `n` is the matrix size (default 4096).
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53

Run the Java program as:

```
java matmul_test [n]
```

Finally, run the C program as:

```
./matmul-test [n]
```

<<<<<<< HEAD
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

=======
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53
> [!NOTE]
> Modern processors have separate P(erformance) and E(fficient) cores
> that operate ad different clock rates. For example, on one of my
> systems (12th Gen Intel(R) Core(TM) i9-12900F processor) the OS
<<<<<<< HEAD
> reports 24 cores, of which the first 16 are P-cores where adjacent
> pairs are mapped to the same (hyperthreaded) physical core. To use
> the P-cores only, I use the `OMP_PLACES` and `OMP_NUM_THREADS`
=======
> reports 24 cores, of which 16 are P-cores where adjacent pairs are
> mapped to the same (hyperthreaded) physical core. On this machine,
> to use the P-cores only, set the `OMP_PLACES` and `OMP_NUM_THREADS`
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53
> environment variables as follows:
>
> ```OMP_NUM_THREADS=16 OMP_PLACES="0:15" ./matmul-test```

<<<<<<< HEAD
## Hardware/Software configuration

The following table reports the hardware/software configuration of the
machine used for the tests; the program has been executed on the bare
hardware.
=======

# Results

Performance results on several machines are listed in the file
[matmul_results.ods](matmul_results.ods). I report below one of the
data tables.

## Hardware/Software configuration
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53

| Feature              | Value                                  |
|----------------------|----------------------------------------|
| Processor Type       | Intel(R) Core(TM) i7-9800X             |
| Clock frequency      | 3.8 GHz                                |
| Processor chips      | 1                                      |
<<<<<<< HEAD
| Processing cores     | 8	                                    |
| Threads per core     | 2                                      |
| Floating-point unit  | 742 GFLOPS[^1]                         |
| L1-icache	           | 256 KB x 8                             |
=======
| Processing cores     | 8	                                |
| Threads per core     | 2                                      |
| Floating-point unit  | 742 GFLOPS[^1]                         |
| L1-icache	       | 256 KB x 8                             |
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53
| L1-dcache            | 256 KB x 8                             |
| L2-cache             | 8 MB x 8                               |
| L3-cache             | 16.5 MB                                |
| DRAM                 | 32 GB                                  |
| OS                   | Ubuntu 24.04 Linux 6.14.0-29-generic   |
| Python               | 3.12.3                                 |
| Java                 | openjdk 21.0.8 2025-07-15              |
| gcc                  | 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04)  |
| Matrix size          | 4096                                   |

[^1]: Peak performance (GFLOPS) taken from [this
page](https://www.intel.com/content/www/us/en/support/articles/000005755/processors.html).

## Results

<<<<<<< HEAD
- _Time_ is the execution time, in seconds;
- _GFLOPS_ is an estimate of the number of floating-point operations, in billions ($10^9$, see below);
- _% of peak_ is the percentage of peak performance achieved by the algorithm; the peak performance of this machine is 742 GFLOPS;
- _Speedup_ is the relative speedup with respect with Python.

=======
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53
|                             | Time (s) | GFLOPS | % of peak |  Speedup |
|-----------------------------|---------:|-------:|----------:|---------:|
| Python                      | 13422.66 |   0.01 |     0.00% |     1.00 |
| Java                        |   463.77 |   0.30 |     0.04% |    28.94 |
| Serial ijk                  |  1582.71 |   0.09 |     0.01% |     8.48 |
| Serial ijk + O2             |   559.39 |   0.25 |     0.03% |    24.00 |
| Serial ikj + O2             |    43.73 |   3.14 |     0.42% |   306.94 |
| Serial jik + O2             |   366.82 |   0.37 |     0.05% |    36.59 |
| Serial jki + O2             |  1108.43 |   0.12 |     0.02% |    12.11 |
| Serial kij + O2             |    44.57 |   3.08 |     0.42% |   301.17 |
| Serial kji + O2             |  1157.95 |   0.12 |     0.02% |    11.59 |
| ikj + auto SIMD             |    21.97 |   6.25 |     0.84% |   610.87 |
| ikj + manual SIMD           |    23.14 |   5.94 |     0.80% |   580.14 |
| ikj + manual SIMD + OpenMP  |     1.84 |  74.82 |    10.08% |  7306.84 |
| Parallel divide-and-conquer |     1.06 | 129.66 |    17.47% | 12662.88 |

Each iteration of the innermost loop performs two floating-point
<<<<<<< HEAD
operations (one addition and one multiplication). Therefore, the total
number of floating-point operations is $2n^3$, where $n$ is the
matrix size, and the GFLOPS are:
=======
operations (one addition and one multiplication). The total number of
floating-point operations is $2n^3$, where $n$ is the matrix size, and
the GFLOPS are:
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53

```math
\text{GFLOPS} = \frac{2n^3}{\text{Execution Time}}
```

## Discussion

- The most striking result is that Java is _four times faster_ than
<<<<<<< HEAD
  unoptimized C code. This is in stark contrast with the results shown
  in the MIT lecture from year 2018, where Java was two times
  slower. Java is still faster than optimized code produced by `gcc`
  with the `-O2` flag. This happens on all processors of different
  generation that I was able to test.
- Different permutations of the loop nest show wide variations in the
  execution time. This is consistent with the MIT lecture, and is due
  to memory access patterns that are more or less cache-friendly.
- Compiler auto-vectorization produces faster code than manual
  vectorization using AVX2 extensions (I wrote the SIMD code from
  scratch, so it might be sub-optimal).
- The single-core SIMD version is only twice as fast as the
  unvectorized code. This is disappointing since we are using AVX2
  instructions with 256-bit registers that can hold 8 floats, so one
  would expect a speedup close to $8\times$.
=======
  unoptimized C code. This is in stark contrast with the results from
  the MIT lecture from 2018, where Java was two times slower. Java is
  currently still faster than the optimized code produced by `gcc`
  with the `-O2` flag. This happens on all processors I ran the
  programs on, which however have identical software stack.
- Different permutations of the loop nest produce large variations in
  the execution time, from 43s to 1582s. This is consistent with the
  MIT lecture, and is due to the different memory access patterns that
  are more or less cache-friendly.
- Compiler auto-vectorization produces faster code than manual
  vectorization using AVX2 extensions. The manually-written (by me)
  SIMD code is therefore sub-optimal.
- The single-core SIMD versions are only twice as fast as the
  unvectorized code with the optimal lopo nest. This is a bit
  disappointing, since we are using AVX2 instructions that can process
  8 floats at the same time, so one might expect a speedup close to
  $8\times$.
>>>>>>> 474a899ed85577e0adc7505a30c6d56c150c6c53
