#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000  // Taille de la grille
#define MAX_ITER 1000  // Nombre maximum d'itérations

// Initialisation de la grille
void initialize_grid(double grid[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = drand48();
        }
    }
}

// Simulation séquentielle de la diffusion de chaleur
void heat_diffusion_seq(double grid[N][N], double new_grid[N][N]) {
    for (int iter = 0; iter < MAX_ITER; iter++) {
        for (int i = 1; i < N - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                new_grid[i][j] = 0.25 * (grid[i-1][j] + grid[i+1][j] + grid[i][j-1] + grid[i][j+1]);
            }
        }
        // Swap the grids
        double (*temp)[N] = grid;
        grid = new_grid;
        new_grid = temp;
    }
}

// Simulation parallèle de la diffusion de chaleur
void heat_diffusion_parallel(double grid[N][N], double new_grid[N][N]) {
    #pragma omp parallel
    {
        for (int iter = 0; iter < MAX_ITER; iter++) {
            #pragma omp for collapse(2)
            for (int i = 1; i < N - 1; i++) {
                for (int j = 1; j < N - 1; j++) {
                    new_grid[i][j] = 0.25 * (grid[i-1][j] + grid[i+1][j] + grid[i][j-1] + grid[i][j+1]);
                }
            }
            #pragma omp single
            {
                // Swap the grids
                double (*temp)[N] = grid;
                grid = new_grid;
                new_grid = temp;
            }
        }
    }
}

int main() {
    double (*grid)[N] = malloc(sizeof(double[N][N]));
    double (*new_grid)[N] = malloc(sizeof(double[N][N]));

    initialize_grid(grid);
    initialize_grid(new_grid);

    // Mesure du temps séquentiel
    clock_t start_seq = clock();
    heat_diffusion_seq(grid, new_grid);
    clock_t end_seq = clock();
    double time_seq = (double)(end_seq - start_seq) / CLOCKS_PER_SEC;

    initialize_grid(grid);  // Réinitialiser la grille
    initialize_grid(new_grid);

    // Mesure du temps parallèle
    double start_par = omp_get_wtime();
    heat_diffusion_parallel(grid, new_grid);
    double end_par = omp_get_wtime();
    double time_par = end_par - start_par;

    // Calcul du speedup
    double speedup = time_seq / time_par;

    printf("Temps d'exécution séquentiel : %f secondes\n", time_seq);
    printf("Temps d'exécution parallèle : %f secondes\n", time_par);
    printf("Speedup : %f\n", speedup);

    free(grid);
    free(new_grid);

    return 0;
}