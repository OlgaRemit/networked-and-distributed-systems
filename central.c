#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define REQUEST_MSG 10
#define RELEASE_MSG 11

void check_res(int status_result, int rank) {
	if(status_result != MPI_SUCCESS) {
		printf("Error is in %d \n", rank);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
}

void server(int amount, int repeat) {
  printf("Server: start\n");
	int res = 0;
	int msg = 0;
	int source = 0;
	int i = 0;
	MPI_Status status;
  while(i++ < (amount - 1) * repeat) {
    res = MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    check_res(res, 0);
		assert(REQUEST_MSG == msg);

    source = status.MPI_SOURCE;
		printf("Server: got request msg from %d\n", source);
		res = MPI_Send(&msg, 1, MPI_INT, source, 99, MPI_COMM_WORLD);
		printf("Server: send critical section premission for %d\n", source);

		res = MPI_Recv(&msg, 1, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    check_res(res, 0);
		assert(RELEASE_MSG == msg);
		printf("Server: got release msg from %d\n", source);
  }
  printf("Server: end %d\n");
}

void work() {
	int time = rand() % 5 + 1;
	sleep(time);
}

void client(int rank, int repeat) {
	printf("Worker %d: start\n", rank);
	int res = 0;
	int msg = 0;
	int i = 0;
	MPI_Status status;
	while(i++ < repeat) {
		printf("Worker %d: repeat %d time\n", rank, i);
		msg = REQUEST_MSG;
		printf("Worker %d: send request for critical section\n", rank);
		res = MPI_Send(&msg, 1, MPI_INT, 0, 99, MPI_COMM_WORLD);
		check_res(res, rank);
		printf("Worker %d: wait permission for critical section\n", rank);
		res = MPI_Recv(&msg, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		check_res(res, rank);
		printf("Worker %d: start critical section\n", rank);
		work();
		printf("Worker %d: end critical section\n", rank);
		msg = RELEASE_MSG;
		res = MPI_Send(&msg, 1, MPI_INT, 0, 99, MPI_COMM_WORLD);
		check_res(res, rank);
		printf("Worker %d: in remainder section\n", rank);
	}

	printf("Worker %d: end\n", rank);
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

	const int repeat = 3;

  if (rank == 0) {
    server(amount, repeat);
  }
  else {
    client (rank, repeat);
  }

  printf("Process %d finished work\n", rank);

  MPI_Finalize();

  return 0;
}
