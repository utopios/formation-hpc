#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

// Fonction pour calculer les carrés de manière séquentielle
void square_array(int *array, int *result, int n) {
    for (int i = 0; i < n; i++) {
            result[i] = array[i] * array[i];
        }
}

void square_array_parallel(int *array, int *result, int n) {
    int num_threads = omp_get_max_threads();
    double *times = malloc(num_threads * sizeof(double));
    double global_start = omp_get_wtime();

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        double t_start = omp_get_wtime();

        #pragma omp for
        for (int i = 0; i < n; i++) {
            result[i] = array[i] * array[i];
        }

        double t_end = omp_get_wtime();
        times[tid] = t_end - t_start;  // Directly store the time spent by each thread
    }

    double global_end = omp_get_wtime();

    // Afficher le temps pour chaque thread
    for (int i = 0; i < num_threads; i++) {
        printf("Thread %d - Time: %f seconds\n", i, times[i]);
    }
    printf("Total parallel time (measured internally): %f seconds\n", global_end - global_start);

    free(times);
}

int main() {
    int n = 100000000; // 100 million elements
    int *array = malloc(n * sizeof(int));
    int *result_seq = malloc(n * sizeof(int));
    int *result_par = malloc(n * sizeof(int));
    
    // Initialization of the array
    for (int i = 0; i < n; i++) {
        array[i] = i;
    }

    // Measure time for the sequential version
    clock_t start_seq = clock();
    square_array(array, result_seq, n);
    clock_t end_seq = clock();
    double sequential_time = ((double) (end_seq - start_seq)) / CLOCKS_PER_SEC;

    // Measure time for the parallel version
    clock_t start_par = clock();
    square_array_parallel(array, result_par, n);
    clock_t end_par = clock();
    double parallel_time = ((double) (end_par - start_par)) / CLOCKS_PER_SEC;

    // Calculate speedup and efficiency
    double speedup = sequential_time / parallel_time;
    double efficiency = speedup / omp_get_max_threads();

    // Display results
    printf("Sequential Time: %f seconds\n", sequential_time);
    printf("Parallel Time: %f seconds\n", parallel_time);
    printf("Speedup: %f\n", speedup);
    printf("Efficiency: %f\n", efficiency);

    free(array);
    free(result_seq);
    free(result_par);
    return 0;
}