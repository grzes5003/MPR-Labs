//
// Created by xgg on 2 Jun 2022.
//
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <utility>
#include "Utility.h"
#include "GpuTimer.h"


// CUDA kernel. Each thread takes care of one element of c
__global__ void vecAdd(const double *a, const double *b, double *c, const int n) {
    // Get our global thread ID
    int id = blockIdx.x * blockDim.x + threadIdx.x;

    // Make sure we do not go out of bounds
    if (id < n)
        c[id] = a[id] + b[id];
}

namespace gpu {
    std::pair<float, int> vec_add(const int n, const int block_size) {
        GpuTimer gpuTimer;

        // Host input vectors
        double *h_a;
        double *h_b;
        // Host output vector
        double *h_c;

        // Device input vectors
        double *d_a;
        double *d_b;
        //Device output vector
        double *d_c;

        // Size, in bytes, of each vector
        size_t bytes = n * sizeof(double);

        // Allocate memory for each vector on host
        h_a = (double *) malloc(bytes);
        h_b = (double *) malloc(bytes);
        h_c = (double *) malloc(bytes);

        // Allocate memory for each vector on GPU
        cudaMalloc(&d_a, bytes);
        cudaMalloc(&d_b, bytes);
        cudaMalloc(&d_c, bytes);

        int i;
        // Initialize vectors on host
        for (i = 0; i < n; i++) {
            h_a[i] = sin(i) * sin(i);
            h_b[i] = cos(i) * cos(i);
        }

        // Start time measurement
        gpuTimer.start();

        // Copy host vectors to device
        cudaMemcpy(d_a, h_a, bytes, cudaMemcpyHostToDevice);
        cudaMemcpy(d_b, h_b, bytes, cudaMemcpyHostToDevice);

        int gridSize;

        // Number of thread blocks in grid
        gridSize = (int) ceil((float) n / block_size);

        // Execute the kernel
        vecAdd<<<gridSize, block_size>>>(d_a, d_b, d_c, n);

        // Copy array back to host
        cudaMemcpy(h_c, d_c, bytes, cudaMemcpyDeviceToHost);

        gpuTimer.stop();

        int result = Utility::check_result(h_c, n);

        // Release device memory
        cudaFree(d_a);
        cudaFree(d_b);
        cudaFree(d_c);

        // Release host memory
        free(h_a);
        free(h_b);
        free(h_c);

        return std::pair<float, int>{gpuTimer.elapsed(), result};
    }
}