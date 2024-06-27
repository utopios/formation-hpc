#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// Fonction pour la somme séquentielle
long long sum_array(int *array, int n) {
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += array[i];
    }
    return sum;
}

// Fonction pour la somme parallèle
long long sum_array_parallel(int *array, int n) {
    long long sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++) {
        sum += array[i];
    }
    return sum;
}

int main() {
    int n = 100000000;
    int *array = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        array[i] = i;
    }

    // Mesurer le temps pour la version séquentielle
    clock_t start_seq = clock();
    long long result_seq = sum_array(array, n);
    clock_t end_seq = clock();
    double sequential_time = ((double) (end_seq - start_seq)) / CLOCKS_PER_SEC;

    // Mesurer le temps pour la version parallèle
    clock_t start_par = clock();
    long long result_par = sum_array_parallel(array, n);
    clock_t end_par = clock();
    double parallel_time = ((double) (end_par - start_par)) / CLOCKS_PER_SEC;

    // Calcul du speedup et de l'efficacité
    double speedup = sequential_time / parallel_time;
    double efficiency = speedup / omp_get_max_threads();

    // Affichage des résultats
    printf("Sequential Time: %f seconds\n", sequential_time);
    printf("Parallel Time: %f seconds\n", parallel_time);
    printf("Speedup: %f\n", speedup);
    printf("Efficiency: %f\n", efficiency);

    free(array);
    return 0;
}