#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Paramètres de l'option
#define S0 100.0   // Prix initial de l'actif sous-jacent
#define K 100.0    // Prix d'exercice
#define T 1.0      // Temps jusqu'à l'expiration
#define R 0.05     // Taux d'intérêt sans risque
#define SIGMA 0.2  // Volatilité
#define N 1000000  // Nombre total de simulations

// Générateur de nombres aléatoires gaussiens
double gauss_random() {
    double u = ((double) rand() / RAND_MAX) * 2.0 - 1.0;
    double v = ((double) rand() / RAND_MAX) * 2.0 - 1.0;
    double r = u * u + v * v;
    while (r == 0 || r >= 1) {
        u = ((double) rand() / RAND_MAX) * 2.0 - 1.0;
        v = ((double) rand() / RAND_MAX) * 2.0 - 1.0;
        r = u * u + v * v;
    }
    double c = sqrt(-2 * log(r) / r);
    return u * c;
}

// Simulation de Monte Carlo pour le calcul du prix de l'option
double monte_carlo(int num_simulations) {
    double payoff_sum = 0.0;
    for (int i = 0; i < num_simulations; i++) {
        double ST = S0 * exp((R - 0.5 * SIGMA * SIGMA) * T + SIGMA * sqrt(T) * gauss_random());
        double payoff = fmax(0.0, ST - K);
        payoff_sum += payoff;
    }
    return payoff_sum / num_simulations;
}

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    srand(time(NULL) + world_rank);

    int num_simulations = N / world_size;
    double local_sum = monte_carlo(num_simulations);

    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        double price = exp(-R * T) * global_sum;
        double end_time = get_time();
        printf("Option Price: %f\n", price);
    }

    MPI_Finalize();
    return 0;
}