//
// Created by xgg on 5 Apr 2022.
//

#ifndef LAB04_SEQ_BUCKET_H
#define LAB04_SEQ_BUCKET_H

#include <stdint-gcc.h>
#include <sys/types.h>

//#define DEBUG

typedef int32_t item_t;

struct bucket {
    int n_elem;
    item_t* arr;
};

int cmp(const void *a, const void *b);

int sort_v1(item_t* array, int32_t arr_size);

int sort_v2(item_t *array, int32_t arr_size, struct bucket *buckets, omp_lock_t *lock);

int rand_arr(item_t* array, int32_t arr_size, u_int tid);

int validate(const item_t* array, int32_t arr_size);

#endif //LAB04_SEQ_BUCKET_H
