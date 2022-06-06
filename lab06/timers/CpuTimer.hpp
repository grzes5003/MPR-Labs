//
// Created by xgg on 3 Jun 2022.
//

#ifndef LAB06_CPUTIMER_HPP
#define LAB06_CPUTIMER_HPP


#include "Timer.hpp"

class CpuTimer : public virtual Timer  {
public:
    virtual void start();

    virtual void stop();

    virtual float elapsed();

};


#endif //LAB06_CPUTIMER_HPP
