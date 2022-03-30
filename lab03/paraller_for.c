//
// Created by xgg on 29/03/2022.
//
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <getopt.h>


int main(int argc, char *argv[]) {

    double start; double delta;
    int opt;
    char *end;

    int threads = 1;
    while (-1 != (opt = getopt(argc, argv, "n:"))) {
        switch (opt) {
            case 'n':
                threads = (int) strtol(optarg, &end, 10);
                break;
            default:
                fprintf(stderr, "unexpected argument: %d\n", optopt);
                return 1;
        }
    }
    omp_set_num_threads(threads);

    const int arr_size = 100000;
    int nthreads; int tid;
    int *i_tab = malloc(sizeof(int) * arr_size);

    start = omp_get_wtime();

#pragma omp parallel default(none) private(tid) shared(nthreads, i_tab) //  num_threads(threads)
{
        tid = omp_get_thread_num();
        if (tid == 0) {
            nthreads = omp_get_num_threads();
        }
#pragma omp for
        for (int i = 0; i < arr_size; ++i) {
            i_tab[i] = i;
        }
}

    delta = omp_get_wtime() - start;

    printf("n=%d:delta=%f\n", nthreads, delta);

    return 0;
}