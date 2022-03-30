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

    double start; double delta;
    int opt;
    char *end;
    lehmer64_seed(0);

    int threads = 1; int arr_size = 100000;
    while (-1 != (opt = getopt(argc, argv, "t:n"))) {
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
            default:
                fprintf(stderr, "unexpected argument: %d\n", optopt);
                return 1;
        }
    }
    omp_set_num_threads(threads);

    int nthreads = -1; int tid;
    const int32_t range = 1000;
    int *i_tab = malloc(sizeof(int) * arr_size);

    start = omp_get_wtime();

#pragma omp parallel default(none) private(tid) shared(nthreads, i_tab, arr_size) //  num_threads(threads)
{
        tid = omp_get_thread_num();
        if (tid == 0) {
            nthreads = omp_get_num_threads();
        }
#pragma omp for
        for (int i = 0; i < arr_size; ++i) {
            i_tab[i] = (int32_t) lehmer64() % range;
        }
}

    delta = omp_get_wtime() - start;

    printf("n=%d:delta=%f\n", nthreads, delta);
    for (int i = 0; i < (arr_size > 100 ? 100 : arr_size); ++i) {
        printf("%d,", i_tab[i]);
    }
    printf("\n");
    free(i_tab);

    return 0;
}