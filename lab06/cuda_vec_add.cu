//
// Created by xgg on 2 Jun 2022.
//
#include <cstdio>
#include <cmath>


// CUDA kernel. Each thread takes care of one element of c
__global__ void vecAdd(const double *a, const double *b, double *c, const int n) {
    // Get our global thread ID
    int id = blockIdx.x * blockDim.x + threadIdx.x;

    // Make sure we do not go out of bounds
    if (id < n)
        c[id] = a[id] + b[id];
}

namespace gpu {
    int vec_add(const double *h_a, const double *h_b, double *h_c, const int n, const int threads) {

        // Size, in bytes, of each vector
        size_t bytes = n * sizeof(double);

        // Device input vectors
        double *d_a;
        double *d_b;
        //Device output vector
        double *d_c;

        // Allocate memory for each vector on GPU
        cudaMalloc(&d_a, bytes);
        cudaMalloc(&d_b, bytes);
        cudaMalloc(&d_c, bytes);

        // Copy host vectors to device
        cudaMemcpy(d_a, h_a, bytes, cudaMemcpyHostToDevice);
        cudaMemcpy(d_b, h_b, bytes, cudaMemcpyHostToDevice);

        int gridSize;

        // Number of thread blocks in grid
        gridSize = (int) ceil((float) n / threads);

        // Execute the kernel
        vecAdd<<<gridSize, threads>>>(d_a, d_b, d_c, n);

        // Copy array back to host
        cudaMemcpy(h_c, d_c, bytes, cudaMemcpyDeviceToHost);

        // Release device memory
        cudaFree(d_a);
        cudaFree(d_b);
        cudaFree(d_c);

        return 0;
    }
}