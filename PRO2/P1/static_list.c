/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practica 1
 * AUTHOR 1: Susana Vilarnovo Allal LOGIN 1: s.vallal
 * AUTHOR 2: Laura Palmeiro Castro LOGIN 2: l.pcastro
 * GROUP: 2.4
 * DATE: 17 / 02 / 23
 */

#include "static_list.h"

void createEmptyList (tList *L) {
    L->LastPos = LNULL; //Creamos la lista inicializandola con la ultima posicion en LNULL, creando una lista vacia
}


bool isEmptyList (tList L) {
    return (L.LastPos == LNULL); //Si la ultima posicion es LNULL, entonces la lista esta vacia, devolvemos true, en cualquier otro caso devolvemos false
}


tPosL first(tList L) {
    return 0; //La primera posicion de una lista no vacia en la implementacion estatica siempre es 0
}


tPosL last (tList L) {
    return L.LastPos; //Devolvemos la ultima posicion
}


tPosL next (tPosL p, tList L) {
    if(p == L.LastPos)
        return LNULL;//Si la posición p es la última de la lista, la siguiente será LNULL
    else
        return ++p;//Si la posición p NO es la última de la lista, la siguiente será ++p
}


tPosL previous(tPosL p, tList L) {
    return --p;// Devuelve --p, si p es la primera posición de la lista devolverá 0-1 = -1 (LNULL).
}


bool insertItem (tItemL d, tPosL p, tList *L) {
    tPosL i;// Declaramos una variable de tipo tPosL (entero) i.

    if (L->LastPos == MAX - 1)// CASO 1: la lista está llena (las 24 posiciones ocupadas), entonces:
        return false;// No se puede insertar un nuevo item.
    else {
        L->LastPos++;// El puntero L apunta a una nueva última posición vacía creada al aumentar en 1 el número de posiciones de la lista
        if(p == LNULL)// CASO 2: la posición indicada es la última de la lista
            L->data[L->LastPos] = d;
        else {// CASO 3: la posición indicada es una posición intermedia de la lista
            for(i = L->LastPos; i >= p+1; i--)// Recorremos la lista desde la nueva última posición hasta la posición indicada p
                L->data[i] = L->data[i-1];//Intercambiamos los items una posición, dejando una posición vacía para el nuevo item
            L->data[i] = d;//Insertamos el item dado en la posición indicada, vacía tras realizar el bucle
        }
        return true;// Se pudo insertar el nuevo item
    }
}


void deleteAtPosition (tPosL p, tList *L) {
    tPosL i;
    L->LastPos--;// El puntero L apunta a una nueva última posición vacía creada al reducir en 1 el número de posiciones de la lista

    for(i = p; i <= L->LastPos; i++)// El puntero L apunta a una nueva última posición vacía creada al reducir en 1 el número de posiciones de la lista
        L->data[i] = L->data[i+1];// Sustituimos el item de la posición i (inicialmente p) por el item de la posición siguiente
}


tItemL getItem (tPosL p, tList L) {
    return L.data[p]; // Devolvemos los valores del participante que este en la posicion dada
}


void updateItem (tItemL d, tPosL p, tList *L) {
    L->data[p] = d; // Cambiamos los datos iniciales del participante por los dados (d)
}


tPosL findItem (tParticipantName d, tList L) {
    tPosL p;// Declaramos una variable de tipo tPosL (entero) p

    if (L.LastPos == LNULL)// CASO 1: la lista está vacía, entonces:
        return LNULL;// devolvemos LNULL
    else {// CASO 2: la lista no está vacía
        for(p=0; ((p<= L.LastPos) && (strcmp(L.data[p].participantName, d) != 0)); p++); //Recorremos la lista con p mientras comparamos el nombre de los participantes, salimos del bucle cuando: p termina de recorrer la lista, o si la comparación devuelve un valor distinto de 0 (las 2 cadenas son distintas)
        if (strcmp(L.data[p].participantName, d) == 0) //CASO 2.1: Salimos del bucle porque las cadenas son iguales
            return p; //Devolvemos la posición p en las que las cadenas son iguales
        else //CASO 2.2: Salimos del bucle porque p terminó de recorrer la lista
            return LNULL; //Devolvemos LNULL, no se pudo encontrar el nombre del participante buscado
    }
}