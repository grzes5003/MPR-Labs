//
// Created by xgg on 5 Apr 2022.
//

#include <stdio.h>
#include <malloc.h>
#include <omp.h>
#include "params.h"
#include "algs.h"

int main(int argc, char *argv[]) {
    double start;
    double end;

    const int32_t ARR_DEFAULT_SIZE = 1000000;
    const int16_t DEFAULT_N_BUCKETS = 4;
    const int8_t DEFAULT_THREADS = 4;
    const int8_t DEFAULT_ALG = 1;

    int result;
    param _param = {DEFAULT_ALG,
                    DEFAULT_THREADS,
                    DEFAULT_N_BUCKETS,
                    ARR_DEFAULT_SIZE};
    if (0 != (result = get_params(&_param, argc, argv))) {
        fprintf(stderr, "Error parsing %d", result);
        return 1;
    }

    unsigned int tid;
    int nthreads = -1;

    // allocate buckets
    struct bucket *buckets;
    buckets = (struct bucket *) calloc(_param.n_buckets, sizeof(struct bucket));
    for (int i = 0; i < _param.n_buckets; ++i) {
        buckets[i].arr = (item_t *) calloc(_param.arr_size, sizeof(item_t));
        omp_init_lock(&buckets[i].lock);
    }

    omp_set_num_threads(_param.threads);

    // allocate array
    item_t *array = malloc(sizeof(int32_t) * _param.arr_size);

    start = omp_get_wtime();
#pragma omp parallel default(none) private(tid) shared(nthreads, array, _param, buckets, result, stderr)
    {
        tid = omp_get_thread_num();
        if (tid == 0)
            nthreads = omp_get_num_threads();
        rand_arr(array, _param.arr_size, tid);

        if (_param.alg == 1) {
            if (0 !=(result = sort_v1(array, _param.arr_size, _param.n_buckets,
                    buckets, tid, _param.threads))) {
                fprintf(stderr, "Bad number of buckets to threads (CODE %d)\n", result);
            }
        }
        else sort_v2(array, _param.arr_size, _param.n_buckets, buckets);
    }
    end = omp_get_wtime();

    printf("t=%d:alg=%d:arr=%d:b=%d:time=%f\n",
           nthreads, _param.alg,
           _param.arr_size, _param.n_buckets, end - start);

    if (0 != (result = validate(array, _param.arr_size))) {
        fprintf(stderr, "Array not sorted properly (CODE %d)", result);
        return 1;
    }
    printf("Ok");
    free(array);

    return 0;
}
