#include <mpi.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    int re, amount_tasks, rank_tasks, amount, rank_2;
    re = MPI_Init(&argc, &argv);

    if (re != MPI_SUCCESS) {
    	printf("Initialization Error MPI\n");
    	MPI_Abort(MPI_COMM_WORLD, 1);
    }
    amount = MPI_Comm_size(MPI_COMM_WORLD, &amount_tasks);
    if (amount != MPI_SUCCESS) {
      printf("Size Error MPI\n");
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
    rank_2 = MPI_Comm_rank(MPI_COMM_WORLD, &rank_tasks);
    if (rank_2 != MPI_SUCCESS) {
    	printf("Abortion with rank MPI\n");
    	MPI_Abort(MPI_COMM_WORLD, 1);
    }
    printf("hello from = %d number of tasks = %d\n", rank_tasks, amount_tasks);
    MPI_Finalize();
}
