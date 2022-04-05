//
// Created by xgg on 05/04/2022.
//

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

    double start, end_time;
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

    int *A, *B, *temp;
    int nthreads = -1;
    unsigned int tid;
    int32_t range = arr_size;
    int32_t *i_tab = malloc(sizeof(int32_t) * arr_size);

    int n_buckets = (int) arr_size / 5;
    int w = range / n_buckets;
    int j;
    int b_index;

    struct bucket *buckets;
    buckets = (struct bucket *) calloc(n_buckets, sizeof(struct bucket));


    w = (int) range / n_buckets;
    A = (int *) malloc(sizeof(int) * arr_size);
    B = (int *) malloc(sizeof(int) * arr_size);

    int local_index; // [0 : n_buckets)
    int real_bucket_index; // [0 : n_buckets * num_threads)

    tid = 0;
    start = omp_get_wtime();
//#pragma omp parallel default(none) private(tid) shared(nthreads, i_tab, arr_size, range)
    for (int i = 0; i < arr_size; ++i) {
//        i_tab[i] = (rand_r(&tid) % range);
        i_tab[i] = i;
    }
    i_tab[0] = 5;
    i_tab[5] = 10;

    // divide into buckets:
    // v1 read whole array, with offset
    // == v2 read chunks

    for (int i = 0; i < arr_size; i++) {
        j = i_tab[i] / w;
        if (j > n_buckets - 1)
            j = n_buckets - 1;
        buckets[j].n_elem++;
    }

    //buckets[0].index=0; //bucket 0 starts from 0 in B, bucket 1 starts from the start of bucket 0 + the number of elements in bucket 0 ...
    //buckets[0].start=0;
    for (int i = 1; i < n_buckets; i++) {
        buckets[i].index = buckets[i - 1].index + buckets[i - 1].n_elem;
        buckets[i].start = buckets[i - 1].start + buckets[i - 1].n_elem;
    }

    for (int i = 0; i < arr_size; i++) {
        j = i_tab[i] / w;
        if (j > n_buckets - 1)
            j = n_buckets - 1;
        b_index = buckets[j].index++;
        B[b_index] = A[i];
    }
//}
    end_time = omp_get_wtime();

    printf("delta=%f\n", end_time - start);

}
