//
// Created by xgg on 3 Jun 2022.
//

#ifndef LAB06_TIMER_HPP
#define LAB06_TIMER_HPP

struct AddArgs {
    const double *h_a;
    const double *h_b;
    double *h_c;
    const int n;
    const int block_size;
};

class Timer {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual float elapsed() = 0;
//    virtual ~Timer() = 0;
};

#endif //LAB06_TIMER_HPP
