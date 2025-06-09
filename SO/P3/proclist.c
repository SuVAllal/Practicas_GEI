/*
 * P3 - SISTEMAS OPERATIVOS (UDC)
 * Alumno 1: Susana Vilarnovo Allal (s.vallal@udc.es)
 * Alumno 2: Laura Palmeiro Castro (l.pcastro@udc.es)
 */

#include "proclist.h"

void createEmptyListP(tListP *P) {
    P->lastPos = -1;
}

bool isEmptyListP(tListP P) {
    return P.lastPos == -1;
}

int firstP(tListP P) {
    return 0;
}

int lastP(tListP P) {
    return P.lastPos;
}

dataProc getItemP(int p, tListP P) {
    return P.itemP[p];
}

void deleteItemP(int p, tListP *P) {
    int i;

    for(i = 0; i < lastP(*P); i++) {
        if(i >= p)
            P->itemP[i] = P->itemP[i+1];
    }

    P->lastPos--;
}

void deleteListP(tListP *P) {
    int i;

    for(i = 0; i <= P->lastPos; i++) {
        deleteItemP(i, P);
        P->lastPos--;
    }
}

void updateItemP(dataProc item, int p, tListP *P) {
    int i;

    for(i = 0; i <= lastP(*P); i++) {
        if(i == p)
            P->itemP[i] = item;
    }
}

bool insertItemP(dataProc item, tListP *P) {
    if(P->lastPos < MAXDATA) {
        P->lastPos++;
        P->itemP[P->lastPos] = item;
        return true;
    } else
        return false;
}