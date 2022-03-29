//
// Created by xgg on 29/03/2022.
//
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <limits.h>
#include <errno.h>
#include <float.h>


int main(int argc, char *argv[]) {

    double start, delta;
    int opt;
    char *end;

    long threads = 1;
    while (-1 != (opt = getopt(argc, argv, "n:"))) {
        switch (opt) {
            case 'n':
                threads = strtol(optarg, &end, 10);
                omp_set_num_threads(threads);
                break;
            default:
                fprintf(stderr, "unexpected argument: %d\n", optopt);
                return 1;
        }
    }

    const int arr_size = 1000;
    int nthreads, tid;
    int *i_tab = malloc(sizeof(int) * arr_size);

    start = omp_get_wtime();

#pragma omp parallel for private(nthreads, tid)
{
    tid = omp_get_thread_num();
    for (int i = 0; i < arr_size; ++i) {
        i_tab[i] = i;
    }
}
    delta = omp_get_wtime() - start;

    printf("n=%ld:delta=%f\n", threads, delta);

///* Fork a team of threads giving them their own copies of variables */
//#pragma omp parallel private(nthreads, tid)
//    {
//
//        /* Obtain thread number */
//        tid = omp_get_thread_num();
//        printf("Hello World from thread = %d\n", tid);
//
//        /* Only master thread does this */
//        if (tid == 0)
//        {
//            nthreads = omp_get_num_threads();
//            printf("Number of threads = %d\n", nthreads);
//        }
//
//    } /* All threads join master thread and disband */

}