//
// Created by xgg on 10 Mar 2022.
//
#include <mpi.h>
#include <getopt.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>

#define N 1000

double elapse_time(void (*f)(int, char *), int world_rank, unsigned int msg_size) {
    char *msg = malloc(msg_size * sizeof(char));
    double start_time;
    double end_time;

    start_time = MPI_Wtime();
    for (int i = 0; i < N; i++) {
        (*f)(world_rank, msg);
    }
    end_time = MPI_Wtime();
    printf("%d:%f:%f\n", msg_size, end_time - start_time, (end_time - start_time) / (double) N);
    return end_time - start_time;
}


void send(int world_rank, char *msg) {
    if (world_rank == 0) {
        MPI_Send(msg, 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    } else if (world_rank == 1) { // world_rank should be eq to 1
        MPI_Recv(msg, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }
}


void ssend(int world_rank, char *msg) {
    if (world_rank == 0) {
        MPI_Ssend(msg, 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    } else if (world_rank == 1) { // world_rank should be eq to 1
        MPI_Recv(msg, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}


int main(int argc, char *argv[]) {
    int variant = 0;
    unsigned int msg_size = 1;
    int opt;
    char *end;

    while (-1 != (opt = getopt(argc, argv, "vs:"))) {
        switch (opt) {
            case 's':
                msg_size = (unsigned int) strtol(optarg, &end, 10);
                if (msg_size > INT_MAX || (errno == ERANGE && msg_size == INT_MAX))
                    return 10;
                if (errno == ERANGE && msg_size == 0)
                    return 11;
                if (*end != '\0')
                    return 12;
                break;
            case 'v':
                variant = (int) strtol(optarg, &end, 10);
                if (variant > INT_MAX || (errno == ERANGE && variant == INT_MAX))
                    return 10;
                if (variant < INT_MIN || (errno == ERANGE && variant == INT_MIN))
                    return 11;
                if (*end != '\0')
                    return 12;
                break;
            default:
                fprintf(stderr, "unexpected argument: %d\n", optopt);
                return 1;
        }
    }

    int world_rank;
    int world_size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Barrier(MPI_COMM_WORLD);

    printf("variant=%d", variant);
    if (variant == 0) {
        elapse_time(send, world_rank, msg_size);
    } else if (variant == 1) {
        elapse_time(ssend, world_rank, msg_size);
    }

    MPI_Finalize();
    return 0;
}