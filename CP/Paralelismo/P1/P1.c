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

            // Enviar el numero de intervalos a los demas procesos
            for(i = 1; i < numprocs; i++)
                MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        } else // Procesos que NO son el 0:
            MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Recibir el numero de intervalos

        if(n == done) { // Si n es 0 salimos del bucle y termina el programa
            break;
        }

        // Calculo de la aproximacion de pi:
        h   = 1.0 / (double) n;
        sum = 0.0;

        // Cada proceso calcula una parte de la aproximacion:
        for (i = 1; i <= n; i+= numprocs) { //rank + 1, el 0 -> 1, el 1 -> 2
            x = h * ((double)i - 0.5);
            sum += 4.0 / (1.0 + x*x);
        }
        my_pi = h * sum; // Aproximacion de pi para el proceso actual

        if(rank == 0) { // El proceso 0 recibe las aproximaciones parciales de los demas
            pi = my_pi;
            for(int src = 1; src < numprocs; src++) {
                MPI_Recv(&my_pi, 1, MPI_DOUBLE, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                pi += my_pi; // Suma todas las aproximaciones
            }
            // Y muestra el resultado:
            printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
        } else // Procesos que NO son el 0:
            MPI_Send(&my_pi, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD); // Envian sus aproximaciones de pi al proceso 0

    }

    MPI_Finalize(); // Dar por finalizada la aplicacion
}
