#ifndef P0_HISTORIAL_H
#define P0_HISTORIAL_H

#define MAXENTRY 1024

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef char command[MAXENTRY];
#define HNULL NULL

typedef struct tItemH {
    command cmd;
} tItemH;

typedef struct tNode * tPosH;

struct tNode {
    tItemH data;
    tPosH next;
};

typedef tPosH tHist;

void createHist(tHist *H);
bool isEmptyHist(tHist H);
bool insertCommand(tItemH cmd, tHist *H);
tPosH first(tHist H);
tPosH next(tPosH p, tHist H);
tItemH getItem(tPosH p, tHist H);
void clearHist(tHist *H);


#endif //P0_HISTORIAL_H
