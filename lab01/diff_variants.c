//
// Created by xgg on 8 Mar 2022.
//

#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>

int msleep(long msec) {
    struct timespec ts;
    int res;

    if (msec < 0) {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}


void send(int rank) {
    int idx;
    if (rank == 0) {
        idx = -1;
        MPI_Send(&idx, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else { // rank should be eq to 1
        MPI_Recv(&idx, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 1 received idx %d from process 0\n", idx);
    }
}


void ssend(int rank) {
    int idx;
    if (rank == 0) {
        idx = -1;
        MPI_Ssend(&idx, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else { // rank should be eq to 1
        msleep(500);
        MPI_Recv(&idx, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received idx %d from process 0\n", idx);
    }
}


int main(int argc, char *argv[]) {
    const int8_t V = 1;
    int len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    int world_rank, world_size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // We are assuming at least 2 processes for this task
    if (world_size < 2) {
        fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    MPI_Get_processor_name(hostname, &len);
    printf("Hello world from process %d of %d, name: %s\n", world_rank, world_size, hostname);

    printf("Starting variant %d\n", V);
    if (V == 1) {
        send(world_rank);
    } else if (V == 2) {
        ssend(world_rank);
    }

    MPI_Finalize();
    return 0;
}
