//
// Created by xgg on 2 Jun 2022.
//

#include "GpuTimer.h"

GpuTimer::GpuTimer() {
    cudaEventCreate(&_start);
    cudaEventCreate(&_stop);
}

GpuTimer::~GpuTimer() {
    cudaEventDestroy(_start);
    cudaEventDestroy(_stop);
}

void GpuTimer::start() {
    cudaEventRecord(_start, 0);
}

void GpuTimer::stop() {
    cudaEventRecord(_stop, 0);
}

float GpuTimer::elapsed() {
    float elapsed;
    cudaEventSynchronize(_stop);
    cudaEventElapsedTime(&elapsed, _start, _stop);
    return elapsed;
}
