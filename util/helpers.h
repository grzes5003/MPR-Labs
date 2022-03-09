//
// Created by xgg on 9 Mar 2022.
//

#ifndef LAB01_HELPERS_H
#define LAB01_HELPERS_H

#define BUFSIZE 128

int msleep(long msec);

struct t_env_vars {
    long variant;
    int msg_size;
    int n;
};

int read_env(struct t_env_vars *str);

#endif //LAB01_HELPERS_H
