SRC:=$(wildcard *.c)
OBJ:=${SRC:%.c=%.o}
EXE:=matmul-test matmul_test.class

.PHONY: clean distclean

ALL: $(EXE)

matmul-test: CFLAGS=-fopenmp
matmul-test: LDFLAGS=-fopenmp
matmul-test: $(OBJ)

matmul_test.class: matmul_test.java
	javac $<

matmul_opt.o: CFLAGS=-O2

matmul_ikj_simd_auto.o: CFLAGS=-O2 -ffast-math -ftree-vectorize -march=native

matmul_ikj_omp.o: CFLAGS=-O2 -fopenmp

matmul_ikj_simd.o: CFLAGS=-O2 -march=native -ffast-math

matmul_ikj_simd_omp.o: CFLAGS=-O2 -fopenmp -march=native -ffast-math

matmul_dac.o: CFLAGS=-O2 -fopenmp -ftree-vectorize -march=native -ffast-math

clean:
	\rm -f $(EXE) $(OBJ) *~ *.txt

distclean: clean

