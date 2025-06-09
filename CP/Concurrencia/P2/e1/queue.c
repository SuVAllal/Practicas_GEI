#include <stdlib.h>
#include <pthread.h>

// circular array
typedef struct _queue {
    int size;
    int used;
    int first;
    void **data;
    pthread_mutex_t queue_lock;     // Mutex para garantizar exclusion mutua
    pthread_cond_t queue_full;      // Condicion de cola llena
    pthread_cond_t queue_empty;     // Condicion de cola vacia
} _queue;

#include "queue.h"

queue q_create(int size) {
    queue q = malloc(sizeof(_queue));

    q->size  = size;
    q->used  = 0;
    q->first = 0;
    q->data  = malloc(size*sizeof(void *));

    pthread_mutex_init(&q->queue_lock, NULL);   // Inicializar el mutex
    pthread_cond_init(&q->queue_full, NULL);     // Inicializar la condicion de cola llena
    pthread_cond_init(&q->queue_empty, NULL);    // Inicializar la condicion de cola vacia

    return q;
}

int q_elements(queue q) {
    pthread_mutex_lock(&q->queue_lock);
    int x;
    x = q->used;
    pthread_mutex_unlock(&q->queue_lock);
    return x;
}

int q_insert(queue q, void *elem) {
    pthread_mutex_lock(&q->queue_lock);

    if(q->used == q->size){     // Verificar si la cola esta llena
        pthread_mutex_unlock(&q->queue_lock);
        return 0;   // La insercion falla si la cola esta llena
    }
    q->data[(q->first+q->used) % q->size] = elem;
    q->used++;

    pthread_cond_signal(&q->queue_empty);   // Despertar a threads que estan esperando por una cola con espacio

    pthread_mutex_unlock(&q->queue_lock);

    return 1;
}

void *q_remove(queue q) {
    void *res;

    pthread_mutex_lock(&q->queue_lock);

    if(q->used==0){     // Verificar si la cola esta vacia
        pthread_mutex_unlock(&q->queue_lock);
        return NULL;    // Devolver NULL si la cola esta vacia (no se pueden eliminar elementos)
    }
    res = q->data[q->first];
    q->first = (q->first + 1) % q->size;
    q->used--;

    pthread_cond_signal(&q->queue_full); // Despertar a threads que estan esperando por una cola con elementos

    pthread_mutex_unlock(&q->queue_lock);

    return res;
}

void q_destroy(queue q) {
    pthread_mutex_destroy(&q->queue_lock);
    pthread_cond_destroy(&q->queue_full);
    pthread_cond_destroy(&q->queue_empty);

    free(q->data);
    free(q);
}
