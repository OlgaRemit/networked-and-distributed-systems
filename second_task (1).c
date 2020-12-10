#include <mpi.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    int re1, re2, re3, amount_tasks, rank, rc, is_success, is_receve;
    MPI_Status status;
    re1 = MPI_Init(&argc, &argv);
    if (re1 != MPI_SUCCESS) {
        printf("Error in init\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    re2 = MPI_Comm_size(MPI_COMM_WORLD, &amount_tasks);
    if (re2 != MPI_SUCCESS) {
        printf("Error in get amount of tasks\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    re3 = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (re3 != MPI_SUCCESS) {
            printf("Error in get rank of tasks\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
    }
    if (rank == 0) {
        is_receve = 0;
        is_success = MPI_Send(&is_receve, 1, MPI_INT, rank+1, 99, MPI_COMM_WORLD);
        if (is_success != MPI_SUCCESS) {
            printf("Error in send 0\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("Send %d to = %d, from = %d\n", 0, 1, 0);
    } else if (rank == (amount_tasks-1)){
        is_success = MPI_Recv(&is_receve, 1, MPI_INT, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
        if (is_success != MPI_SUCCESS) {
            printf("Error in recive end\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("Get %d from = %d, to = %d\n", is_receve, status.MPI_SOURCE, rank);
        is_receve = is_receve + rank;
        is_success = MPI_Send(&is_receve, 1, MPI_INT, 0, 99, MPI_COMM_WORLD);
        if (is_success != MPI_SUCCESS) {
            printf("Error in send end\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("Send %d to = %d, from = %d \n", is_receve+rank, 0, rank);
    } else {
        is_success = MPI_Recv(&is_receve, 1, MPI_INT, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
        if (is_success != MPI_SUCCESS) {
            printf("Error in receve \n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("Get %d from = %d, to = %d \n", is_receve, status.MPI_SOURCE ,rank );
        is_receve = is_receve + rank;
        is_success = MPI_Send(&is_receve, 1, MPI_INT, rank+1, 99, MPI_COMM_WORLD);
        if (is_success != MPI_SUCCESS) {
            printf("Error in send \n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("Send %d to = %d, from = %d \n", is_receve+rank, rank+1, rank);
    }
    MPI_Finalize();
}
