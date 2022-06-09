//
// Created by xgg on 3 Jun 2022.
//

#include "CpuTimer.hpp"

#ifdef __linux__

#elif _WIN32 || _WIN64

#include <windows.h>
#include <iostream>

struct PrivateTimingCPU {
    double PCFreq;
    __int64 counter_start;
    __int64 counter_stop;
};


void CpuTimer::start() {
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li)) std::cerr << "QueryPerformanceFrequency failed!\n";

    (*privateTimingCpu).PCFreq = double(li.QuadPart) / 1000.0;

    QueryPerformanceCounter(&li);
    (*privateTimingCpu).counter_start = li.QuadPart;
}

void CpuTimer::stop() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    (*privateTimingCpu).counter_stop = li.QuadPart;
}

float CpuTimer::elapsed() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double((*privateTimingCpu).counter_stop - (*privateTimingCpu).counter_start) / (*privateTimingCpu).PCFreq;
}

CpuTimer::CpuTimer() {
    privateTimingCpu = new PrivateTimingCPU{
            0.0, 0
    };
}

#endif
