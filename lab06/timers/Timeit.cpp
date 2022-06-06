//
// Created by xgg on 3 Jun 2022.
//

#include "Timeit.hpp"
#include "GpuTimer.cuh"
#include "CpuTimer.hpp"
#include <iostream>


template<>
float Timeit::timeit<GpuTimer>(add_vec f, AddArgs addArgs) {
    int result;
    GpuTimer timer;

    timer.start();
    if(0 != (result = f(addArgs.h_a, addArgs.h_b, addArgs.h_c, addArgs.n, addArgs.block_size))) {
        std::cerr << "Error while adding vectors, code: " << result << std::endl;
    }
    timer.stop();
    return timer.elapsed();
}

template<>
float Timeit::timeit<CpuTimer>(add_vec f, AddArgs addArgs) {
    int result;
    CpuTimer timer;

    timer.start();
    if(0 != (result = f(addArgs.h_a, addArgs.h_b, addArgs.h_c, addArgs.n, addArgs.block_size))) {
        std::cerr << "Error while adding vectors, code: " << result << std::endl;
    }
    timer.stop();
    return timer.elapsed();
}