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

void server(int server_time, int amount) {
  for (int i = 1; i < amount; i++) {
    int client_time;
    int res;
    MPI_Status status;
    res = MPI_Recv(&client_time, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    check_res(res, 0);
    res = MPI_Send(&server_time, 1, MPI_INT, status.MPI_SOURCE, 99, MPI_COMM_WORLD);
    check_res(res, 0);
    printf("Server; current server time is %d\n", server_time);
    server_time = server_time + rand()%100;
  }
}

void client(int client_time_init, int rank) {
  int server_time;
  int res;
  MPI_Status status;

  res = MPI_Send(&client_time_init, 1, MPI_INT, 0, 99, MPI_COMM_WORLD);
  check_res(res, rank);
  printf("Number of process %d; request was sent; Current time is %d\n", rank, client_time_init);

  int client_time = client_time_init + rand()%100;
  res = MPI_Recv(&server_time, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  check_res(res, rank);
  printf("Number of process %d; request was received; Current time is %d; Received server time is %d\n", rank, client_time, server_time);

  int delta_time = server_time - (client_time_init + client_time + 1) / 2;

  client_time = client_time + delta_time;

  printf("Number of process %d; fixed current client time is %d\n", rank, client_time);
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

  srand(rank + 1);
	int time = rand()%100;

  if (rank == 0) {
    server(time, amount);
  }
  else {
    client (time, rank);
  }

  printf("Number of process %d finished work\n", rank);

  MPI_Finalize();

  return 0;
}
