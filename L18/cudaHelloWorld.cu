#include <stdio.h>
#include <stdlib.h>

#include "cuda.h"

__global__ void kernelHelloWorld() {

	int thread = threadIdx.x;
	int block = blockIdx.x;

	printf("Hello World from thread %d of block %d!\n", thread, block);
}

int main(int argc, char** argv){

	int Nblocks = 10;
	int Nthreads = 3;

	kernelHelloWorld <<< Nblocks, Nthreads >>> ();

	cudaDeviceSynchronize();

	return 0;

}

