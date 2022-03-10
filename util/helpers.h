//
// Created by xgg on 9 Mar 2022.
//

#ifndef LAB01_HELPERS_H
#define LAB01_HELPERS_H



int msleep(long msec);

struct t_env_vars {
    long variant;
    long msg_size;
    long n;
};

int read_env(struct t_env_vars *str);

#endif //LAB01_HELPERS_H
