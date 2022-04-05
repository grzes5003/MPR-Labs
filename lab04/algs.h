//
// Created by xgg on 5 Apr 2022.
//

#ifndef LAB04_SEQ_BUCKET_H
#define LAB04_SEQ_BUCKET_H

#include <stdint-gcc.h>
#include <sys/types.h>

typedef int32_t item_t;

struct bucket {
    int n_elem;
    int index; // [start : n_elem)
    int start; //starting point in B array
};

int cmp(const void *a, const void *b);

int sort_v1(item_t* array);

int rand_arr(item_t* array, int32_t arr_size, u_int tid);

#endif //LAB04_SEQ_BUCKET_H
