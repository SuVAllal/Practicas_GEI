/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Vilarnovo Allal, Susana LOGIN 1: s.vallal
 * AUTHOR 2: Palmeiro Castro, Susana LOGIN 2: l.pcastro
 * GROUP: 2.4
 * DATE: 30 / 03 / 23
 */

#include "jury_list.h"
#include "participant_list.h"

void createEmptyListJ (tListJ *J) {
    J -> LastPosJ = NULLJ; // Creamos la lista inicializandola con la ultima posicion en NULLJ, creando una lista vacia
}


bool isEmptyListJ (tListJ J) {
    return(J.LastPosJ == NULLJ); // Si la ultima posicion es NULLJ, entonces la lista esta vacia y devuelve true, en cualquier otro caso devolvemos false
}


tPosJ firstJ (tListJ J) {
    return 0; // La primera posicion de una lista no vacia en la implementacion estatica siempre es 0
}


tPosJ lastJ (tListJ J) {
    return J.LastPosJ; // Devolvemos la ultima posicion
}


tPosJ nextJ (tPosJ p, tListJ J) {
    if (p == J.LastPosJ)
        return NULLJ; // Si la posicion p es la ultima de la lista, la siguiente sera NULLJ
    else
        return ++p; // Si la posición p NO es la última de la lista, la siguiente será ++p
}


tPosJ previousJ (tPosJ p, tListJ J) {
    return --p;// Devuelve --p, si p es la primera posición de la lista devolverá 0-1 = -1 (NULLJ).
}


bool insertItemJ (tItemJ d, tListJ *J) {
    tPosJ i;

    if (J -> LastPosJ == MAX-1) // Comprobamos que hay espacio en el array para insertar
        return false;
    else {
        if (isEmptyListJ(*J) || strcmp(d.juryName, J->dataJ[J->LastPosJ].juryName) > 0) { // Si la lista esta vacia o queremos insertar en la ultima posicion
            J -> LastPosJ++;
            J -> dataJ[J -> LastPosJ] = d;
        }
        else { // Queremos insertar en cualquier otra posicion
            J -> LastPosJ++;

            for (i = J->LastPosJ; i > 0 && strcmp(d.juryName, J->dataJ[i-1].juryName) < 0; i--) // Recorremos la lista por orden hasta llegar al que sera el elemento anterior al dado
                // Establecemos los valores:
                J -> dataJ[i] = J -> dataJ[i-1];
            J -> dataJ[i] = d;
        }
        return true;
    }
}


void deleteAtPositionJ (tPosJ p, tListJ *J) {
    tPosJ i;
    J -> LastPosJ--; // Reducimos el numero de posiciones de la lista para eliminar

    for(i = p; i <= J->LastPosJ;i++) // Recorremos la lista desde la posicion dada
        J->dataJ[i] = J->dataJ[i+1]; // Intercambiamos los valores de la lista, eliminando el solicitado
}


tItemJ getItemJ (tPosJ p, tListJ J) {
    return J.dataJ[p]; // Devolvemos los valores del jurado que este en la posicion dada
}


void updateItemJ (tItemJ d, tPosJ p, tListJ *J) {
    J->dataJ[p] = d; // Cambiamos los datos iniciales del jurado por los dados (d)
}


tPosJ findItemJ (tJuryName d, tListJ J) {
    tPosJ p;

    if(isEmptyListJ(J)) // CASO 1: la lista esta vacia, el dato solicitado no existe en la lista, devolvemos NULLJ
        return NULLJ;
    else { // CASO 2: la lista no esta vacia
        for(p = 0; (p < J.LastPosJ) && (strcmp(J.dataJ[p].juryName, d) != 0); p++); // Recorremos la lista, salimos del bucle si encontramos el elemento o terminamos de recorrer sin encontrarlo

        if((p <= J.LastPosJ) && (strcmp(J.dataJ[p].juryName, d) == 0)) // Comprobamos el motivo de la salida del bucle
            return p; // Si encontramos el elemento devolvemos su posicion
        else
            return NULLJ; // Si no devolvemos NULLJ, no se pudo encontrar
    }
}

