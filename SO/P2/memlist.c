/*
 * P2 - SISTEMAS OPERATIVOS (UDC)
 * Alumno 1: Susana Vilarnovo Allal (s.vallal@udc.es)
 * Alumno 2: Laura Palmeiro Castro (l.pcastro@udc.es)
 */

#include "memlist.h"

void createEmptyMem(tListM *M) {
    M->lastPos = -1;
}


bool isEmptyMem(tListM M) {
    return M.lastPos == -1;
}


int nextMem(int p, tListM M) {
    if(p == M.lastPos)
        return -1;
    else
        return ++p;
}


int previousMem(int p, tListM M) {
    return --p;
}


int firstMem(tListM M) {
    return 0;
}


int lastMem(tListM M) {
    return M.lastPos;
}


bool insertMem(dataMem new, tListM *M) {
    if(M->lastPos < MAX_LISTMEM) {
        M->lastPos++;
        M->itemM[M->lastPos] = new;
        return true;
    } else
        return false;
}

dataMem getDataMem(int p, tListM M) {
    return M.itemM[p];
}

void freeList(tListM *M) {
    int pos;
    dataMem item;

    for(pos = firstMem(*M); pos <= lastMem(*M); pos++) {
        item = getDataMem(pos, *M);

        if(item.cmdType == MALLOC)
            free(item.dir);
        else if(item.cmdType == SHARED) {
            if(shmget(item.Union.key, 0, 0) != -1) {
                if(shmdt(item.dir) == -1)
                    return;
            }
        } else if(item.cmdType == MMAP)
            munmap(item.dir, item.size);
    }
}

void deleteItemM(int p, tListM *M) {
    int i;

    for(i = 0; i < lastMem(*M); i++) {
        if (i >= p)
            M->itemM[i] = M->itemM[i + 1];
    }
    M->lastPos--;
}


void deleteMem(tListM *M) {
    int i;

    for(i = 0; i <= M->lastPos; i--) {
        deleteItemM(i, M);
        M->lastPos--;
    }
}
