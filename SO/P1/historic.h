#ifndef P1_HISTORIC_H
#define P1_HISTORIC_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MAXDATA 4096
#define MAX 1024

typedef struct tList {
    char *cmd[MAX];
    int lastPos;
} tList;

void createEmptyList(tList *L);
bool isEmptyList(tList L);
int next(int p, tList L);
int previous(int p, tList L);
int first(tList L);
int last(tList L);
bool insertItem(char cmd[], tList *L);
char * getItem(int p, tList L);
void deleteList(tList *L);

#endif //P1_HISTORIC_H
