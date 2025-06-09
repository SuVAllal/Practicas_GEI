/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practica 1
 * AUTHOR 1: Susana Vilarnovo Allal LOGIN 1: s.vallal
 * AUTHOR 2: Laura Palmeiro Castro LOGIN 2: l.pcastro
 * GROUP: 2.4
 * DATE: 08 / 03 / 23
 */

#include "dynamic_list.h"

void createEmptyList (tList *L) {
    *L = LNULL; // Inicializacion de la lista vacia a LNULL
}

bool isEmptyList (tList L) {
    return (L == LNULL); // La comparacion es verdadera si la lista esta vacia (a LNULL), falsa si no lo esta
}

tPosL first(tList L) {
    return L; // Devuelve la posicion del primer elemento de la lista, que esta apuntado por L
}

tPosL last (tList L) {
    tPosL p;

    for(p = L; p -> next != LNULL; p = p->next); // El bucle recorre toda la lista hasta llegar al ultimo elemento, cuyo campo next es NULL
    return p; // Devuelve p que, tras el bucle, es el ultimo elemento
}

tPosL next (tPosL p, tList L) {
    return p->next; // Devuelve la posicion siguiente a p, en caso de ser p la ultima posicion devuelve NULL
}

tPosL previous(tPosL p, tList L) {
    tPosL q;

    if(p == L)
        return LNULL; // Si p es el primer elemento de la lista devuelve LNULL
    else {
        for(q = L; q->next != p; q = next(q,L)); // Si no, recorremos la lista con la variable auxiliar q mientras q->next no apunte p
        return q; // Cuando lleguemos a la posicion deseada, devolvemos q (elemento anterior a p)
    }
}

// FUNCIÓN AUXILIAR
bool createNode (tPosL *p) {
    *p = malloc(sizeof(**p)); // Reserva en un puntero p una direccion de memoria de tamaño suficiente para un tNode
    return *p != LNULL; // Si se pudo reservar memoria devuelve true, si no se pudo devuelve false
}

bool insertItem (tItemL d, tPosL p, tList *L) {
    tPosL q, r;

    if (!createNode(&q))
        return false; // La lista esta llena, devuelve false, no se puede insertar
    else {
        q -> data = d;
        q -> next = LNULL;
        if (*L == LNULL) // CASO 1: la lista esta vacia
            *L = q; // Si la lista esta vacia se inserta el elemento como primero
        else if (p == LNULL){ // CASO 2: insertamos el elemento al final de la lista
                for(r = *L; r ->next != LNULL; r = r -> next); // Recorremos la lista hasta llegar al ultimo elemento
                r -> next = q; // Ponemos q como el ultimo elemento, donde esta insertado el item d
            } else if (p == *L) { // CASO 3: insertamos el item en el primer elemento de la lista
                    q -> next = p; // El campo next de la variable auxiliar (con d como item) apunta a p (el primer elemento)
                    *L = q; // Igualamos q y *L, ahora q es el primer elemento de la lista
                } else { // CASO 4: insertamos el item en una posicion intermedia
                    q -> data = p -> data; // Igualamos el campo data de q y p
                    p -> data = d; // Cambiamos el campo data de p insertando d
                    q -> next = p -> next; // El campo next de la variable auxiliar q apunta a la misma que p
                    p -> next = q; // Cambiamos el campo next de p para que apunte a q
                    // Ahora en la variable p esta el item d, y su siguiente es q, cuyos datos son los que tenía antes p
                }
                return true; // La funcion devuelve true si se pudo insertar el item
    }
}

void deleteAtPosition (tPosL p, tList *L) {
    tPosL q;

    if(p == *L) // CASO 1: p es el primer elemento de la lista
        *L = (*L)->next; // Cambiamos *L por su siguiente
    else {
        if(p->next == LNULL) { // CASO 2: p es el ultimo elemento de la lista
            for(q = *L; q -> next != p; q = q->next); // Recorremos la lista hasta llegar a la ultima posicion (p)
            q -> next = LNULL; // Igualamos el campo next de q a LNULL, convirtiendolo en el ultimo elemento de la lista
        } else { // CASO 3: p es un elemento intermedio
            q = p->next; // q apunta al elemento siguiente de p
            p -> data = q -> data; // Cambiamos el campo data de p por el de su siguiente (q)
            p -> next = q -> next;
            p = q; // Igualamos el resto de campos
        }
    }
    free(p); // Liberamos p
}

tItemL getItem (tPosL p, tList L) {
    return p->data; // Devuelve el campo data del elemento p deseado
}

void updateItem (tItemL d, tPosL p, tList *L) {
    p->data = d; // Cambia el campo data del elemento p por el item d deseado
}

tPosL findItem (tParticipantName d, tList L) {
    tPosL p;

    for(p = L; (p != LNULL) && (strcmp(p->data.participantName, d) != 0); p = p->next); // Recorre la lista hasta llegar al ultimo elemento mientras compara los nombres de los participantes
    /* Sale del bucle en dos casos:
     * CASO 1: ha llegado a la ultima posición y no ha encontrado el item, en tal caso devuelve LNULL (a donde apunta la ultima posicion)
     * CASO 2: ha encontrado el item, en tal caso devuelve la posicion donde lo ha encontrado
     */
    return p;
}
