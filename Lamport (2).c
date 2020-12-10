#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void check_res(int status_result, int rank) {
	if(status_result != MPI_SUCCESS) {
		printf("Error is in %d \n", rank);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
}


int max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

void main_func(int rank) {
  int res;
  int i = 0;
  int k;
  char file_name[10];
  sprintf(file_name, "/tmp/BM/lamport/%02d.dat", rank+1);
  FILE *file = fopen(file_name, "r");
  if (file == NULL){
    printf("No file in =%d\n", rank);
    return;
  }
  int time = 0;
  while (fscanf(file, "%d", &k) != EOF){

    if (k == 0){
      time += 1;
      printf("Proc %d inside event, time = %d\n", rank, time);
    }
    if (k < 0){
      printf("Proc %d recive event from = %d, time = %d\n", rank, -k - 1,time);
      MPI_Status status;
      int res_time;
      res = MPI_Recv(&res_time, 1, MPI_INT, -k - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      check_res(res, rank);
      time = max(time, res_time) + 1;
    }
    if (k > 0){
			time += 1;
      printf("Proc %d send event to = %d, time = %d\n", rank, k - 1, time);
      res = MPI_Send(&time, 1, MPI_INT, k - 1, 99, MPI_COMM_WORLD);
      check_res(res, rank);
    }
  }
  printf("Proc %d last event, time = %d\n", rank, time);

}

int main (int argc, char *argv[]) {
  int res;
  int rank;
  int amount;
	res = MPI_Init(&argc, &argv);
  check_res(res, 0);

  res = MPI_Comm_size(MPI_COMM_WORLD, &amount);
  check_res(res, 0);
	res = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  check_res(res, 0);

  main_func(rank);


  printf("Number of process %d finished work\n", rank);

  MPI_Finalize();

  return 0;
}
