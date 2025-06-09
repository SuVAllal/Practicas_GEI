/*
 * P3 - SISTEMAS OPERATIVOS (UDC)
 * Alumno 1: Susana Vilarnovo Allal (s.vallal@udc.es)
 * Alumno 2: Laura Palmeiro Castro (l.pcastro@udc.es)
 */

#ifndef P3_PROCLIST_H
#define P3_PROCLIST_H

#define MAXDATA 4096
#define MAXVAR 4096

#include <stdbool.h>
#include <sys/types.h>

typedef enum status {
    FINISHED, STOPPED, SIGNALED, ACTIVE, UNKNOWN
} status;

typedef struct dataProc {
    pid_t pid;
    status estado;
    char * user;
    char * cmd;
    char * date;
    int state;
    int end;
} dataProc;

typedef struct tListP {
    dataProc itemP [MAXDATA];
    int lastPos;
} tListP;

void createEmptyListP(tListP *P);
bool isEmptyListP(tListP P);
int firstP(tListP P);
int lastP(tListP P);
dataProc getItemP(int p, tListP P);
void deleteItemP(int p, tListP *P);
void deleteListP(tListP *P);
void updateItemP(dataProc item, int p, tListP *P);
bool insertItemP(dataProc item, tListP *P);


#endif //P3_PROCLIST_H
