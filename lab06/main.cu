#include "cuda_vec_add.cu"
#include <iostream>
#include "args_parser.cpp"
#include "Utility.h"
#include "Timer.h"
#include "GpuTimer.h"
#include <boost/format.hpp>

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
    double *h_c;

    // Size, in bytes, of each vector
    size_t bytes = _args->vector_size * sizeof(double);

    // Allocate memory for each vector on host
    h_a = (double *) malloc(bytes);
    h_b = (double *) malloc(bytes);
    h_c = (double *) malloc(bytes);

    Utility::init_vectors(h_a, h_b, bytes);

    AddArgs addArgs = AddArgs{
            h_a, h_b, h_b, _args->vector_size, _args->blocks
    };

    GpuTimer gpuTimer;

    auto gpu_time = Timeit::timeit(gpu::vec_add, addArgs, gpuTimer);
    auto gpu_result = Utility::check_result(h_c, _args->vector_size);

    // Release host memory
    free(h_a);
    free(h_b);
    free(h_c);

    std::cout << boost::format("%2% %1%!\n") % gpu_result % gpu_time;
    return 0;
}