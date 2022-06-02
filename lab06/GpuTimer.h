//
// Created by xgg on 2 Jun 2022.
//

#ifndef LAB06_GPUTIMER_H
#define LAB06_GPUTIMER_H


#include "Timer.h"

class GpuTimer : public Timer {
    cudaEvent_t _start;
    cudaEvent_t _stop;
public:
    GpuTimer();
    ~GpuTimer();
    void start();
    void stop();
    float elapsed();
};


#endif //LAB06_GPUTIMER_H
