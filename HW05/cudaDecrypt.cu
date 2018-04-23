#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "cuda.h"
#include "functions.c"

//compute a*b mod p safely
__device__ unsigned int d_modprod(unsigned int a, unsigned int b, unsigned int p) {
  unsigned int za = a;
  unsigned int ab = 0;

  while (b > 0) {
    if (b%2 == 1) ab = (ab +  za) % p;
    za = (2 * za) % p;
    b /= 2;
  }
  return ab;
}

//compute a^b mod p safely
__device__ unsigned int d_modExp(unsigned int a, unsigned int b, unsigned int p) {
  unsigned int z = a;
  unsigned int aExpb = 1;

  while (b > 0) {
    if (b%2 == 1) aExpb = d_modprod(aExpb, z, p);
    z = d_modprod(z, z, p);
    b /= 2;
  }
  return aExpb;
}

__global__ void kernelFindKeyPoint(unsigned int *d, unsigned int p, unsigned int g, unsigned int h)
{
	int threadId = threadIdx.x;
	int blockId = blockIdx.x;
	int Nblock = blockDim.x;

	int id = threadId +blockId*Nblock;

	if(d_modExp(g, id, p)==h)
	{
			d[0] = id;
	}
}


int main (int argc, char **argv) {

  /* Part 2. Start this program by first copying the contents of the main function from 
     your completed decrypt.c main function. */
  //declare storage for an ElGamal cryptosytem
  unsigned int n, p, g, h, x;
  unsigned int Nints;
  int Nthreads = atoi(argv[1]);

  //get the secret key from the user
  printf("Enter the secret key (0 if unknown): "); fflush(stdout);
  char stat = scanf("%u",&x);

  printf("Reading file.\n");

  /* Q3 Complete this function. Read in the public key data from public_key.txt
    and the cyphertexts from messages.txt. */
  FILE *key, *message;
 // key = fopen("public_key.txt", "r");
  key = fopen("bonus_public_key.txt", "r");
  fscanf(key, "%u" "%u" "%u" "%u", &n, &p, &g, &h);
  fclose(key);
 

//  message = fopen("message.txt", "r");
  message = fopen("bonus_message.txt", "r");
  fscanf(message, "%d", &Nints);
  unsigned int *Zmessage = 
      (unsigned int *) malloc(Nints*sizeof(unsigned int)); 
  
  //storage for extra encryption coefficient 
  unsigned int *a = 
      (unsigned int *) malloc(Nints*sizeof(unsigned int)); 

  for (int i=0; i < Nints; i++)
  {
		  fscanf(message, "%u" "%u", &Zmessage[i], &a[i]);
  }
  fclose(message);



  // find the secret key
 // if (x==0 || modExp(g,x,p)!=h) {
 //   printf("Finding the secret key...\n");
    double startTime = clock();
 //   for (unsigned int i=0; i<p-1; i++) {
 //     if (modExp(g,i+1,p)==h) {
 //       printf("Secret key found! x = %u \n", i+1);
 //       x=i+1;
 //     } 
 //   }
	unsigned int *h_key = (unsigned int*) malloc(1*sizeof(unsigned int));

    unsigned int *d_key;
	cudaMalloc(&d_key,1*sizeof(unsigned int));

	dim3 B(Nthreads, 1, 1);
	dim3 G((p+Nthreads-1)/Nthreads,1,1);

	kernelFindKeyPoint <<<G ,B>>>(d_key, p, g, h);

	cudaDeviceSynchronize();

    double endTime = clock();

	cudaMemcpy(h_key,d_key,1*sizeof(unsigned int),cudaMemcpyDeviceToHost);
	x = h_key[0];

	ElGamalDecrypt(Zmessage,a,Nints,p,x);
	
	unsigned int charsPerInt = (n-1)/8;
	unsigned int Nchars = Nints*charsPerInt;
	
	int bufferSize = 1024;
    unsigned char *newString = (unsigned char *) malloc(bufferSize*sizeof(unsigned char));
	

   
	convertZToString(Zmessage, Nints, newString, Nchars);
	
    printf("Decrypted Message = \"%s\"\n", newString);
    printf("\n");


    double totalTime = (endTime-startTime)/CLOCKS_PER_SEC;
    double work = (double) p;
    double throughput = work/totalTime;

    printf("Searching all keys took %g seconds, throughput was %g values tested per second.\n", totalTime, throughput);

  return 0;
}
