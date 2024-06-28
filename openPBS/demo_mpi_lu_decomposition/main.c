#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SIZE 1000 // Taille de la matrice

void initialize_matrix(double matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix[i][j] = rand() % 10 + 1; // Eviter les zéros pour la simplicité
        }
    }
}

void lu_decomposition(double matrix[SIZE][SIZE], double L[SIZE][SIZE], double U[SIZE][SIZE], int rank, int size) {
    for (int k = 0; k < SIZE; k++) {
        if (k % size == rank) {
            for (int j = k; j < SIZE; j++) {
                U[k][j] = matrix[k][j];
                for (int s = 0; s < k; s++) {
                    U[k][j] -= L[k][s] * U[s][j];
                }
            }
            for (int i = k; i < SIZE; i++) {
                if (i == k) {
                    L[i][k] = 1.0;
                } else {
                    L[i][k] = matrix[i][k];
                    for (int s = 0; s < k; s++) {
                        L[i][k] -= L[i][s] * U[s][k];
                    }
                    L[i][k] /= U[k][k];
                }
            }
        }

        MPI_Bcast(&U[k][k], SIZE - k, MPI_DOUBLE, k % size, MPI_COMM_WORLD);
        MPI_Bcast(&L[k][k], SIZE - k, MPI_DOUBLE, k % size, MPI_COMM_WORLD);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    double matrix[SIZE][SIZE], L[SIZE][SIZE] = {0}, U[SIZE][SIZE] = {0};

    if (world_rank == 0) {
        srand(time(NULL));
        initialize_matrix(matrix);
    }

    MPI_Bcast(matrix, SIZE*SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double start_time = MPI_Wtime();
    lu_decomposition(matrix, L, U, world_rank, world_size);
    double end_time = MPI_Wtime();

    if (world_rank == 0) {
        printf("Time taken with MPI: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}