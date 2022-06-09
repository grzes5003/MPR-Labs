//
// Created by xgg on 3 Jun 2022.
//

#ifndef LAB06_CPUTIMER_HPP
#define LAB06_CPUTIMER_HPP


#include "Timer.hpp"

#ifdef __linux__

class CpuTimer : public virtual Timer  {
private:
    long cur_time_;
public:
    virtual void start();

    virtual void stop();

    virtual float elapsed();

};

#elif _WIN32 || _WIN64

struct PrivateTimingCPU;

class CpuTimer : public virtual Timer  {
    PrivateTimingCPU *privateTimingCpu;
public:
    CpuTimer();

    virtual void start();

    virtual void stop();

    virtual float elapsed();

};

#endif

#endif //LAB06_CPUTIMER_HPP
