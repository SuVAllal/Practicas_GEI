/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Vilarnovo Allal, Susana LOGIN 1: s.vallal
 * AUTHOR 2: Palmeiro Castro, Laura LOGIN 2: l.pcastro
 * GROUP: 2.4
 * DATE: 30 / 03 / 23
 */

#include "participant_list.h"

void createEmptyListP (tListP *P) {
    *P = NULLP; // Inicializacion de la lista vacia de participantes a NULLP
}


bool isEmptyListP (tListP P) {
    return (P == NULLP); // La comparacion es verdadera si la lista esta vacia (a NULLP), falsa si no lo esta
}


tPosP firstP (tListP P) {
    return P; // Devuelve la posicion del primer elemento de la lista, que esta apuntado por P
}


tPosP lastP (tListP P) {
    tPosP p;

    for(p = P; p -> next != NULLP; p = p -> next); // El bucle recorre toda la lista hasta llegar al ultimo elemento, cuyo campo next es NULLP
    return p; // Devuelve p, que tras el bucle, es el ultimo elemento
}


tPosP nextP (tPosP p, tListP P) {
    return p -> next; // Devuelve la posicion siguiente a p, en caso de ser p la ultima posicion devuelve NULLP
}


tPosP previousP (tPosP p, tListP P) {
    tPosP q;

    if(p == P)
        return NULLP; // Si p es el primer elemento de la lista devuelve NULLP
    else {
        for(q = P; q -> next != p; q = nextP(q, P)); // Si no, recorremos la lista con la variable auxiliar q mientras q->next no apunte a p
        return q; // Cuando lleguemos a la posicion deseada, devolvemos q (elemento anterior a p)
    }
}

// FUNCIONES AUXILIARES PARA insertItem
bool createNodeP (tPosP *p) {
    *p = malloc(sizeof(**p)); // Reserva en un puntero p una direccion de memoria de tamaño suficiente para un tNodeP
    return *p != NULLP; // Si se pudo reservar memoria devuelve true, si no se pudo devuelve false
}

tPosP findPositionP (tListP P, tItemP d) { //PreCD: lista no vacia
    tPosP p;
    p = P;

    while((p->next != NULLP) && (strcmp(p->next->dataP.participantName, d.participantName) < 0)) // Recorremos la lista buscando el puntero con el tItemP buscado
        p = p -> next;
    return p; // Devolvemos el puntero que, si no se ha encontrado el elemento, sera NULLP
}

bool insertItemP (tItemP d, tListP *P) {
    tPosP p, q;

    if(!createNodeP(&q)) // Comprobamos si hay memoria suficiente para insertar, en caso de no haber devolvemos false, no se puede insertar
        return false;
    else {
        q->dataP = d;
        q->next = NULLP;

        if (isEmptyListP(*P)) // CASO 1: la lista esta vacia
            *P = q; // Insertamos el tItemP como primero de la lista
        else if(strcmp(d.participantName, (*P)->dataP.participantName) < 0) { // CASO 2: la lista no esta vacia e insertamos el tItemP antes de la primera posicion de la lista, convirtiéndolo en el primero
            q->next = *P;
            *P = q;
        } else { // CASO 3: insertamos el item en una posicion intermedia o en la ultima
            p = findPositionP(*P, d);
            q->next = p->next;
            p->next = q;
        }
        return true;
    }
}


void deleteAtPositionP (tPosP p, tListP *P) {
    tPosP q;

    if(p == *P) // CASO 1: p es el primer elemento de la lista
        *P = (*P) -> next; // Cambiamos *P por su siguiente
    else {
        if(p -> next == NULLP) { // CASO 2: p es el ultimo elemento de la lista
            for(q = *P; q -> next != p; q = q -> next); // Recorremos la lista hasta llega al ultimo elemento (p)
            q -> next = NULLP; // Igualamos el campo next de q a NULLP, convirtiendolo en el ultimo elemento de la lista
        } else { // CASO 3: p es un elemento intermedio
            q = p -> next; // q apunta al elemento siguiente de p
            p -> dataP = q -> dataP; // Cambiamos el campo dataP de p por el de su siguiente (q)
            p -> next = q -> next; // Ídem con el campo next
            p = q; // Igualamos el resto de campos
        }
    }
    free(p); // Liberamos p
}


tItemP getItemP (tPosP p, tListP P) {
    return p -> dataP; // Devuelve el campo dataP del elemento p deseado
}


void updateItemP (tItemP d, tPosP p, tListP *P) {
    p -> dataP = d; // Cambia el campo dataP del elemento p por el ItemP d deseado
}


tPosP findItemP (tParticipantName d, tListP P) {
    tPosP p;

    for(p = P; (p != NULLP) && (strcmp(p->dataP.participantName, d) != 0); p = p->next); // Recorremos la lista, salimos del bucle si encontramos el elemento o hemos recorrido toda la lista sin encontrarlo
    if((p != NULLP) && (strcmp(p->dataP.participantName, d) == 0)) // Si encontramos el elemento devolvemos la posicion en la que se encuentra
        return p;
    else // Si no devolvemos NULLP, no se encontro el elemento
        return NULLP;
}