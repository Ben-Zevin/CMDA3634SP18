#include <stdio.h>
#include <stdlib.h>
#include "cuda.h"

// This is my DEVICE function
// __global__ means this function is visible to the host
__global__  void kernelHelloWorld() {

		printf("HEllo World!\n");

}

int main(int argc, char** argv) {


		int Nblocks = 10; //number of blocks
		int Nthreads = 3; //number of threads
		
		// run the function 'kernalHellowWorld' on the DEVICE
		kernelHelloWorld <<< Nblocks, Nthreads >>> ();



}
