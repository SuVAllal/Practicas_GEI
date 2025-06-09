/*
 * P3 - SISTEMAS OPERATIVOS (UDC)
 * Alumno 1: Susana Vilarnovo Allal (s.vallal@udc.es)
 * Alumno 2: Laura Palmeiro Castro (l.pcastro@udc.es)
 */

#include "historic.h"

void createEmptyList(tList *L) {
    L->lastPos = -1;
}


bool isEmptyList(tList L) {
    return L.lastPos == -1;
}


int next(int p, tList L) {
    if(p == L.lastPos)
        return -1;
    else
        return ++p;
}


int previous(int p, tList L) {
    return --p;
}


int first(tList L) {
    return 0;
}


int last(tList L) {
    return L.lastPos;
}


bool insertItem(char cmd[], tList *L) {
    if(L->lastPos < MAXDATA) {
        L->lastPos++;
        char *aux = malloc(sizeof(char[MAX]));

        L->cmd[L->lastPos] = aux;
        strcpy(L->cmd[L->lastPos], cmd);

        return true;
    } else
        return false;
}


char * getItem(int p, tList L) {
    if(p < MAX && p >= 0)
        return L.cmd[p];
    else
        return NULL;
}


void deleteList(tList *L) {
    int i;
    for(i = last(*L); i >= 0; i--) {
        free(L->cmd[i]);
        L->lastPos--;
    }
}