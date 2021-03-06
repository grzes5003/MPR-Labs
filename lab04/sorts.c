//
// Created by xgg on 5 Apr 2022.
//

#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <stdio.h>
#include "algs.h"

int sort_v1(item_t *array, int32_t arr_size, int32_t n_buckets, struct bucket *buckets, unsigned int thread_idx,
            int32_t num_of_threads) {

    double start;
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

#pragma omp master
    start = omp_get_wtime();
    for (unsigned int i = thread_idx; i < arr_size + thread_idx; ++i) {
        j = array[i % arr_size] / width;
        if (j > n_buckets - 1)
            j = n_buckets - 1;
        if (j >= thread_idx * buckets_per_thread && j < (thread_idx + 1) * buckets_per_thread)
            buckets[j].arr[buckets[j].n_elem++] = array[i % arr_size];
    }
#pragma omp master
    printf("t_bck=%f:", omp_get_wtime() - start);

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

#pragma omp master
    start = omp_get_wtime();
#pragma omp for schedule(static, 1) nowait
    for (int i = 0; i < n_buckets; ++i) {
        qsort(buckets[i].arr, buckets[i].n_elem, sizeof(item_t), cmp);
    }
#pragma omp master
    printf("t_sort=%f:", omp_get_wtime() - start);

#pragma omp master
    start = omp_get_wtime();
#pragma omp for schedule(static, 1) nowait
    for (int i = 0; i < n_buckets; ++i) {
        item_t *dest = array;
        for (int k = 0; k < i; ++k) {
            dest += buckets[k].n_elem;
        }
        memcpy(dest, buckets[i].arr, sizeof(item_t) * buckets[i].n_elem);
    }
#pragma omp master
    printf("t_cpy=%f:", omp_get_wtime() - start);

#pragma omp master
    start = omp_get_wtime();
#pragma omp for schedule(static, 1) nowait
    for (int i = 0; i < n_buckets; ++i) {
        free(buckets[i].arr);
        omp_destroy_lock(&buckets[i].lock);
    }
#pragma omp master
    printf("t_clean=%f:", omp_get_wtime() - start);

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


int sort_v2(item_t *array, int32_t arr_size, int32_t n_buckets, struct bucket *buckets) {

    double start;
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

#pragma omp master
    start = omp_get_wtime();
#pragma omp for
    for (int i = 0; i < arr_size; ++i) {
        j = array[i] / width;
        if (j > n_buckets - 1)
            j = n_buckets - 1;
        omp_set_lock(&buckets[j].lock);
            buckets[j].arr[buckets[j].n_elem++] = array[i];
        omp_unset_lock(&buckets[j].lock);
    }
#pragma omp master
    printf("t_bck=%f:", omp_get_wtime() - start);

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

#pragma omp master
    start = omp_get_wtime();
#pragma omp for schedule(static, 1)
    for (int i = 0; i < n_buckets; ++i) {
        qsort(buckets[i].arr, buckets[i].n_elem, sizeof(item_t), cmp);
    }
#pragma omp master
    printf("t_sort=%f:", omp_get_wtime() - start);

#pragma omp master
    start = omp_get_wtime();
#pragma omp for schedule(static, 1)
    for (int i = 0; i < n_buckets; ++i) {
        item_t *dest = array;
        for (int k = 0; k < i; ++k) {
            dest += buckets[k].n_elem;
        }
        memcpy(dest, buckets[i].arr, sizeof(item_t) * buckets[i].n_elem);
    }
#pragma omp master
    printf("t_cpy=%f:", omp_get_wtime() - start);

#pragma omp master
    start = omp_get_wtime();
#pragma omp for schedule(static, 1)
    for (int i = 0; i < n_buckets; ++i) {
        free(buckets[i].arr);
        omp_destroy_lock(&buckets[i].lock);
    }
#pragma omp master
    printf("t_clean=%f:", omp_get_wtime() - start);

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
