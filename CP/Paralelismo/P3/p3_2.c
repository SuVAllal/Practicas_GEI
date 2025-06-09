#include <stdio.h>
#include <sys/time.h>
#include <mpi.h>
#include <stdlib.h>

#define DEBUG 1

#define N 10


int main(int argc, char *argv[] ) {

// Variables e inicialización del entorno MPI
  int numprocs, rank;

  MPI_Init(&argc, &argv); // Inicializacion de MPI
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs); // Obtener el numero total de procesos
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtener el rango (identificador) del proceso actual

  // Calcular numero de filas por proceso
  int m = (N / numprocs) + (rank < N%numprocs? 1:0); // Si el proceso es uno de los primeros N % numprocs procesos, se le asigna una fila adicional

  // Declaracion de arrays para contar y desplazar para Scatterv y Gatherv
  int *countScatter = NULL;
  int *displsScatter = NULL;
  int *countGather = NULL;
  int *displsGather = NULL;

  // Declaracion de variables y punteros a arrays
  int i, j;
  float *matrix = NULL; // Solo el proceso 0 necesita leer la matriz completa
  float *local_matrix = malloc(sizeof(float) * m * N); // Cada proceso almacena m filas
  float *vector = malloc(sizeof(float) * N); // Lo necesitan todos los procesos
  float *local_result = malloc(sizeof(float) * m); // Resultado parcial de cada proceso
  float *result = NULL; // Solo el proceso 0 necesita almacenar el resultado completo
  
  // Declaracion de variables para medir el tiempo
  struct timeval  tv1, tv2, bcast1, bcast2, scatter1, scatter2, gather1, gather2;

  // Inicializacion de la matriz y el vector (solo el proceso 0)
  if(rank == 0) {
    // Reserva memoria para la matriz y el resultado completo
    matrix = malloc(sizeof(float) * N * N);
    result = malloc(sizeof(float) * N);

    // Reserva de memoria para los arrays de counts y displs
    countScatter = malloc(numprocs * sizeof(int));
    displsScatter = malloc(numprocs * sizeof(int));
    countGather = malloc(numprocs * sizeof(int));
    displsGather = malloc(numprocs * sizeof(int));

    // Inicializacion del vector y la matriz
    for(i=0;i<N;i++) {
      vector[i] = i;
      for(j=0;j<N;j++) {
        matrix[i * N + j] = i+j; // i * N -> fila; (+j) -> desplazamiento
      }
    }

    for(i = 0; i < numprocs; i++) {
      // Calcular counts para cada proceso
      countScatter[i] = (i < (N%numprocs)) ? m * N : (N/numprocs) * N;  // Si el proceso tiene filas adicionales, añadelas
      countGather[i] = (i < (N%numprocs)) ? m : N/numprocs;

      // Calcular desplazamientos para cada proceso 
      displsScatter[i] = (i == 0) ? 0 : displsScatter[i-1] + countScatter[i-1];
      displsGather[i] = (i == 0) ? 0 : displsGather[i-1] + countGather[i-1];
    }

  }
  
  // Registrar el tiempo antes de la operacion de broadcast
  gettimeofday(&bcast1, NULL);
  // Distribuir el vector a todos los procesos
  MPI_Bcast(vector, N, MPI_FLOAT, 0, MPI_COMM_WORLD);
  // Registrar el tiempo despues de la operacion de broadcast
  gettimeofday(&bcast2, NULL);

  // Registrar el tiempo antes de la operacion de scatter
  gettimeofday(&scatter1, NULL);
  // Distribuir filas de la matriz entre los procesos
  MPI_Scatterv(matrix, countScatter, displsScatter, MPI_FLOAT, local_matrix, m * N, MPI_FLOAT, 0, MPI_COMM_WORLD);
  // Registrar el tiempo despues de la operacion scatter
  gettimeofday(&scatter2, NULL);

  // Registrar el tiempo antes de la operacion de computacion
  gettimeofday(&tv1, NULL);

  // Realizar la multiplicacion de la matriz local por el vector
  for(i = 0; i < m; i++) {
    local_result[i]=0;
    for(j = 0; j < N; j++) {
      local_result[i] += local_matrix[i * N + j] * vector[j];
    }
  }


  // Registrar el tiempo despues de la operacion de computacion
  gettimeofday(&tv2, NULL);

  // Registrar el tiempo antes de la operacion de gather
  gettimeofday(&gather1, NULL);
  // Recopilar resultados parciales en el proceso 0
  MPI_Gatherv(local_result, m, MPI_FLOAT, result, countGather, displsGather, MPI_FLOAT, 0, MPI_COMM_WORLD);
  // Registrar el tiempo despues de la operacion de gather
  gettimeofday(&gather2, NULL);
    
  // Calcular el tiempo de computacion
  int microseconds = (tv2.tv_usec - tv1.tv_usec)+ 1000000 * (tv2.tv_sec - tv1.tv_sec);    
  
  // Calcular el tiempo de broadcast
  int microsecondsBcast = (bcast2.tv_usec - bcast1.tv_usec)+ 1000000 * (bcast2.tv_sec - bcast1.tv_sec);
  // Calcular el tiempo de scatter
  int microsecondsScatter = (scatter2.tv_usec - scatter1.tv_usec)+ 1000000 * (scatter2.tv_sec - scatter1.tv_sec);
  // Calcular el tiempo de gather
  int microsecondsGather = (gather2.tv_usec - gather1.tv_usec)+ 1000000 * (gather2.tv_sec - gather1.tv_sec);

  // Calcular el tiempo total de comunicacion
  int tComm = microsecondsBcast + microsecondsScatter + microsecondsGather;



  // Mostrar resultados
  if (DEBUG){
    if(rank == 0) {
      for(i = 0; i < N; i++) {
      printf(" %f \t ",result[i]);
      }
    }
  } else {
    printf ("Tiempo Computacion (segundos) = %lf\n", (double) microseconds/1E6);
    printf("Tiempo Comunicacion (segundos) = %lf\n", (double) tComm/1E6);
  }

// Liberar la memoria asignada en el proceso 0
if(rank == 0) {
      free(matrix);
      free(result);
      free(countScatter);
      free(countGather);
      free(displsScatter);
      free(displsGather);
}

// Liberar la memoria asignada
free(vector);
free(local_result);
free(local_matrix);    

MPI_Finalize();

  return 0;
}
