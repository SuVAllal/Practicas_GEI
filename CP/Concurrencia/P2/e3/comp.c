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

// Estructura para los argumentos del thread lector
typedef struct {
    queue in;
    pthread_mutex_t * reader_lock;
    int fd;
    int num_chunks;
    int size;
} reader_args;

// Estructura para los argumentos del threads de compresion
typedef struct {
    queue in;
    queue out;
    pthread_mutex_t * lock;
} thr_args;

// Estructura para los argumentos del thread escritor
typedef struct {
    queue out;
    pthread_mutex_t * writer_lock;
    archive ar;
    int num_chunks;
} writer_args;

// Funcion ejecutada por el thread lector
void *reader(void *args) {
    reader_args *r_args = (reader_args *) args;
    int i, offset;
    int chunks = r_args->num_chunks;
    chunk ch;

    // Iterar sobre el numero de chunks a leer
    for (i = 0; i < chunks; i++) {
        // Bloquear el mutex para evitar conflictos de acceso concurrente
        pthread_mutex_lock(r_args->reader_lock);

        // Crear un nuevo chunk y obtener el offset actual del archivo
        ch = alloc_chunk(r_args->size);
        offset = (int) lseek(r_args->fd, 0, SEEK_CUR);

        // Leer datos del archivo al chunk
        ch->size = (int) read(r_args->fd, ch->data, r_args->size);
        ch->num = i;
        ch->offset = offset;

        // Insertar el chunk en la cola compartida
        q_insert(r_args->in, ch);

        // Desbloquear el mutex despues de insertar el chunk
        pthread_mutex_unlock(r_args->reader_lock);
    }

    // Finalizar el thread lector
    pthread_exit(NULL);
}

// Funcion ejecutada por los threads de compresion
void *worker(void *args) {
    thr_args *targs = (thr_args *)args;
    chunk ch, res;

    // Bucle infinito para procesar chunks continuamente
    while (1) {
        // Bloquear el mutex para evitar conflictos de acceso concurrente
        pthread_mutex_lock(targs->lock);

        // Quitar un chunk de la cola de entrada
        ch = q_remove(targs->in);

        // Verificar si la cola esta vacia y terminar el bucle si es el caso
        if (ch == NULL) {
            pthread_mutex_unlock(targs->lock);
            break;
        }

        // Comprimir el chunk
        res = zcompress(ch);

        // Liberar la memoria del chunk original
        free_chunk(ch);

        // Insertar el resultado comprimido en la cola de salida
        q_insert(targs->out, res);

        // Desbloquear el mutex despues de completar la operacion
        pthread_mutex_unlock(targs->lock);
    }

    // Finalizar el thread de compresion
    pthread_exit(NULL);
}

// Funcion ejecutada por el thread escritor
void *writer(void *args) {
    writer_args *w_args = (writer_args *) args;
    int i;
    int chunks = w_args->num_chunks;
    chunk ch;

    // Iterar sobre el numero de chunks a escribir en el archivo
    for (i = 0; i < chunks; i++) {
        // Bloquear el mutex para evitar conflictos de acceso concurrente
        pthread_mutex_lock(w_args->writer_lock);

        // Quitar el chunk de la cola de salida
        ch = q_remove(w_args->out);

        // Agregar el chunk al archivo
        add_chunk(w_args->ar, ch);

        // Liberar la memoria del chunk
        free_chunk(ch);

        // Desbloquear el mutex despues de completar la operacion
        pthread_mutex_unlock(w_args->writer_lock);
    }

    // Finalizar el thread escritor
    pthread_exit(NULL);
}


// Funcion principal para la compresion de archivos
void comp(struct options opt) {
    int fd, chunks, i;
    char comp_file[256];
    struct stat st;
    archive ar;
    queue in, out;

    pthread_t* threads;
    thr_args* threads_args;

    // Abrir el archivo de entrada para lectura
    if ((fd = open(opt.file, O_RDONLY)) == -1) {
        printf("Cannot open %s\n", opt.file);
        exit(0);
    }

    // Obtener informacion sobre el archivo de entrada
    fstat(fd, &st);

    // Calcular el numero de chunks basandose en el tamaño del archivo y del fragmento
    chunks = (int) st.st_size / opt.size + (st.st_size % opt.size ? 1 : 0);

    // Crear el nombre del archivo de salida comprimido
    if (opt.out_file) {
        strncpy(comp_file, opt.out_file, 255);
    } else {
        strncpy(comp_file, opt.file, 255);
        strncat(comp_file, ".ch", 255);
    }

    // Crear el archivo de salida
    ar = create_archive_file(comp_file);

    // Crear colas compartidas para la comunicacion entre threads
    in = q_create(opt.queue_size);
    out = q_create(opt.queue_size);

    // Inicializar el mutex auxiliar
    pthread_mutex_t *aux = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(aux, NULL);

    // Crear el thread lector
    pthread_t read;
    reader_args readerArgs;

    readerArgs.in = in;
    readerArgs.fd = fd;
    readerArgs.size = opt.size;
    readerArgs.num_chunks = chunks;
    readerArgs.reader_lock = aux;
    pthread_create(&read, NULL, reader, &readerArgs);

    // Inicializar y crear los threads de compresion
    threads = malloc(sizeof(pthread_t) * opt.num_threads);
    threads_args = malloc(sizeof(thr_args) * opt.num_threads);

    for (i = 0; i < opt.num_threads; i++) {
        threads_args[i].in = in;
        threads_args[i].out = out;
        threads_args[i].lock = aux;

        pthread_create(&threads[i], NULL, worker, &threads_args[i]);
    }

    // Crear el thread escritor
    pthread_t write;
    writer_args writerArgs;

    writerArgs.writer_lock = aux;
    writerArgs.ar = ar;
    writerArgs.out = out;
    writerArgs.num_chunks = chunks;
    pthread_create(&write, NULL, writer, &writerArgs);

    // Esperar a que el thread lector termine
    pthread_join(read, NULL);

    // Informar a los threads que no se insertaran mas chunks
    for (i = 0; i < opt.num_threads; i++) {
        q_insert(in, NULL);
    }

    // Esperar a que los threads de compresion terminen
    for (i = 0; i < opt.num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Esperar a que el thread escritor termine
    pthread_join(write, NULL);

    // Destruir el mutex auxiliar y iberar la memoria asociada
    pthread_mutex_destroy(aux);
    free(aux);

    // Cerrar el archivo de compresion y el archivo de entrada
    close_archive_file(ar);
    close(fd);

    // Destruir las colas compartidas
    q_destroy(in);
    q_destroy(out);

    // Liberar la memoria utilizada por los threads y sus argumentos
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