//
// Created by xgg on 8 Mar 2022.
//

#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include "../util/helpers.h"



double elapse_time(void (*f)(int, int), int world_rank, int msg_size) {
    double starttime, endtime;
    starttime = MPI_Wtime();
    (*f)(world_rank, msg_size);
    endtime   = MPI_Wtime();
    printf("That took %f seconds\n",endtime-starttime);
    return endtime-starttime;
}


void send(int world_rank, int msg_size) {
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


void bsend(int world_rank, int msg_size) {
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


void ssend(int world_rank, int msg_size) {
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
    struct t_env_vars env_vars;
    read_env(&env_vars);

    int len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int world_rank, world_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    if (env_vars.variant == 1) printf("Starting variant %ld\n", env_vars.variant);

    // We are assuming at least 2 processes for this task
    if (world_size < 2) {
        fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    MPI_Get_processor_name(hostname, &len);
    printf("Process %d of %d, name: %s\n", world_rank, world_size, hostname);

    // init phase finished
    if (world_rank == 0) printf("/////////////////////\n");
    MPI_Barrier(MPI_COMM_WORLD);

    uint16_t i;
    for(i = 0; i < env_vars.n; i++) {
        if (env_vars.variant == 1) {
            elapse_time(send, world_rank, env_vars.msg_size);
        } else if (env_vars.variant == 2) {
            elapse_time(ssend, world_rank, env_vars.msg_size);
        } else if (env_vars.variant == 3) {
            elapse_time(bsend, world_rank, env_vars.msg_size);
        }
    }

    MPI_Finalize();
    return 0;
}
