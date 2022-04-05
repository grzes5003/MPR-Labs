//
// Created by xgg on 5 Apr 2022.
//

#include <getopt.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "params.h"

int get_params(param* _params, int argc, char *argv[]) {
    int opt;
    char *end;

    long tmp;
    while (-1 != (opt = getopt(argc, argv, "a:t:n:"))) {
        switch (opt) {
            case 'a':
                tmp = strtol(optarg, &end, 10);
                if (tmp > SCHAR_MAX || (errno == ERANGE && tmp == SCHAR_MAX))
                    return 10;
                if (errno == ERANGE && tmp == 0)
                    return 11;
                if (*end != '\0')
                    return 12;
                _params->alg = (int8_t) tmp;
                break;
            case 't':
                tmp = (int) strtol(optarg, &end, 10);
                if (tmp > SCHAR_MAX || (errno == ERANGE && tmp == SCHAR_MAX))
                    return 10;
                if (errno == ERANGE && tmp == 0)
                    return 11;
                if (*end != '\0')
                    return 12;
                _params->threads = (int8_t) tmp;
                break;
            case 'n':
                tmp = (int) strtol(optarg, &end, 10);
                if (tmp > INT_MAX || (errno == ERANGE && tmp == INT_MAX))
                    return 10;
                if (errno == ERANGE && tmp == 0)
                    return 11;
                if (*end != '\0')
                    return 12;
                _params->arr_size = (int32_t) tmp;
                break;
            default:
                fprintf(stderr, "unexpected argument: %d\n", optopt);
                return 1;
        }
    }

    return 0;
}