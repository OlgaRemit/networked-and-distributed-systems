#include <mpi.h>
#include <stdio.h>
#include <math.h>

double f(double x)
{
    return sqrt(4 - x * x);
}
double countPiValue(int N, int minIndex, int maxIndex)
{
    int minIndex;
    int maxIndex;
    double pi = 0;
    double delta = 2.0 / N;
    double x1 = minIndex * delta;
    double x2 = x1 + delta;
    int i = minIndex;
    while (i <= maxIndex && i < N)
    {
        pi += delta * (f(x1) + f(x2)) / 2;

        i++;
        x1 = x2;
        x2 += delta;
        if (i == N - 1) {
            x2 = 2;
        }
    }
    return pi;
}

void print_rank(int rank) {
  printf("Rank %d:\t", rank);
}

void print_error_start_and_abort(int m) {
  printf("Error starting MPI program. Terminating.\n");
  MPI_Abort(MPI_COMM_WORLD, m);
}

int main(int argc, char *argv[])
{
    int numtasks, rank, m;
    m = MPI_Init(&argc, &argv);
    if (m != MPI_SUCCESS) {print_error_start_and_abort(m); return 1;};
    m = MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    if (m != MPI_SUCCESS) {print_error_start_and_abort(m); return 1;};
    m = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (m != MPI_SUCCESS) {print_error_start_and_abort(m); return 1;};

    int n = 100;
    if (rank == 0)
    {
        FILE *input = fopen("/home/bm202006/BM/input.txt", "r");
        if (input == NULL)
        {
            print_rank(rank);
            printf ("Cannot open input file. Default N == %d\n", n);
        }
        else
        {
            fscanf(input, "%d", &n);
            print_rank(rank);
            printf("Get value N == %d from file\n", n);
        }
        fclose(input);
        int i = 1;
        while (i < numtasks)
        {
            print_rank(rank);
            printf("Send %d to rank %d\n", n, i);
            m = MPI_Send(&n, 1, MPI_INT, i, 534, MPI_COMM_WORLD);
            if (m != MPI_SUCCESS) {print_error_start_and_abort(m); return 1;};
            i++;
        }
    }
    else
    {
        MPI_Status status;
        int msgRecvRank = 0;
        m = MPI_Recv(&n, 1, MPI_INT, msgRecvRank, 534, MPI_COMM_WORLD, &status);
        if (m != MPI_SUCCESS) {print_error_start_and_abort(m); return 228;};
        printf("Receive %d value from rank %d\n", n, msgRecvRank);
    }

    int delta = n / numtasks;
    int minIndex = rank * delta;
    int maxIndex = rank == (numtasks - 1) ? (numtasks * n) - 1 : ((rank + 1) * delta) - 1;
    double result = countPiValue(n, minIndex, maxIndex);
    print_rank(rank);
    printf("Min: %d, Max: %d,\t\tResult: %lf\n", minIndex, maxIndex, result);

    if (rank == 0)
    {
        double pi = result;
        int i = 1;
        while (i < numtasks)
        {
            MPI_Status status;
            m = MPI_Recv(&result, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 534, MPI_COMM_WORLD, &status);
            if (m != MPI_SUCCESS) {print_error_start_and_abort(m); return 1;};
            print_rank(rank);
            printf("Result receive %d value from rank %d\n", n, status.MPI_SOURCE);
            pi += result;

            i++;
        }

        printf("\n");
        print_rank(rank);
        printf("Common result:\t%lf\n\n", pi);
    }
    else
    {
        print_rank(rank);
        printf("Result send %d to rank %d\n", n, 0);
        m = MPI_Send(&result, 1, MPI_DOUBLE, 0, 534, MPI_COMM_WORLD);
        if (m != MPI_SUCCESS) {print_error_start_and_abort(m); return 1;};
    }

    MPI_Finalize();
    return 0;
}
