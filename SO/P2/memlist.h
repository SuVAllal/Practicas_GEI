/*
 * P2 - SISTEMAS OPERATIVOS (UDC)
 * Alumno 1: Susana Vilarnovo Allal (s.vallal@udc.es)
 * Alumno 2: Laura Palmeiro Castro (l.pcastro@udc.es)
 */

#ifndef P2_MEMLIST_H
#define P2_MEMLIST_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/mman.h>


#define MAXDATA 4096
#define MAX_LISTMEM 2048
#define MAX_FILENAME 256

typedef struct fich {
    int df;
    char filename[MAX_FILENAME];
} fich;

typedef enum cmd {
    MALLOC, SHARED, MMAP
} cmd;

typedef struct dataMem {
    void * dir;
    size_t size;
    cmd cmdType;
    char date[128];
    union {
        int key;
        fich fichero;
    } Union;
} dataMem;

typedef struct tListM {
    dataMem itemM[MAX_LISTMEM];
    int lastPos;
} tListM;

void createEmptyMem(tListM *M);
bool isEmptyMem(tListM M);
int nextMem(int p, tListM M);
int previousMem(int p, tListM M);
int firstMem(tListM M);
int lastMem(tListM M);
bool insertMem(dataMem new, tListM *M);
dataMem getDataMem(int p, tListM M);
void freeList(tListM *M);
void deleteItemM(int p, tListM *M);
void deleteMem(tListM *M);

#endif //P2_MEMLIST_H
