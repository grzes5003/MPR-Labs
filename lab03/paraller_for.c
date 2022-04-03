//
// Created by xgg on 29/03/2022.
//
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <limits.h>
#include "lehmer64.h"


int main(int argc, char *argv[]) {

    double start;
    int opt;
    char *end;

    int threads = 1;
    int arr_size = 100000;
    int b_variant = 0;
    int i_variant = -1;
    // -n size of array
    // -t num of threads
    // -d variant dynamic: default static
    // -c chunk size
    while (-1 != (opt = getopt(argc, argv, "n:t:dc:"))) {
        switch (opt) {
            case 'n':
                arr_size = (int) strtol(optarg, &end, 10);
                if (arr_size > INT_MAX || (errno == ERANGE && arr_size == INT_MAX))
                    return 10;
                if (errno == ERANGE && arr_size == 0)
                    return 11;
                if (*end != '\0')
                    return 12;
                break;
            case 't':
                threads = (int) strtol(optarg, &end, 10);
                if (threads > INT_MAX || (errno == ERANGE && threads == INT_MAX))
                    return 10;
                if (errno == ERANGE && threads == 0)
                    return 11;
                if (*end != '\0')
                    return 12;
                break;
            case 'c':
                i_variant = (int) strtol(optarg, &end, 10);
                if (i_variant > INT_MAX || (errno == ERANGE && i_variant == INT_MAX))
                    return 10;
                if (errno == ERANGE && i_variant == 0)
                    return 11;
                if (*end != '\0')
                    return 12;
                break;
            case 'd':
                b_variant = 1;
                break;
            default:
                fprintf(stderr, "unexpected argument: %d\n", optopt);
                return 1;
        }
    }
    omp_set_num_threads(threads);

    int nthreads = -1;
    int tid;
    int32_t range = 1000;
    int32_t *i_tab = malloc(sizeof(int32_t) * arr_size);

    if (b_variant) {
        omp_set_schedule(omp_sched_dynamic, i_variant);
    } else {
        omp_set_schedule(omp_sched_static, i_variant);
    }

    omp_sched_t kind;
    int chunk;
    omp_get_schedule(&kind, &chunk);

    double delta = 0; const int reruns = 100;
    for (int iter = 0; iter < reruns; ++iter) {

        start = omp_get_wtime();

#pragma omp parallel default(none) firstprivate(g_lehmer64_state)\
private(tid) shared(nthreads, i_tab, arr_size, range) //  num_threads(threads)
        {
            tid = omp_get_thread_num();
            lehmer64_seed(tid);
            if (tid == 0) {
                nthreads = omp_get_num_threads();
            }
#pragma omp for schedule(runtime)
            for (int i = 0; i < arr_size; ++i) {
                i_tab[i] = (int32_t) (lehmer64() % range);
//            i_tab[i] = i;
            }
        }
        delta += omp_get_wtime() - start;
    }


    printf("t=%d:delta=%f:d=%d:c=%d:n=%d:one=%f\n", nthreads, delta, kind, chunk, arr_size, delta / reruns);
    free(i_tab);

    return 0;
}