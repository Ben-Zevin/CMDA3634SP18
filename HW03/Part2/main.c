#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "mpi.h"
#include "functions.h"

int main (int argc, char **argv) {

  MPI_Init(&argc,&argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  //seed value for the randomizer 
  double seed = clock()+rank; //this will make your program run differently everytime
  //double seed = rank; //uncomment this and your program will behave the same everytime it's run

  srand(seed);

  //begin with rank 0 getting user's input
  unsigned int n;
  unsigned int p, g, h, x;


  /* Q3.1 Make rank 0 setup the ELGamal system and
    broadcast the public key information */

  if (rank == 0){
  printf("Enter a number of bits: "); fflush(stdout);
  char status = scanf("%u",&n);

 // make sure the input makes sense
  if ((n<3)||(n>31)) {//Updated bounds. 2 is no good, 31 is actually ok
    printf("Unsupported bit size.\n");
    return 0;   
  }
  printf("\n");
  
//	n = 25;

  //setup an ElGamal cryptosystem
  setupElGamal(n,&p,&g,&h,&x);
  }

     MPI_Bcast(&p,
			1,
			MPI_UNSIGNED,
			0,
			MPI_COMM_WORLD);
	  MPI_Bcast(&g,
			1,
			MPI_UNSIGNED,
			0,
			MPI_COMM_WORLD);
	  MPI_Bcast(&h,
			1,
			MPI_UNSIGNED,
			0,
			MPI_COMM_WORLD);
	

  //Suppose we don't know the secret key. Use all the ranks to try and find it in parallel
  if (rank==0)
    printf("Using %d processes to find the secret key...\n", size);

  /*Q3.2 We want to loop through values i=0 .. p-2
     determine start and end values so this loop is 
     distributed amounst the MPI ranks  */
  unsigned int N = p-1; //total loop size
  unsigned int start, end;
  unsigned int chunk = N/size;
  unsigned int rem = N%size;
  

  if(rank<=rem && rem !=0)
  {
		start = (rank*chunk)+rank;  
		end = start+chunk+1;
	
  }
  else{
	    start = (rank*chunk)+rem; 
	    end = start+chunk;

  }



	double startTime, endTime, throughPut;
	startTime = MPI_Wtime();
  //loop through the values from 'start' to 'end'
  for (unsigned int i=start;i<end;i++) {
    if (modExp(g,i+1,p)==h)
      printf("Secret key found! x = %u \n", i+1);
  }
  MPI_Barrier(MPI_COMM_WORLD);
 // throughPut = MPI_Wtime();
 // printf("ThroughPut is : %f \n", throughPut-startTime);
  endTime = MPI_Wtime();
  if (rank == 0){
  printf("Final Runtime is : %f \n", endTime-startTime);
  printf("ThroughPut is : %f \n", N/(endTime-startTime));
  
  }

  MPI_Finalize();

  return 0;
}
