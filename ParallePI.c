#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "math.h"

#define COMM MPI_COMM_WORLD
#define N 10000
#define PI acos(-1)
#define MASTER 0
#define TAG 777

double f(double x) { return 4 / (1 + x * x);} 

int main(void){

MPI_Init(NULL, NULL); // Starts parallel environment

int size, rank, i;
MPI_Comm_rank(COMM, &rank);
MPI_Comm_size(COMM, &size);

int chunk = N / size;
double x;
double localsum = 0.0;

for(i = 0; i < chunk; i++){
    x = (rank * chunk + i) / (double)N;
    localsum += f(x);
}
localsum /= N;

printf("Rank %d:  %f\n",rank, localsum);
MPI_Status stat;

if(rank != MASTER){
    MPI_Send(&localsum, 1, MPI_DOUBLE, MASTER, TAG+rank, COMM);
}
else{
    double gelen;
    for(i = 1; i <= 3; i++){
	MPI_Recv(&gelen, 1, MPI_DOUBLE, i, TAG+i, COMM, &stat);
	localsum += gelen;
    }
}  

MPI_Barrier(COMM);

if(rank == MASTER)
    printf("Approximate PI: %f\n", localsum);



 
MPI_Finalize();

}
