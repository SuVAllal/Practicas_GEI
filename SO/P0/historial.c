#include "historial.h"

void createHist(tHist *H) {
    *H = HNULL;
}

bool isEmptyHist(tHist H) {
    return(H == HNULL);
}


bool createNode(tPosH *p) {
    *p = malloc(sizeof(**p));
    return *p != HNULL;
}

bool insertCommand(tItemH cmd, tHist *H) {
    tPosH p, q;

    if(!createNode(&p))
        return false;
    else {
        p->data = cmd;
        p->next = HNULL;

        if(*H == HNULL)
            *H = p;
        else {
            for(q = *H; q->next != HNULL; q = q->next);
            q->next = p;
        }
        return true;
    }
}

tPosH first(tHist H) {
    return H;
}

tPosH next(tPosH p, tHist H) {
    return p->next;
}

tItemH getItem(tPosH p, tHist H) {
    return p->data;
}

void clearHist(tHist *H) {
    tPosH p;

    while (*H != HNULL) {
        p = *H;
        *H = (*H)->next;
        free(p);
    }
}