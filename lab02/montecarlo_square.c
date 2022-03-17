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
#include <getopt.h>
#include <limits.h>
#include <errno.h>
#include <float.h>


float estimate_pi(long double n) {
    float in = 0.f;

    for (long double i = 0; i < n; i++) {
        float x = (float) rand() / (float) (RAND_MAX);
        float y = (float) rand() / (float) (RAND_MAX);
        if (x * x + y * y <= 1) in += 1;
    }

    return 4.f * in;
}


int main(int argc, char *argv[]) {
    long double LIMIT = 10000000;
    int opt;
    char *end;

    while (-1 != (opt = getopt(argc, argv, "n:"))) {
        switch (opt) {
            case 'n':
                LIMIT = strtold(optarg, &end);
                if (LIMIT > LDBL_MAX || (errno == ERANGE && LIMIT == LDBL_MAX))
                    return 10;
                if (errno == ERANGE && LIMIT == 0)
                    return 11;
                if (*end != '\0')
                    return 12;
                break;
            default:
                fprintf(stderr, "unexpected argument: %d\n", optopt);
                return 1;
        }
    }

    long double local_data;
    long double result;
    double start_time;
    double end_time;
    srand((unsigned int) time(NULL));

    int world_rank;
    int world_size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    long double n = LIMIT / (long double) world_size;
    MPI_Barrier(MPI_COMM_WORLD);

    start_time = MPI_Wtime();
    local_data = estimate_pi(n);
    MPI_Reduce(&local_data, &result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    if (world_rank == 0) {
        printf("[Process %d]: has in %6f the result: %Le\n", world_rank, end_time - start_time, result / LIMIT);
    }

    MPI_Finalize();
    return 0;
}

#pragma clang diagnostic pop