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
  int res;
  printf("Server; current server time is %d\n", server_time);
  for (int i = 1; i < amount; i++) {
    res = MPI_Send(&server_time, 1, MPI_INT, i, 99, MPI_COMM_WORLD);
    check_res(res, 0);
  }

  int sum_dt = 0;
  int client_dts[100];
  for (int i = 1; i < amount; i++) {
    MPI_Status status;
    int client_dt;
    res = MPI_Recv(&client_dt, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    check_res(res, 0);
    sum_dt += client_dt;
    client_dts[status.MPI_SOURCE] = client_dt;
  }

  int dt = (sum_dt + amount - 1) / amount;

  for (int i = 1; i < amount; i++) {
    int res;
    int client_dt = dt - client_dts[i];
    res = MPI_Send(&client_dt, 1, MPI_INT, i, 99, MPI_COMM_WORLD);
    check_res(res, 0);
  }

  server_time = server_time - dt;
  printf("Server; current server time is %d\n", server_time);
}

void client(int client_time, int rank) {
  int server_time;
  int res;
  MPI_Status status;

  res = MPI_Recv(&server_time, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  check_res(res, rank);

  int delta = server_time - client_time;

  res = MPI_Send(&delta, 1, MPI_INT, 0, 99, MPI_COMM_WORLD);
  check_res(res, rank);
  printf("Number of process %d; request was sent; Current time is %d; Delta with server: %d\n", rank, client_time, delta);

  res = MPI_Recv(&delta, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  check_res(res, rank);
  printf("Number of process %d; request was received; Current time is %d; Received server time delta is %d\n", rank, client_time, delta);

  client_time = client_time - delta;

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
