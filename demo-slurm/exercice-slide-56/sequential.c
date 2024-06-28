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
#define N 1000000  // Nombre de simulations

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
double monte_carlo() {
    double payoff_sum = 0.0;
    for (int i = 0; i < N; i++) {
        double ST = S0 * exp((R - 0.5 * SIGMA * SIGMA) * T + SIGMA * sqrt(T) * gauss_random());
        double payoff = fmax(0.0, ST - K);
        payoff_sum += payoff;
    }
    return exp(-R * T) * (payoff_sum / N);
}

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

int main() {
    srand(time(NULL));

    double start_time = get_time();
    double price = monte_carlo();
    double end_time = get_time();

    printf("Option Price: %f\n", price);
    printf("Time taken without MPI: %f seconds\n", end_time - start_time);

    return 0;
}