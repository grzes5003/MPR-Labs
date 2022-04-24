//
// Created by xgg on 5 Apr 2022.
//

#ifndef LAB04_SEQ_PARAMS_H
#define LAB04_SEQ_PARAMS_H

#include <stdint.h>

typedef struct param {
    int8_t alg;
    int32_t threads;
    int32_t n_buckets;
    int32_t arr_size;
} param;

int get_params(param* _params, int argc, char *argv[]);

#endif //LAB04_SEQ_PARAMS_H
