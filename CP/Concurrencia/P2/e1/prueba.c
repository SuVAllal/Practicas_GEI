/*
 * CODIGO DE PRUEBA que simula la interaccion entre un productor
 * y un consumidor utilizando threads y una cola
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "queue.h"

#define QUEUE_SIZE 5

// Argumentos de los threads que probaran la cola
typedef struct {
    queue q;
    int i;
} thread_args;

// Funcion del productor: inserta elementos en la cola
void *producer(void *arg) {
    thread_args *args = (thread_args *) arg;
    for (int i = 0; i < 10; i++) {
        int *elem = malloc(sizeof(int));
        *elem = i;
        q_insert(args->q, elem);
        printf("Productor %d, elemento insertado: %d\n", args->i, *elem);
        usleep(500000); // Espera 0.5 segundos
    }
    pthread_exit(NULL);
}

// Funcion del consumidor: elimina elementos de la cola
void *consumer(void *arg) {
    thread_args *args = (thread_args *) arg;
    for (int i = 0; i < 10; i++) {
        int *elem = (int *)q_remove(args->q);
        if(elem!=NULL){
            printf("Consumidor %d, elemento eliminado: %d\n", args->i, *elem);
            free(elem);
        } else {
            printf("Cola vacia\n");
        }

        usleep(1000000); // Espera 1 segundo (hace algo con el elemento)
    }
    pthread_exit(NULL);
}

int main() {
    // Crear una cola con capacidad definida por QUEUE_SIZE
    queue myQueue = q_create(QUEUE_SIZE);

    if (myQueue == NULL) {
        fprintf(stderr, "Error creando la cola\n");
        return 1;
    }

    // Crear threads para el productor y el consumidor
    pthread_t threads_productor, threads_consumidor;
    thread_args productor = {myQueue, 1};
    thread_args consumidor = {myQueue, 2};

    if (pthread_create(&threads_productor, NULL, producer, &productor) != 0) {
        fprintf(stderr, "Error creando el thread productor numero 0\n");
        return 1;
    }

    if (pthread_create(&threads_consumidor, NULL, consumer, &consumidor) != 0) {
        fprintf(stderr, "Error creando el thread consumidor numero 2\n");
        return 1;
    }

    // Esperar a que ambos threads terminen su ejecucion
    pthread_join(threads_productor, NULL);
    pthread_join(threads_consumidor, NULL);

    // Destruir la cola despues de que ambos threads hayan terminado
    q_destroy(myQueue);

    return 0;
}