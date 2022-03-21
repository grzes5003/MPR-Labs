//
// Created by xgg on 10 Mar 2022.
//
#include <mpi.h>
#include <getopt.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>

#define N 10000

double elapse_time(void (*f)(int, char *, unsigned int), int world_rank, unsigned int msg_size) {
    char *msg = calloc(msg_size, sizeof(char));
    double start_time;
    double end_time;

    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();
    for (int i = 0; i < N; i++) {
        (*f)(world_rank, msg, msg_size);
    }
    end_time = MPI_Wtime();
    if (world_rank == 0)
        printf("%d:%f:%f\n", msg_size, end_time - start_time, (end_time - start_time) / (double) N);
    free(msg);

    return end_time - start_time;
}


void send(int world_rank, char *msg, unsigned int msg_size) {
    if (world_rank == 0) {
        MPI_Send(msg, (int) msg_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
    } else if (world_rank == 1) { // world_rank should be eq to 1
        MPI_Recv(msg, (int) msg_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }
}


void ssend(int world_rank, char *msg, unsigned int msg_size) {
    if (world_rank == 0) {
        MPI_Ssend(msg, (int) msg_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
    } else if (world_rank == 1) { // world_rank should be eq to 1
        MPI_Recv(msg, (int) msg_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}


int main(int argc, char *argv[]) {
    unsigned int variant = 0;
    unsigned int msg_size = 1;
    int opt;
    char *end;

    while (-1 != (opt = getopt(argc, argv, "s:v:"))) {
        switch (opt) {
            case 'v':
                variant = (unsigned int) strtol(optarg, &end, 10);
                if (variant > INT_MAX || (errno == ERANGE && variant == INT_MAX))
                    return 10;
                if (errno == ERANGE && variant == 0)
                    return 11;
                if (*end != '\0')
                    return 12;
                break;
            case 's':
                msg_size = (unsigned int) strtol(optarg, &end, 10);
                if (msg_size > INT_MAX || (errno == ERANGE && msg_size == INT_MAX))
                    return 10;
                if (errno == ERANGE && msg_size == 0)
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

    if (variant == 0) {
        elapse_time(send, world_rank, msg_size);
    } else if (variant == 1) {
        elapse_time(ssend, world_rank, msg_size);
    }

    MPI_Finalize();
    return 0;
}