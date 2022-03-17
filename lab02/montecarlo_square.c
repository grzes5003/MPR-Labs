#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "cert-msc51-cpp"
//
// Created by xgg on 15/03/2022.
//
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>


float estimate_pi(int n) {
    float in = 0.f;

    for (int i = 0; i < n; i++) {
        float x = (float) rand() / (float) (RAND_MAX);
        float y = (float) rand() / (float) (RAND_MAX);
        if (x * x + y * y <= 1) in += 1;
    }

    return 4.f * in;
}


int main(int argc, char *argv[]) {
    const float LIMIT = 100000.f;

    float local_data;
    float result;
    srand((unsigned int) time(NULL));

    int world_rank;
    int world_size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int n = (int) (LIMIT / (float) world_size);
    MPI_Barrier(MPI_COMM_WORLD);

    local_data = estimate_pi(n);
    MPI_Reduce(&local_data, &result, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        printf("[Process %d]: has the result: %.6f\n", world_rank, result / LIMIT);
    }

    MPI_Finalize();
    return 0;
}

#pragma clang diagnostic pop