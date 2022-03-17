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


float estimate_pi(unsigned long long n) {
    float in = 0.f;

    for (unsigned long long i = 0; i < n; i++) {
        float x = (float) rand() / (float) (RAND_MAX);
        float y = (float) rand() / (float) (RAND_MAX);
        if (x * x + y * y <= 1) in += 1;
    }

    return 4.f * in;
}


int main(int argc, char *argv[]) {
    unsigned long long LIMIT = 10000000;
    int opt;
    char *end;

    while (-1 != (opt = getopt(argc, argv, "n:"))) {
        switch (opt) {
            case 'n':
                LIMIT = strtoull(optarg, &end, 10);
                if (LIMIT > ULLONG_MAX || (errno == ERANGE && LIMIT == ULLONG_MAX))
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

    float local_data;
    float result;
    double start_time;
    double end_time;
    srand((unsigned int) time(NULL));

    int world_rank;
    int world_size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    unsigned long long n = (LIMIT / (unsigned long long) world_size);
    MPI_Barrier(MPI_COMM_WORLD);

    start_time = MPI_Wtime();
    local_data = estimate_pi(n);
    MPI_Reduce(&local_data, &result, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    if (world_rank == 0) {
        printf("[Process %d]: has in %6f the result: %llu\n", world_rank, end_time - start_time, (unsigned long long) result / LIMIT);
    }

    MPI_Finalize();
    return 0;
}

#pragma clang diagnostic pop