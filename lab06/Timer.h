//
// Created by xgg on 3 Jun 2022.
//

#ifndef LAB06_TIMER_H
#define LAB06_TIMER_H

#include <iostream>

typedef int (*add_vec)(const double *, const double *, double *, const int, const int);

struct AddArgs {
    const double *h_a;
    const double *h_b;
    double *h_c;
    const int n;
    const int block_size;
};

class Timer {
public:
    virtual void start();

    virtual void stop();

    virtual float elapsed();
};

class Timeit {
public:
    static float timeit(add_vec f, AddArgs addArgs, Timer timer);
};

float Timeit::timeit(add_vec f, AddArgs addArgs, Timer timer) {
    int result;
    timer.start();
    if(0 != (result = f(addArgs.h_a, addArgs.h_b, addArgs.h_c, addArgs.n, addArgs.block_size))) {
        std::cerr << "Error while adding vectors, code: " << result << std::endl;
    }
    timer.stop();
    return timer.elapsed();
}

#endif //LAB06_TIMER_H
