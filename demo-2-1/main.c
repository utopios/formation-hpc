#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000  // Dimension des matrices

// Allocation dynamique d'une matrice
double** allocate_matrix() {
    double** matrix = (double**)malloc(N * sizeof(double*));
    if (matrix == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la matrice\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++) {
        matrix[i] = (double*)malloc(N * sizeof(double));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour la matrice\n");
            exit(EXIT_FAILURE);
        }
    }
    return matrix;
}

// Libération de la matrice
void free_matrix(double** matrix) {
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Initialisation des matrices
void initialize_matrix(double** mat) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mat[i][j] = drand48();  // Utiliser drand48() pour des valeurs à virgule flottante
        }
    }
}

// Multiplication séquentielle de matrices
void multiply_matrices_seq(double** a, double** b, double** result) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = 0;
            for (int k = 0; k < N; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// Multiplication parallèle de matrices
void multiply_matrices_parallel(double** a, double** b, double** result) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += a[i][k] * b[k][j];
            }
            result[i][j] = sum;
        }
    }
}

int main() {
    double** a = allocate_matrix();
    double** b = allocate_matrix();
    double** result_seq = allocate_matrix();
    double** result_par = allocate_matrix();
    
    // Initialisation des matrices avec des nombres aléatoires
    initialize_matrix(a);
    initialize_matrix(b);

    // Multiplication séquentielle
    clock_t start_seq = clock();
    multiply_matrices_seq(a, b, result_seq);
    clock_t end_seq = clock();
    double time_seq = (double)(end_seq - start_seq) / CLOCKS_PER_SEC;

    // Multiplication parallèle
    double start_par = omp_get_wtime();
    multiply_matrices_parallel(a, b, result_par);
    double end_par = omp_get_wtime();
    double time_par = end_par - start_par;

    // Calcul du speedup
    double speedup = time_seq / time_par;

    printf("Temps d'exécution séquentiel : %f secondes\n", time_seq);
    printf("Temps d'exécution parallèle : %f secondes\n", time_par);
    printf("Speedup : %f\n", speedup);

    // Libération de la mémoire
    free_matrix(a);
    free_matrix(b);
    free_matrix(result_seq);
    free_matrix(result_par);

    return 0;
}