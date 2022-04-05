//
// Created by xgg on 5 Apr 2022.
//

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "algs.h"

int sort_v1(item_t *array, int32_t arr_size) {
    return 0;
}


int sort_v2(item_t *array, int32_t arr_size, struct bucket *buckets) {
    int limit = arr_size;

    int n_buckets = 4;
    int width = limit / n_buckets;
    int j;
#pragma omp for
    for (int i = 0; i < arr_size; ++i) {
        j = array[i] / width;
        if (j > n_buckets - 1)
            j = n_buckets - 1;
#pragma omp critical
        {
            buckets[j].arr[buckets[j].n_elem++] = array[i];
        }
    }

#if defined DEBUG
#pragma omp single
    {
        // printer
        for (int i = 0; i < n_buckets; ++i) {
            printf("bck=%d::  ", i);
            for (int k = 0; k < buckets[i].n_elem; ++k) {
                printf("%d,", buckets[i].arr[k]);
            }
            printf("\n");
        }
        for (int i = 0; i < arr_size; ++i) {
            printf("%d,", array[i]);
        }
    }
    fflush(stdout);
#endif

#pragma omp for schedule(static, 1)
    for (int i = 0; i < n_buckets; ++i) {
        item_t *dest = array;
        qsort(buckets[i].arr, buckets[i].n_elem, sizeof(item_t), cmp);
        for (int k = 0; k < i; ++k) {
            dest += buckets[k].n_elem;
        }
        memcpy(dest, buckets[i].arr, sizeof(item_t) * buckets[i].n_elem);
        free(buckets[i].arr);
    }

#if defined DEBUG
#pragma omp single
    {
        // printer
        for (int i = 0; i < arr_size; ++i) {
            printf("%d,", array[i]);
        }
        printf("\n");
    }
#endif
    return 0;
}