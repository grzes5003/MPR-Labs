//
// Created by xgg on 9 Mar 2022.
//
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "helpers.h"

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


int read_env(struct t_env_vars *str) {
    char path[BUFSIZE];
    memset(path, 0, sizeof path);

    char *env_variant = "VARIANT";
    char *msg_size = "MSG_SIZE";
    char *n_times = "N_TIMES";

    if(!getenv(env_variant)){
        str->variant = 1;
    } else if (snprintf(path, BUFSIZE, "%s", getenv(env_variant)) >= BUFSIZE) {
        fprintf(stderr, "BUFSIZE of %d was too small. Aborting\n", BUFSIZE);
        return 1;
    } else if (0L == (str->variant = strtol(path, NULL, 10))) {
        fprintf(stderr, "Error when parsing\n");
        return 1;
    }

    memset(path, 0, sizeof path);
    if(!getenv(msg_size)){
        str->msg_size = DEFAULT_MSG_SIZE;
    } else if (snprintf(path, BUFSIZE, "%s", getenv(msg_size)) >= BUFSIZE) {
        fprintf(stderr, "MSG_SIZE of %d was too small. Aborting\n", BUFSIZE);
        return 1;
    } else if (0L == (str->msg_size = strtol(path, NULL, 10))) {
        fprintf(stderr, "Error when parsing\n");
        return 1;
    }

    memset(path, 0, sizeof path);
    if(!getenv(n_times)){
        str->n = DEFAULT_N_TIMES;
    } else if (snprintf(path, BUFSIZE, "%s", getenv(n_times)) >= BUFSIZE) {
        fprintf(stderr, "n_times of %d was too small. Aborting\n", BUFSIZE);
        return 1;
    } else if (0L == (str->n = strtol(path, NULL, 10))) {
        fprintf(stderr, "Error when parsing\n");
        return 1;
    }

    return 0;
}