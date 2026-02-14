/****************************************************************************
 *
 * matmul_test.java - Sequential, non-optimized matrix-matrix multiply
 *
 * Adapted from "6.172 Performance Engineering of Software Systems" at MIT
 * <https://ocw.mit.edu/courses/6-172-performance-engineering-of-software-systems-fall-2018/>
 *
 * Transcribed with modifications by Moreno Marzolla <https://www.moreno.marzolla.name/>
 * Last modified 2026-02-14 by Moreno Marzolla.
 *
 ****************************************************************************/

import java.util.Random;
import java.io.*;

public class matmul_test {

    public static void main(String[] args)
    {
        Random rnd = new Random();
        int n = 4096;
        float[][] p, q, r;

        if (args.length > 0) {
            n = Integer.parseInt(args[0]);
        }

        p = new float[n][n];
        q = new float[n][n];
        r = new float[n][n];

        for (int i=0; i<n; i++) {
            for (int j=0; j<n; j++) {
                p[i][j] = rnd.nextFloat();
                q[i][j] = rnd.nextFloat();
                r[i][j] = 0;
            }
        }

        System.out.printf("# Matrix-Matrix multiplication, Java version\n");
        System.out.printf("# n %d\n", n);

        final long start = System.currentTimeMillis();
        for (int i=0; i<n; i++) {
            for (int j=0; j<n; j++) {
                for (int k=0; k<n; k++) {
                    r[i][j] += p[i][k] * q[k][j];
                }
            }
        }
        final double elapsed = (System.currentTimeMillis() - start) * 1e-3;
        System.out.printf("Java, %.3f\n", elapsed);
    }
}
