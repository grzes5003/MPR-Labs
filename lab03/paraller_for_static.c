//
// Created by xgg on 5 Apr 2022.
//

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
    unsigned short int tid[3] = {0, 0, 0};
    int32_t *i_tab = malloc(sizeof(int32_t) * arr_size);

    omp_sched_t kind;
    int chunk;
    omp_get_schedule(&kind, &chunk);

    double delta = 0;
    const int reruns = 10;


    start = omp_get_wtime();
    int i = 0;

#pragma omp parallel default(none) firstprivate(tid) shared(nthreads, i_tab, arr_size, i)
    {
        tid[0] = (unsigned short int) omp_get_thread_num();
        if (tid[0] == 0)
            nthreads = omp_get_num_threads();
#pragma omp for
        for (i = 0; i < arr_size; ++i) {
            i_tab[i] = (int32_t) (erand48(tid) * 100);
        }
    }
    delta += omp_get_wtime() - start;


    printf("t=%d:delta=%f:d=%d:c=%d:n=%d:one=%f\n", nthreads, delta, kind, chunk, arr_size, delta / reruns);
    free(i_tab);

    return 0;
}