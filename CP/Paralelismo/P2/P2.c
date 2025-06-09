#include <stdio.h>
#include <math.h>
#include <mpi.h>

int MPI_FlattreeColectiva(void *buff, void *recvbuff, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm) {
    int i, j, total, error, dato;
    int numprocs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Realizamos el control de errores
    if(datatype != MPI_DOUBLE)
        return MPI_ERR_TYPE;

    if(root < 0 || root >= numprocs)
        return MPI_ERR_ROOT;

    if(count <= 0)
        return MPI_ERR_COUNT;

    if(buff == NULL || recvbuff == NULL)
        return MPI_ERR_BUFFER;

    // Variable para almacenar el resultado de la operacion
    double local_sum = *(double *)buff;
    double global_sum = 0.0;

    // Cada proceso envia su valor al proceso root
    error = MPI_Send(&local_sum, 1, datatype, root, 0, comm);
    if(error != MPI_SUCCESS)
        return error;

    // El proceso root recibe los valores de todos los procesos y los suma
    if(rank == root) {
        global_sum = local_sum; // Inicializar global_sum con el valor del proceso root
        for(i = 0; i < numprocs; i++) {
            if(i != root) {
                double recv_val;
                error = MPI_Recv(&recv_val, 1, datatype, i, 0, comm, MPI_STATUS_IGNORE);
                if(error != MPI_SUCCESS)
                    return error;
                // Aplicar la suma
                global_sum += recv_val;
            }
        }
    }

    // Almacenar el resultado final en el proceso root:
    if(rank == root)
        *(double *)recvbuff = global_sum;

    return MPI_SUCCESS;
    
}

int MPI_BinomialColectiva(void * buf, int count, MPI_Datatype datatype, int root, MPI_Comm comm){
    int numprocs, rank, error;

    // Se inicia el MPI.
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Espero a que todos los procesos reciban el valor de n
    MPI_Barrier(comm);

    // Aqui vamos a usar una estructura de árbol binario tal como se describe en el enunciado.
    for(int i=1; pow(2,i-1) <= numprocs; i++){
        // Si el proceso está dentro del grupo de los que puede enviar y existe destinatario se envía el mensaje con MPI_Send.
        if(rank < pow(2,i-1) && rank+pow(2,i-1) < numprocs){
            printf("Soy el proceso %d y envio el dato al proceso %d\n", rank, rank+(int)pow(2,i-1));
            error=MPI_Send(buf, count, datatype, rank+(int)pow(2,i-1), 0, comm);
            if(error!=MPI_SUCCESS){
                return error;
            }
        }

        // Y los destinatarios reciben el mensaje con MPI_Recv.
        if(rank >= pow(2,i-1) && rank < pow(2,i)){
            printf("Soy el proceso %d y recibo el dato del proceso %d\n", rank, rank-(int)pow(2,i-1));
            error=MPI_Recv(buf, count, datatype, rank-(int)pow(2,i-1), 0, comm, MPI_STATUS_IGNORE);
            if(error!=MPI_SUCCESS){
                return error;
            }
        }

        // Esperar a que todos los procesos terminen la impresion antes de continuar
        MPI_Barrier(comm);
    }
    // Si no hay ningún problema se manda un MPI_SUCCESS.
    return MPI_SUCCESS;
}

int main(int argc, char *argv[])
{
    int numprocs, rank; // Numero de procesos y rango (identificador) de cada uno
    MPI_Status status; // Variable para almacenar el estado de la comunicacion MPI

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

        MPI_BinomialColectiva(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

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
        MPI_FlattreeColectiva(&my_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if(rank == 0) { 
            // Y muestra el resultado:
            printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
        }
    }

    MPI_Finalize(); // Dar por finalizada la aplicacion
}
