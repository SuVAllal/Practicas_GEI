#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include "compress.h"
#include "chunk_archive.h"
#include "queue.h"
#include "options.h"

#define CHUNK_SIZE (1024*1024)
#define QUEUE_SIZE 20

#define COMPRESS 1
#define DECOMPRESS 0

// Estructura para almacenar los argumentos de los hilos
typedef struct {
    queue in;
    queue out;
    pthread_mutex_t * lock;
} thr_args;


// take chunks from queue in, run them through process (compress or decompress), send them to queue out
void *worker(void *args) {
    thr_args *targs = (thr_args *)args;
    chunk ch, res;

    while (1) {
        pthread_mutex_lock(targs->lock);

        // Extrae un fragmento de la cola de entrada
        ch = q_remove(targs->in);
        if (ch == NULL) {
            pthread_mutex_unlock(targs->lock);
            break;
        }

        // Comprime o descomprime el fragmento
        res = zcompress(ch); // O zdecompress, dependiendo del contexto
        free_chunk(ch);

        // Inserta el resultado en la cola de salida
        q_insert(targs->out, res);

        pthread_mutex_unlock(targs->lock);
    }

    pthread_exit(NULL);
}


// Compress file taking chunks of opt.size from the input file,
// inserting them into the in queue, running them using a worker,
// and sending the output from the out queue into the archive file
void comp(struct options opt) {
    int fd, chunks, i, offset;
    char comp_file[256];
    struct stat st;
    archive ar;
    queue in, out;
    chunk ch;

    pthread_t* threads;
    thr_args* threads_args;

    // Abre el archivo de entrada para lectura
    if ((fd = open(opt.file, O_RDONLY)) == -1) {
        printf("Cannot open %s\n", opt.file);
        exit(0);
    }

    // Obtiene información sobre el archivo
    fstat(fd, &st);

    // Calcula el número de fragmentos basándose en el tamaño del archivo y del fragmento
    chunks = st.st_size / opt.size + (st.st_size % opt.size ? 1 : 0);

    // Crea el nombre del archivo comprimido de salida
    if (opt.out_file) {
        strncpy(comp_file, opt.out_file, 255);
    } else {
        strncpy(comp_file, opt.file, 255);
        strncat(comp_file, ".ch", 255);
    }

    // Crea el archivo de salida comprimido
    ar = create_archive_file(comp_file);

    // Crea colas de entrada y salida
    in = q_create(opt.queue_size);
    out = q_create(opt.queue_size);

    // Asigna memoria para los threads y argumentos de los threads
    threads = malloc(sizeof(pthread_t) * opt.num_threads);
    threads_args = malloc(sizeof(thr_args) * opt.num_threads);

    // Inicializa un mutex auxiliar
    pthread_mutex_t *aux = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(aux, NULL);

    // Inicializa los argumentos de los threads y crea los threads
    for (i = 0; i < opt.num_threads; i++) {
        threads_args[i].in = in;
        threads_args[i].out = out;
        threads_args[i].lock = aux;

        pthread_create(&threads[i], NULL, worker, &threads_args[i]);
    }

    // Lee el archivo de entrada e inserta chunks en la cola de entrada
    for (i = 0; i < chunks; i++) {
        ch = alloc_chunk(opt.size);

        offset = lseek(fd, 0, SEEK_CUR);

        ch->size = read(fd, ch->data, opt.size);
        ch->num = i;
        ch->offset = offset;

        q_insert(in, ch);
    }

    // Indica a los threads que no se insertarán más chunks
    for (i = 0; i < opt.num_threads; i++) {
        q_insert(in, NULL);
    }

    // Espera a que los threads terminen
    for (i = 0; i < opt.num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Envía chunks al archivo comprimido de salida
    for (i = 0; i < chunks; i++) {
        ch = q_remove(out);

        add_chunk(ar, ch);
        free_chunk(ch);
    }

    // Destruye el mutex auxiliar y libera la memoria
    pthread_mutex_destroy(aux);
    free(aux);

    // Cierra el archivo comprimido de salida y el archivo de entrada
    close_archive_file(ar);
    close(fd);

    // Destruye las colas de entrada y salida y libera la memoria
    q_destroy(in);
    q_destroy(out);
    free(threads_args);
    free(threads);
}


// Decompress file taking chunks of size opt.size from the input file

void decomp(struct options opt) {
    int fd, i;
    char uncomp_file[256];
    archive ar;
    chunk ch, res;

    if((ar=open_archive_file(opt.file))==NULL) {
        printf("Cannot open archive file\n");
        exit(0);
    }

    if(opt.out_file) {
        strncpy(uncomp_file, opt.out_file, 255);
    } else {
        strncpy(uncomp_file, opt.file, strlen(opt.file) -3);
        uncomp_file[strlen(opt.file)-3] = '\0';
    }

    if((fd=open(uncomp_file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH))== -1) {
        printf("Cannot create %s: %s\n", uncomp_file, strerror(errno));
        exit(0);
    }

    for(i=0; i<chunks(ar); i++) {
        ch = get_chunk(ar, i);

        res = zdecompress(ch);
        free_chunk(ch);

        lseek(fd, res->offset, SEEK_SET);
        write(fd, res->data, res->size);
        free_chunk(res);
    }

    close_archive_file(ar);
    close(fd);
}

int main(int argc, char *argv[]) {
    struct options opt;

    opt.compress    = COMPRESS;
    opt.num_threads = 3;
    opt.size        = CHUNK_SIZE;
    opt.queue_size  = QUEUE_SIZE;
    opt.out_file    = NULL;

    read_options(argc, argv, &opt);

    if(opt.compress == COMPRESS) comp(opt);
    else decomp(opt);
}