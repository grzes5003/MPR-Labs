//
// Created by xgg on 12 Jun 2022.
//

#ifndef IMAGE_SCALING_GPUTIMER_H
#define IMAGE_SCALING_GPUTIMER_H


struct PrivateTimingGPU;

class GpuTimer {
    PrivateTimingGPU *privateTimingGPU;
public:
    GpuTimer();
    ~GpuTimer();
    void start();
    void stop();
    float elapsed();
};



#endif //IMAGE_SCALING_GPUTIMER_H
