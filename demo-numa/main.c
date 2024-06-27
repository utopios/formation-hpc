#include <stdio.h>
#include <numa.h>
#include <numaif.h>
#include <stdlib.h>

#define SIZE 1000

void matrix_multiply(double **A, double **B, double **C, int size) {
    int i, j, k;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            C[i][j] = 0.0;
            for (k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int i, j;

    // Supposons que le serveur a 2 nœuds NUMA : 0 et 1
    // Allouer des matrices sur des nœuds spécifiques
    double **A = (double **)numa_alloc_onnode(SIZE * sizeof(double *), 0);
    double **B = (double **)numa_alloc_onnode(SIZE * sizeof(double *), 1);
    double **C = (double **)numa_alloc_onnode(SIZE * sizeof(double *), 0);

    for (i = 0; i < SIZE; i++) {
        A[i] = (double *)numa_alloc_onnode(SIZE * sizeof(double), 0);
        B[i] = (double *)numa_alloc_onnode(SIZE * sizeof(double), 1);
        C[i] = (double *)numa_alloc_onnode(SIZE * sizeof(double), 0);
    }

    // Initialiser les matrices A et B
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            A[i][j] = (double)(i + j);
            B[i][j] = (double)(i - j);
        }
    }

    // Multiplier les matrices
    matrix_multiply(A, B, C, SIZE);

    // Libérer la mémoire
    for (i = 0; i < SIZE; i++) {
        numa_free(A[i], SIZE * sizeof(double));
        numa_free(B[i], SIZE * sizeof(double));
        numa_free(C[i], SIZE * sizeof(double));
    }
    numa_free(A, SIZE * sizeof(double *));
    numa_free(B, SIZE * sizeof(double *));
    numa_free(C, SIZE * sizeof(double *));

    return 0;
}