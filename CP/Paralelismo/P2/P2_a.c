#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int numprocs, rank; // Numero de procesos y rango (identificador) de cada uno

    int i, done = 0, n;
    double PI25DT = 3.141592653589793238462643;
    double pi, h, sum, x;
    double my_pi; // Estimacion de pi de cada uno de los procesos

    MPI_Init(&argc, &argv); // Iniciar la aplicacion paralela
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs); // Averiguar el numero de procesos que participan
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Averiguar rango (identificador) del proceso actual

    while(!done) { // Mientras no se mande un 0
        if(rank == 0) { // La E/S la hace el proceso 0
            // Solicitar al usuario el numero de intervalos
            printf("Enter the number of intervals: (0 quits) \n");
            scanf("%d",&n);
        }

        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if(n == done) { // Si n es 0 salimos del bucle y termina el programa
            break;
        }

        // Calculo de la aproximacion de pi:
        h   = 1.0 / (double) n;
        sum = 0.0;

        // Cada proceso calcula una parte de la aproximacion:
        for (i = rank + 1; i <= n; i+= numprocs) {
            x = h * ((double)i - 0.5);
            sum += 4.0 / (1.0 + x*x);
        }
        my_pi = h * sum; // Aproximacion de pi para el proceso actual

        // El proceso 0 recibe las aproximaciones parciales de los demas
        MPI_Reduce(&my_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if(rank == 0) { 
            // Y muestra el resultado:
            printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
        }
    }

    MPI_Finalize(); // Dar por finalizada la aplicacion
}
