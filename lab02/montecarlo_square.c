//
// Created by xgg on 15/03/2022.
//
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


float randomFloat() {
    return (float)rand()/(float)(RAND_MAX/1);
}


float estimate_pi(int n) {
    int i;
    float in = 0.f, out = 0.f;
    for (i = 0; i < n; i++) {
        float x = randomFloat();
        float y = randomFloat();
        if (sqrtf(x * x + y * y) <= 1) {
            in += 1;
        } else {
            out += 1;
        }
    }

    return 4.f * out / in;
}


int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));
    printf("%.6f", estimate_pi(1000));
    return 0;
}

