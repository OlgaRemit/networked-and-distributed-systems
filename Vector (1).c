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


void print_times(int* times, int size) {
	printf("times: ");
	int i = 0 ;
	while(i < size) {
		  printf("%d ", times[i]);
			i += 1;
	}
	printf("\n");
}

void main_func(int rank, int total) {
  int res;
  int i = 0;
  int k;
  char file_name[10];
  sprintf(file_name, "/tmp/BM/vector/v%02d.dat", rank+1);
  FILE *file = fopen(file_name, "r");
  if (file == NULL){
    printf("No file in =%d\n", rank);
    return;
  }
  int time[10] = {0};
  while (fscanf(file, "%d", &k) != EOF){
		time[rank] += 1;
    if(k == 0){
      printf("Proc %d inside event, ", rank);
			print_times(time, total);
    }
    if(k < 0){
      printf("Proc %d recive event from = %d, ", rank, -k - 1);
			print_times(time, total);
      MPI_Status status;
      int res_time[10] = {0};
      res = MPI_Recv(&res_time, total, MPI_INT, -k - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      check_res(res, rank);
			int i = 0;
			while(i < total) {
      	time[i] = max(time[i], res_time[i]);
				i += 1;
			}
    }
    if(k > 0){
      printf("Proc %d send event to = %d, ", rank, k - 1);
			print_times(time, total);
      res = MPI_Send(&time, total, MPI_INT, k - 1, 99, MPI_COMM_WORLD);
      check_res(res, rank);
    }
  }
  printf("Proc %d last event, ", rank);
	print_times(time, total);

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

  main_func(rank, amount);


  printf("Number of process %d finished work\n", rank);

  MPI_Finalize();

  return 0;
}
