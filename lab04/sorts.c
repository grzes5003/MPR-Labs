//
// Created by xgg on 5 Apr 2022.
//

#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <stdio.h>
#include "algs.h"

int sort_v1(item_t *array, int32_t arr_size, int16_t n_buckets, struct bucket *buckets, unsigned int thread_idx,
            int8_t num_of_threads) {

    int width = arr_size / n_buckets;
    if (n_buckets < num_of_threads)
        return 52;
    if (n_buckets % num_of_threads != 0 )
        return 51;
    int buckets_per_thread = n_buckets / num_of_threads;
    int j;

#if defined DEBUG
    #pragma omp single
    {
        for (int i = 0; i < arr_size; ++i) {
            printf("%d,", array[i]);
        }
    }
    printf("\n");
#endif

    for (unsigned int i = thread_idx; i < arr_size + thread_idx; ++i) {
        j = array[i % arr_size] / width;
        if (j > n_buckets - 1)
            j = n_buckets - 1;
        if (j >= thread_idx * buckets_per_thread && j < (thread_idx + 1) * buckets_per_thread)
            buckets[j].arr[buckets[j].n_elem++] = array[i % arr_size];
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
        omp_destroy_lock(&buckets[i].lock);
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


int sort_v2(item_t *array, int32_t arr_size, int16_t n_buckets, struct bucket *buckets) {

    int width = arr_size / n_buckets;
    int j;

#if defined DEBUG
#pragma omp single
    {
        for (int i = 0; i < arr_size; ++i) {
            printf("%d,", array[i]);
        }
    }
    printf("\n");
#endif

#pragma omp for
    for (int i = 0; i < arr_size; ++i) {
        j = array[i] / width;
        if (j > n_buckets - 1)
            j = n_buckets - 1;
        omp_set_lock(&buckets[j].lock);
            buckets[j].arr[buckets[j].n_elem++] = array[i];
        omp_unset_lock(&buckets[j].lock);
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
        omp_destroy_lock(&buckets[i].lock);
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
