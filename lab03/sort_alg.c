//
// Created by xgg on 05/04/2022.
//
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <limits.h>


#define THREADS 4


struct bucket {
    int n_elem;
    int index; // [start : n_elem)
    int start; //starting point in B array
};

int cmpfunc(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}


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
    omp_set_num_threads(THREADS);

    int nthreads = -1;
    unsigned int tid;
    int32_t range = 1000;
    int32_t *i_tab = malloc(sizeof(int32_t) * arr_size);

    int n_buckets = (int) arr_size / 100;

    struct bucket *buckets;
    buckets = (struct bucket *) calloc(n_buckets, sizeof(struct bucket));

#pragma omp parallel default(none) private(tid) shared(nthreads, i_tab, arr_size, range)
    {
        tid = omp_get_thread_num();
        if (tid == 0)
            nthreads = omp_get_num_threads();
#pragma omp for
        for (int i = 0; i < arr_size; ++i) {
            i_tab[i] = (rand_r(&tid) % range);
        }

        // divide into buckets:
        // v1 read whole array, with offset
        // v2 read chunks
#pragma omp for
        for (int i = 0; i < arr_size; i++) {
            local_index = A[i] / w;
            if (local_index > n_buckets - 1)
                local_index = n_buckets - 1;
            real_bucket_index = local_index + my_id * n_buckets;
            buckets[real_bucket_index].n_elem++;
        }


#pragma omp barrier

        //

        ////
    }
    delta += omp_get_wtime() - start;


    return 0;
}
