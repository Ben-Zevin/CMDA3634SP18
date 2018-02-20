#include "mympi.h"

int main (int argc, char **argv) {

  MPI_Init(&argc,&argv);

  int rank,size;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  int N;

  if (rank==0) N=199;

  //myMPI_Bcast(&N,0);
  // treeMPI_Bcast(&N);
  

  //this is the actual MPI broadcast function
  MPI_Bcast(&N,   //pointer to data
			1, 		//count
			MPI_INT, // data type
			0, 		//root process (the process that broadcats
			MPI_COMM_WORLD);

  printf("Rank %d recieved the value N = %d\n",rank,N);

  if (rank==size-1) N=10;
  MPI_Bcast(&N,   //pointer to data
			1, 		//count
			MPI_INT, // data type
			size -1, //root process (the process that broadcats
			MPI_COMM_WORLD);

 
 // myMPI_Bcast(&N,size-1);
  printf("Rank %d recieved the value N = %d\n",rank,N);

  //myMPI_Barrier();
  

	//MPI's barrier
	MPI_Barrier(MPI_COMM_WORLD);
  //setup a test for the reduction
  float val = 1.0;
  
  // float sum = MPI_Reduction(val);
  float sum;

  //MPI's reduce function
  MPI_Reduce(&val,		//send buffer
			&sum, 		//recvive buffer
			1,			//count
			MPI_FLOAT,	//data type
			MPI_SUM,	//operation - others are MPI_MIN, MPI_MAX, MPI_PROD
			0,			//root process (the one that has the final answer
			MPI_COMM_WORLD);

  for (int r=0;r<size;r++) {
    if (r==rank) {
      printf("Rank %d has value %f after the reduction.\n", rank, sum);
    }
    myMPI_Barrier(MPI_COMM_WORLD); 
  }

	// this preforms the reduction so all ranks have the final value
  MPI_Allreduce(&val, 
		  		&sum,
				1,
				MPI_FLOAT,
				MPI_SUM,
				MPI_COMM_WORLD);


  for (int r=0;r<size;r++) {
    if (r==rank) {
      printf("Rank %d has value %f after the reduction.\n", rank, sum);
    }
    myMPI_Barrier(MPI_COMM_WORLD); 
  }


	val = (float) rank;
	float *gatheredVal;
	
	if (rank == 0) gatheredVal = (float *) malloc(size*sizeof(float));

	 MPI_Gather(&val, 		//send buffer
				1,			//send count
				MPI_FLOAT,	//send type
				gatheredVal,//recv buffer
				1,			//recv count
				MPI_FLOAT,	//recv type
				0,			//root process
				MPI_COMM_WORLD);

	 if (rank == 0) {
			 for (int r=0; r<size; r++){
					 printf("gatheredval[%d] = %f \n" ,r, gatheredVal[r]);
					 gatheredVal[r] *= 2; // change the data
			 }
	 }

	 //scatter the data back (this is the reverse of the gather
	MPI_Scatter(gatheredVal,	//send buffer
				1,				//send count
				MPI_FLOAT,		//send type
				&val,			//recv buffer
				1,				//recv count
				MPI_FLOAT,		//recv type
				0,				//root process
				MPI_COMM_WORLD);

	printf("Rank %d has the value %f after the scatter. \n", rank, val);

  MPI_Finalize();
  return 0;
}
