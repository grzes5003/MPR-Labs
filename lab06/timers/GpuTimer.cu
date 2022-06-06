//
// Created by xgg on 2 Jun 2022.
//

#include "GpuTimer.cuh"

struct PrivateTimingGPU {
    cudaEvent_t     _start;
    cudaEvent_t     _stop;
};

GpuTimer::GpuTimer() {
    privateTimingGPU = new PrivateTimingGPU;

    cudaEventCreate(&(*privateTimingGPU)._start);
    cudaEventCreate(&(*privateTimingGPU)._stop);
}

GpuTimer::~GpuTimer() {
    cudaEventDestroy((*privateTimingGPU)._start);
    cudaEventDestroy((*privateTimingGPU)._stop);
}

void GpuTimer::start() {
    cudaEventRecord((*privateTimingGPU)._start, nullptr);
}

void GpuTimer::stop() {
    cudaEventRecord((*privateTimingGPU)._stop, nullptr);
}

float GpuTimer::elapsed() {
    float elapsed;
    cudaEventSynchronize((*privateTimingGPU)._stop);
    cudaEventElapsedTime(&elapsed, (*privateTimingGPU)._start, (*privateTimingGPU)._stop);
    return elapsed;
}
