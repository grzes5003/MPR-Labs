#include<cstdio>
#include<cstdlib>
#include <boost/format.hpp>
#include "arg_parser.cpp"
#include "BLOCK_SIZE.h"
#include "GpuTimer.h"

__global__ void matrix_transpose_naive(int *input, int *output, int n) {

    int indexX = threadIdx.x + blockIdx.x * blockDim.x;
    int indexY = threadIdx.y + blockIdx.y * blockDim.y;
    int index = indexY * n + indexX;
    int transposedIndex = indexX * n + indexY;

    // this has discoalesced global memory store
    output[transposedIndex] = input[index];

    // this has discoalesced global memore load
    // output[index] = input[transposedIndex];
}

__global__ void matrix_transpose_shared(int *input, int *output, int n) {

    __shared__ int sharedMemory[BLOCK_SIZE][BLOCK_SIZE + 1];

    // global index
    int indexX = threadIdx.x + blockIdx.x * blockDim.x;
    int indexY = threadIdx.y + blockIdx.y * blockDim.y;

    // transposed global memory index
    int tindexX = threadIdx.x + blockIdx.y * blockDim.x;
    int tindexY = threadIdx.y + blockIdx.x * blockDim.y;

    // local index
    int localIndexX = threadIdx.x;
    int localIndexY = threadIdx.y;

    int index = indexY * n + indexX;
    int transposedIndex = tindexY * n + tindexX;

    // reading from global memory in coalesed manner and performing tanspose in shared memory
    sharedMemory[localIndexX][localIndexY] = input[index];

    __syncthreads();

    // writing into global memory in coalesed fashion via transposed data in shared memory
    output[transposedIndex] = sharedMemory[localIndexY][localIndexX];
}

//basically just fills the array with index.
void fill_array(int *data, int n) {
    for (int idx = 0; idx < (n * n); idx++)
        data[idx] = idx;
}

void print_output(int *a, int *b, int n) {
    printf("\n Original Matrix::\n");
    for (int idx = 0; idx < (n * n); idx++) {
        if (idx % n == 0)
            printf("\n");
        printf(" %d ", a[idx]);
    }
    printf("\n Transposed Matrix::\n");
    for (int idx = 0; idx < (n * n); idx++) {
        if (idx % n == 0)
            printf("\n");
        printf(" %d ", b[idx]);
    }
}

int main(int argc, char *argv[]) {
    int *a, *b;
    int *d_a, *d_b; // device copies of a, b, c

    boost::optional<args> _args = parse(argc, argv);
    if (!_args) {
        std::cerr << "Invalid parsing; exiting" << std::endl;
        return 1;
    }

    int n = _args->vector_size;
    int size = n * n * sizeof(int);

    // Alloc space for host copies of a, b, c and setup input values
    a = (int *) malloc(size);
    fill_array(a, n);
    b = (int *) malloc(size);

    // start timer
    GpuTimer gpuTimer = GpuTimer();
    GpuTimer gpuTimer2 = GpuTimer();

    // Alloc space for device copies of a, b, c
    cudaMalloc((void **) &d_a, size);
    cudaMalloc((void **) &d_b, size);

    // Copy inputs to device
    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

    dim3 blockSize(BLOCK_SIZE, BLOCK_SIZE, 1);
    dim3 gridSize(n / BLOCK_SIZE, n / BLOCK_SIZE, 1);


    gpuTimer.start();
    matrix_transpose_naive<<<gridSize, blockSize>>>(d_a, d_b, n);
    gpuTimer.stop();

    // Copy result back to host
    // cudaMemcpy(b, d_b, size, cudaMemcpyDeviceToHost);
    // print_output(a,b);

    gpuTimer2.start();
    matrix_transpose_shared<<<gridSize, blockSize>>>(d_a, d_b, n);
    gpuTimer2.stop();

    // Copy result back to host
    cudaMemcpy(b, d_b, size, cudaMemcpyDeviceToHost);
    // print_output(a,b);

    // stop timer
    gpuTimer.stop();
    std::cout << boost::format("t2:gpu_1:t=%1%:n=%2%:b=%3%\n") % gpuTimer.elapsed() % _args->vector_size % BLOCK_SIZE;
    std::cout << boost::format("t2:gpu_2:t=%1%:n=%2%:b=%3%\n") % gpuTimer2.elapsed() % _args->vector_size % BLOCK_SIZE;

    // terminate memories
    free(a);
    free(b);
    cudaFree(d_a);
    cudaFree(d_b);

    return 0;
}
