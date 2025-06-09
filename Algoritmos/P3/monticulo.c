#include <stdio.h>
#include <stdlib.h>
#include "monticulo.h"

// IMPLEMENTACION DE LAS FUNCIONES:

void inicializarMonticulo(pmonticulo * m) {
    *m = malloc(sizeof (struct monticulo));
    (*m)->ultimo = -1;
}

// Funcion auxiliar para crearMonticulo y quitarMenor:
void Hundir(pmonticulo m, int i) {
    int hijoDer, hijoIzq, j, aux;

    do {
        hijoIzq = 2 * i + 1;
        hijoDer = 2 * i + 2;
        j = i;

        if(hijoDer <= m->ultimo && m->vector[hijoDer] < m->vector[i])
            i = hijoDer;

        if(hijoIzq <= m->ultimo && m->vector[hijoIzq] < m->vector[i])
            i = hijoIzq;

        aux = m->vector[i];
        m->vector[i] = m->vector[j];
        m->vector[j] = aux;
    } while(j != i);

}


// Funcion 1:
void crearMonticulo(const int v[], int n, pmonticulo m) {
    int i;

    for(i = 0; i < n; i++)
        m->vector[i] = v[i];

    m->ultimo = n - 1;

    for(i = n/2; i >= 0; i--)
        Hundir(m, i);
}

//Funcion 2:
int quitarMenor(pmonticulo m) {
    int x;

    if(m->ultimo == -1) {
        printf("Error: monticulo vacio\n");
        return -1;
    } else {
        x = m->vector[0];
        m->vector[0] = m->vector[m->ultimo];
        m->ultimo--;

        if(m->ultimo >= 0)
            Hundir(m, 0);

        return x;
    }
}


// Funcion 3:
/*void ordenarPorMonticulos(int v[], int n) {
    int i;
    struct monticulo m;

    crearMonticulo(v, n, &m);

    for(i = 0; i < n; i++)
        v[i] = quitarMenor(&m);

}*/

void borrarMonticulo(pmonticulo * m) {
    if(*m != NULL) {
        free(*m);
        *m = NULL;
    }
}