//
// Created by xgg on 5 Apr 2022.
//

#include <omp.h>
#include <stdlib.h>
#include "algs.h"

int rand_arr(item_t* array, int32_t arr_size, u_int tid) {
#pragma omp for
    for (int i = 0; i < arr_size; ++i) {
        array[i] = rand_r(&tid) % arr_size;
    }
    return 0;
}

int cmp(const void *a, const void *b) {
    return (*(item_t *) a - *(item_t *) b);
}

int validate(const item_t* array, int32_t arr_size) {
    for (int i = 1; i < arr_size; ++i) {
        if (array[i] - array[i-1] < 0 ) return 1;
    }
    return 0;
}