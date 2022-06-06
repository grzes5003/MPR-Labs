//
// Created by xgg on 2 Jun 2022.
//

#ifndef LAB06_GPUTIMER_CUH
#define LAB06_GPUTIMER_CUH


#include "Timer.hpp"

struct PrivateTimingGPU;

class GpuTimer : public virtual Timer {
    PrivateTimingGPU *privateTimingGPU;
//    cudaEvent_t _start;
//    cudaEvent_t _stop;
public:
    GpuTimer();
    ~GpuTimer();
    void start() override;
    void stop() override;
    float elapsed() override;
};


#endif //LAB06_GPUTIMER_CUH
