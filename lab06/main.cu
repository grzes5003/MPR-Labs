#include "cuda_vec_add.cu"
#include "cpu_vec_add.cpp"
#include <iostream>
#include "Utility.hpp"
#include "timers/Timeit.hpp"
#include "timers/GpuTimer.cuh"
#include <boost/format.hpp>
#include "args_parser.cpp"
#include "timers/CpuTimer.hpp"

int main(int argc, char *argv[]) {
    boost::optional<args> _args = parse(argc, argv);
    if (!_args) {
        std::cerr << "Invalid parsing; exiting" << std::endl;
        return 1;
    }

    // Host input vectors
    double *h_a;
    double *h_b;
    // Host output vector
    double *h_cpu;
    double *h_gpu;

    // Size, in bytes, of each vector
    size_t bytes = _args->vector_size * sizeof(double);

    // Allocate memory for each vector on host
    h_a = (double *) malloc(bytes);
    h_b = (double *) malloc(bytes);
    h_cpu = (double *) malloc(bytes);
    h_gpu = (double *) malloc(bytes);

    Utility util;
    Timeit timer;
    util.init_vectors(h_a, h_b, _args->vector_size);

    AddArgs addArgs = AddArgs{
            h_a, h_b, h_b, _args->vector_size, _args->blocks
    };

    auto gpu_time = timer.timeit<GpuTimer>(gpu::vec_add, addArgs);
    auto gpu_result = util.check_result(h_gpu, _args->vector_size);

    auto cpu_time = timer.timeit<CpuTimer>(cpu::vec_add, addArgs);
    auto cpu_result = util.check_result(h_gpu, _args->vector_size);

    // Release host memory
    free(h_a);
    free(h_b);
    free(h_cpu);
    free(h_gpu);

    std::cout << boost::format("gpu:t=%1%:ok=%2%:b=%3%:s=%4%\n") % gpu_time % gpu_result % _args->blocks % _args->vector_size;
    std::cout << boost::format("cpu:t=%1%:ok=%2%:b=%3%:s=%4%\n") % cpu_time % cpu_result % _args->blocks % _args->vector_size;
    return 0;
}