//
// Created by xgg on 8 Mar 2022.
//

#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#define BUFSIZE 128

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

double elapse_time(void (*f)(int), int arg) {
    double starttime, endtime;
    starttime = MPI_Wtime();
    (*f)(arg);
    endtime   = MPI_Wtime();
    printf("That took %f seconds\n",endtime-starttime);
    return endtime-starttime;
}


void send(int world_rank) {
    int number;
    if (world_rank == 0) {
        number = -1;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (world_rank == 1) { // world_rank should be eq to 1
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);
    }
}


void bsend(int world_rank) {
    char *buf = malloc(BUFSIZE);
    MPI_Buffer_attach( buf, BUFSIZE );

    int number;
    if (world_rank == 0) {
        number = -1;
        MPI_Bsend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (world_rank == 1) { // world_rank should be eq to 1
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);
    }
    // TODO segfault here
    MPI_Buffer_detach(&buf,  &(int) {BUFSIZE});
}


void ssend(int world_rank) {
    int number;
    if (world_rank == 0) {
        number = -1;
        MPI_Ssend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (world_rank == 1) { // world_rank should be eq to 1
        msleep(500);
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);
    }
}


int main(int argc, char *argv[]) {

    // get variant
    long variant;
    char path[BUFSIZE];
    char *envvar = "VARIANT";
    if(!getenv(envvar)){
        variant = 1;
    } else if (snprintf(path, BUFSIZE, "%s", getenv(envvar)) >= BUFSIZE) {
        fprintf(stderr, "BUFSIZE of %d was too small. Aborting\n", BUFSIZE);
        exit(1);
    } else if (0L == (variant = strtol(path, NULL, 10))) {
        fprintf(stderr, "Error when parsing\n");
        exit(1);
    }


    int len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int world_rank, world_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    if (variant == 1) printf("Starting variant %ld\n", variant);

    // We are assuming at least 2 processes for this task
    if (world_size < 2) {
        fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    MPI_Get_processor_name(hostname, &len);
    printf("Process %d of %d, name: %s\n", world_rank, world_size, hostname);

    // init phase finished
    if (world_rank == 1) printf("/////////////////////\n");
    MPI_Barrier(MPI_COMM_WORLD);

    if (variant == 1) {
        elapse_time(send, world_rank);
    } else if (variant == 2) {
        elapse_time(ssend, world_rank);
    } else if (variant == 3) {
        elapse_time(bsend, world_rank);
    }

    MPI_Finalize();
    return 0;
}
