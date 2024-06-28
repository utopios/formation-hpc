#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define NX 128
#define NY 128
#define STEPS 1000
#define DT 0.01

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void initialize(double u[NX][NY], double v[NX][NY], double p[NX][NY], double b[NX][NY]) {
    for (int i = 0; i < NX; i++) {
        for (int j = 0; j < NY; j++) {
            u[i][j] = v[i][j] = p[i][j] = b[i][j] = 0.0;
        }
    }
}

void build_up_b(double b[NX][NY], double u[NX][NY], double v[NX][NY], double dt, double dx, double dy) {
    for (int i = 1; i < NX-1; i++) {
        for (int j = 1; j < NY-1; j++) {
            b[i][j] = (dy * (u[i+1][j] - u[i-1][j]) + dx * (v[i][j+1] - v[i][j-1])) / (2.0 * (dx + dy));
        }
    }
}

void pressure_poisson(double p[NX][NY], double b[NX][NY], double dx, double dy, int niters) {
    double pn[NX][NY];
    for (int it = 0; it < niters; it++) {
        #pragma omp parallel for
        for (int i = 1; i < NX-1; i++) {
            for (int j = 1; j < NY-1; j++) {
                pn[i][j] = p[i][j];
            }
        }
        
        #pragma omp parallel for
        for (int i = 1; i < NX-1; i++) {
            for (int j = 1; j < NY-1; j++) {
                p[i][j] = ((pn[i+1][j] + pn[i-1][j]) * dy*dy +
                           (pn[i][j+1] + pn[i][j-1]) * dx*dx -
                           b[i][j] * dx*dx * dy*dy) / (2.0 * (dx*dx + dy*dy));
            }
        }
    }
}

void solve(double u[NX][NY], double v[NX][NY], double p[NX][NY], double b[NX][NY], double dt, double dx, double dy) {
    for (int t = 0; t < STEPS; t++) {
        build_up_b(b, u, v, dt, dx, dy);
        pressure_poisson(p, b, dx, dy, 50);

        #pragma omp parallel for
        for (int i = 1; i < NX-1; i++) {
            for (int j = 1; j < NY-1; j++) {
                double du = dt / dx * (p[i+1][j] - p[i][j]);
                double dv = dt / dy * (p[i][j+1] - p[i][j]);
                #pragma omp atomic
                u[i][j] -= du;
                #pragma omp atomic
                v[i][j] -= dv;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    double u[NX][NY], v[NX][NY], p[NX][NY], b[NX][NY];
    double dx = 1.0 / (NX - 1);
    double dy = 1.0 / (NY - 1);

    initialize(u, v, p, b);

    double start_time = get_time();
    solve(u, v, p, b, DT, dx, dy);
    double end_time = get_time();

    if (world_rank == 0) {
        printf("Simulation finished in %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}