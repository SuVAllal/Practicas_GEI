#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "options.h"

struct nums {
    long *increase; // Array del contador increase
    long *decrease; // Array del contador decrease
    long total;
    long diff;
    pthread_mutex_t *mutexInc; // Mutex del array increase
    pthread_mutex_t *mutexDec; // Mutex del array decrease
};

struct args {
    int thread_num;		// application defined thread #
    int *iterations;	// number of operations
    struct nums *nums;	// pointer to the counters (shared with other threads)
    pthread_mutex_t *mutexIt; // Mutex para proteger las iteraciones
    int size;
};

struct thread_info {
    pthread_t    id;    // id returned by pthread_create()
    struct args *args;  // pointer to the arguments
};

// Threads run on this function
void *decrease_increase(void *ptr)
{
    struct args *args = ptr;
    struct nums *n = args->nums;

    while(1) {
        // Bloqueamos el mutex de iteraciones
        pthread_mutex_lock(args->mutexIt);
        if((*args->iterations) <= 0) { // En caso de acabar las iteraciones desbloqueamos el mutex
            pthread_mutex_unlock(args->mutexIt);
            break;
        }
        (*args->iterations)--; // Desbloqueamos las iteraciones (seccion critica)

        pthread_mutex_unlock(args->mutexIt); // Desbloqueamos el mutex de iteraciones

        // Conseguimos las posiciones aleatorias a incrementar y decrementar en los arrays
        int posInc = rand() % args->size;
        int posDec = rand() % args->size;

        // Bloqueamos las posiciones seguidas para evitar interbloqueos
        pthread_mutex_lock(&n->mutexInc[posInc]);
        pthread_mutex_lock(&n->mutexDec[posDec]);

        // Seccion critica
        n->decrease[posDec]--;
        n->increase[posInc]++;

        // Desbloqueamos las posiciones de los arrays
        pthread_mutex_unlock(&n->mutexDec[posDec]);
        pthread_mutex_unlock(&n->mutexInc[posInc]);
    }
    return NULL;
}

// start opt.num_threads threads running on decrease_incresase
struct thread_info *start_threads(struct options *opt, struct nums *nums)
{
    int i;
    struct thread_info *threads;

    printf("creating %d threads\n", opt->num_threads);
    threads = malloc(sizeof(struct thread_info) * opt->num_threads);

    // Mutex auxiliar para el mutex de iteraciones
    pthread_mutex_t *aux1 = malloc(sizeof(pthread_mutex_t));
    if(pthread_mutex_init(aux1, NULL) != 0) {
        printf("Mutex initialization failed\n");
        exit(1);
    }

    if (threads == NULL) {
        printf("Not enough memory\n");
        exit(1);
    }

    // Asignamos las memorias para los arrays de contadores y los arrays de mutex
    pthread_mutex_t *aux2 = malloc(sizeof(pthread_mutex_t) * opt->size);
    pthread_mutex_t *aux3 = malloc(sizeof(pthread_mutex_t) * opt->size);
    nums->increase = malloc(sizeof(long) * opt->size);
    nums->decrease = malloc(sizeof(long) * opt->size);

    for(int j = 0; j < opt->size; j++) {
        // Inicializamos los arrays de mutex
        if(pthread_mutex_init(&aux2[j], NULL) != 0) {
            printf("Mutex initialization failed\n");
            exit(1);
        }

        if(pthread_mutex_init(&aux3[j], NULL) != 0) {
            printf("Mutex initialization failed\n");
            exit(1);
        }

        // Inicializamos los arrays de contadores
        nums->increase[j] = 0;
        nums->decrease[j] = nums->total;
    }

    // Create num_thread threads running decrease_increase
    for (i = 0; i < opt->num_threads; i++) {
        threads[i].args = malloc(sizeof(struct args));

        threads[i].args->thread_num = i;
        threads[i].args->nums       = nums;
        threads[i].args->iterations = &(opt->iterations);
        threads[i].args->size = opt->size;

        threads[i].args->mutexIt = aux1;
        threads[i].args->nums->mutexDec = aux2;
        threads[i].args->nums->mutexInc = aux3;

        if (0 != pthread_create(&threads[i].id, NULL, decrease_increase, threads[i].args)) {
            printf("Could not create thread #%d", i);
            exit(1);
        }
    }
    return threads;
}

void print_totals(struct nums *nums, int size)
{
    long totalInc = 0, totalDec = 0;

    // Conseguimos el numero total de valores en los arrays
    for(int i = 0; i < size; i++) {
        totalInc += nums->increase[i];
        totalDec += nums->decrease[i];
    }

    printf("Final: increasing %ld decreasing %ld diff %ld\n",
           totalInc, totalDec, (nums->total*size) - (totalDec + totalInc));
}

// wait for all threads to finish, print totals, and free memory
void wait(struct options opt, struct nums *nums, struct thread_info *threads) {
    // Wait for the threads to finish
    for (int i = 0; i < opt.num_threads; i++)
        pthread_join(threads[i].id, NULL);

    print_totals(nums, opt.size);

    // Destruimos los mutex creados antes de liberar los threads
    pthread_mutex_destroy(threads[0].args->mutexIt);

    for (int i = 0; i < opt.num_threads; i++) {
        pthread_mutex_destroy(&threads[i].args->nums->mutexInc[i]);
        pthread_mutex_destroy(&threads[i].args->nums->mutexDec[i]);

        free(threads[i].args);
    }

    // Liberamos la memoria de los mutex (aux1, aux2, aux3)
    free(threads->args->mutexIt);
    free(threads->args->nums->mutexInc);
    free(threads->args->nums->mutexDec);

    // Liberamos la memoria de los arrays y del thread
    free(nums->increase);
    free(nums->decrease);
    free(threads);
}

int main (int argc, char **argv)
{
    struct options opt;
    struct nums nums;
    struct thread_info *thrs;

    srand(time(NULL));

    // Default values for the options
    opt.num_threads  = 4;
    opt.iterations   = 100000;
    opt.size         = 10;

    read_options(argc, argv, &opt);

    nums.total = opt.iterations;
    nums.diff = 0;
    thrs = start_threads(&opt, &nums);
    wait(opt, &nums, thrs);

    return 0;
}
