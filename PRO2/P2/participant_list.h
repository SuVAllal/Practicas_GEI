/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Vilarnovo Allal, Susana LOGIN 1: s.vallal
 * AUTHOR 2: Palmeiro Castro, Laura LOGIN 2: l.pcastro
 * GROUP: 2.4
 * DATE: 30 / 03 / 23
 */

#ifndef PARTICIPANT_LIST_H
#define PARTICIPANT_LIST_H

#include "types.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// DEFINICION DE CONSTANTES Y TIPOS:
#define NULLP NULL

typedef struct tItemP {
    tParticipantName participantName;
    tNumVotes numVotes;
    tEUParticipant EuParticipant;
} tItemP;

typedef struct tNodeP* tPosP;
struct tNodeP {
    tItemP dataP;
    tPosP next;
};

typedef tPosP tListP;

// FUNCIONES:

void createEmptyListP (tListP *P);
// Operacion para crear una lista vacia
/*
 Objetivo: Crea una lista y la inicializa
 Salidas: Lista vacia
 Postcondiciones: La lista no contiene datos
 */


bool isEmptyListP (tListP P);
// Operacion para determinar si la lista esta vacia
/*
 Objetivo: Determina si la lista esta vacia
 Entradas:
            P: Lista a comprobar
 Salidas:
            True: La lista esta vacia
            False: La lista tiene datos
 */


tPosP firstP (tListP P);
// Operacion para devolver la posicion del primer elemento de la lista
/*
 Objetivo: Devuelve la posicion del primer elemento de la lista
 Entradas:
            P: Lista a manipular
 Salidas:
            Posicion del primer elemento de la lista
 PreCD: La lista no esta vacia
 */

tPosP lastP (tListP P);
// Operacion para devolver la posicion del ultimo elemento de la lista
/*
 Objetivo: Devuelve la posicion del ultimo elemento de la lista
 Entradas:
            P: Lista a manipular
 Salidas:
            Posicion del ultimo elemento de la lista
 PreCD: La lista no esta vacia
 */

tPosP nextP (tPosP p, tListP P);
// Operacion para devolver la posicion en la lista del elemento siguiente al de la posicion indicada (o NULLP si no hay posicion siguiente)
/*
 Objetivo: Devuelve la posicion del elemento siguiente al dado
 Entradas:
            p: Posicion dada
            P: Lista a manipular
 Salidas:
            p: Posicion del elemento siguiente o nulo si es el ultimo
 PreCD: p es una posicion valida de la lista
 */

tPosP previousP (tPosP p, tListP P);
// Operacion para devolver la posicion en la lista del elemento anterior al de la posicion indicada (o NULLP si no hay posicion anterior)
/*
 Objetivo: Devuelve la posicion del elemento anterior al dado
 Entradas:
            p: Posicion dada
            P: Lista a manipular
 Salidas: Posicion del elemento anterior o nulo si es el primero
 PreCD: p es una posicion valida de la lista
 */

bool insertItemP (tItemP d, tListP *P);
// Operacion que inserta un elemento en la lista
/*
 Objetivo: Insertar un elemento en la lista.
 Entradas:
            d: Contenido del elemento a insertar
            P: Lista donde se va a insertar el elemento
 Salidas:
            P: Lista con el elemento d insertado. True si se ha podido insertar o false si no se ha podido
 */

void deleteAtPositionP (tPosP p, tListP *P);
// Operacion para eliminar de la lista el elemento que ocupa la posicion indicada
/*
 Objetivo: Eliminar de la lista el elemento que ocupa la posicion indicada
 Entradas:
            p: Posicion del elemento a eliminar
            P: Lista a modificar
 Salidas:
            P: Lista sin el elemento correspodiente a la posicion dada
 PreCD: p es una posicion valida de la lista
 PostCD: Las posiciones de los elementos de la lista posteriores a la posicion p pueden haber variado
 */

tItemP getItemP (tPosP p, tListP P);
// Operacion para devolver el contenido del elemento de la lista que ocupa la posicion indicada
/*
 Objetivo: Recupera el contenido de un elemento de la lista
 Entradas:
            p: Posicion del elemento buscado
            P: Lista donde se buscara el elemento
 Salidas:
 PreCD: p es una posicion valida en la lista
 */

void updateItemP (tItemP d, tPosP p, tListP *P);
// Operacion para actualizar el contenido del elemento situado en la posicion indicada
/*
 Objetivo: Actualiza el contenido de un elemento de la lista
 Entradas:
            d: Nuevo contenido a asignar al elemento en p
            P: Posicion del elemento que queremos modificar
            P: Lista a modificar
 Salidas:
            P: Lista con el contenido actualizado del elemento
 PreCD: p es una posicion valida de la lista
 */

tPosP findItemP (tParticipantName d, tListP P);
// Operacion que devuelve la posicion del primer elemento de la lista cuyo nombre de participante coincide con el dado
/*
 Objetivo: Busca el primer elemento con cierto contenido en la lista
 Entradas:
            d: Contenido del elemento buscado
            P: Lista donde se buscara el elemento
 Salidas:
            p: Posicion del elemento encontrado o nulo si no se encuentra
 */


#endif
