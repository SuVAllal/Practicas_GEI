/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practica 1
 * AUTHOR 1: Susana Vilarnovo Allal  LOGIN 1: s.vallal
 * AUTHOR 2: Laura Palmeiro Castro  LOGIN 2: l.pcastro
 * GROUP: 2.4
 * DATE: 08 / 03 / 23
 */

#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H

#include "types.h"
#include <string.h>
#include <stdlib.h>

// DEFINICION DE CONSTANTES Y TIPOS:
#define LNULL NULL

typedef struct tNode * tPosL;

struct tNode {
    tItemL data;
    tPosL next;
};

typedef tPosL tList;

// FUNCIONES:


void createEmptyList (tList *L);
// Operacion para crear una lista vacia
/*
 Objetivo: Crea una lista y la inicializa
 Salidas: Lista vacia
 Postcondiciones: La lista no tiene datos
 */


bool isEmptyList (tList L);
// Operacion para determinar si la lista esta vacia
/*
 Objetivo: Determina si la lista esta vacia
 Entradas:
            L: Lista a comprobar
 Salidas:
            True: La lista esta vacia
            False: La lista tiene datos
 */


tPosL first (tList L);
// Operacion para devolver la posicion del primer elemento de la lista
/*
 Objetivo: Devuelve la posicion del primer elemento de la lista
 Entradas:
            L: Lista a manipular
 Salidas:
            Posicion del primer elemento de la lista
 Precondiciones: La lista no esta vacia
 */


tPosL last (tList L);
// Operacion para devolver la posicion del ultimo elemento de la lista
/*
 Objetivo: Devuelve la posicion del ultimo elemento de la lista
 Entradas:
            L: Lista a manipular
 Salidas:
            Posicion del ultimo elemento de la lista
 Precondiciones: La lista no esta vacia
 */


tPosL next (tPosL p, tList L);
// Operacion para devolver la posicion en la lista del elemento siguiente al de la posicion indicada (o LNULL si no hay posicion siguiente)
/*
 Objetivo: Devuelve la posicion del elemento siguiente al actual
 Entradas:
            p: Posicion del elemento actual
            L: Lista a manipular
 Salidas:
            p: Posicion del elemento siguiente o nulo si es el ultimo
 Precondiciones: p es una posicion valida de la lista
 */


tPosL previous (tPosL p, tList L);
// Operacion para devolver la posicion en la lista del elemento anterior al de la posicion indicada (o LNULL si no hay posicion anterior)
/*
 Objetivo: Devuelve la posicion del elemento anterior al actual
 Entradas:
            p: Posicion del elemento actual
            L: Lista a manipular
 Salidas: Posicion del elemento anterior o nulo si es el primero
 Precondiciones: p es una posicion valida de la lista
 */


bool insertItem (tItemL d, tPosL p, tList *L);
// Operacion que inserta un elemento en la lista antes de la posicion indicada
/*
 Objetivo: Insertar un elemento en la lista justo antes de la posicion indicada. Si la posicion es nula, añade un elemento al final de la lista
 Entradas:
            d: Contenido del elemento a insertar
            p: Posicion en la que se desea insertar el elemento
            L: Lista donde se va a insertar el elemento
 Salidas:
            L: Lista con el elemento d insertado. True si se ha podido insertar o false si no se ha podido
 Precondiciones: p es una posicion valida de la lista o es una posicion nula
 Postcondiciones: Las posiciones de los elementos de la lista posteriores a la del elemento insertado pueden haber variado
 */


void deleteAtPosition (tPosL p, tList *L);
// Operacion para eliminar de la lista el elemento que ocupa la posicion indicada
/*
 Objetivo: Eliminar de la lista el elemento que ocupa la posicion indicada
 Entradas:
            p: Posicion del elemento a eliminar
            L: Lista a modificar
 Salidas:
            L: Lista sin el elemento correspodiente a p
 Precondiciones: p es una posicion valida de la lista
 Postcondiciones: Las posiciones de los elementos de la lista posteriores a la posicion p pueden haber variado
 */


tItemL getItem (tPosL p, tList L);
// Operacion para devolver el contenido del elemento de la lista que ocupa la posicion indicada
/*
 Objetivo: Recupera el contenido de un elemento de la lista
 Entradas:
            p: Posicion del elemento buscado
            L: Lista donde se buscara el elemento
 Salidas:
 Precondiciones: p es una posicion valida en la lista
 */


void updateItem (tItemL d, tPosL p, tList *L);
// Operacion para modificar el contenido del elemento situado en la posicion indicada
/*
 Objetivo: Modifica el contenido de un elemento de la lista
 Entradas:
            d: Nuevo contenido a asignar al elemento en p
            P: Posicion del elemento que queremos modificar
            L: Lista a modificar
 Salidas:
            L: Lista con el contenido del elemento modificado
 Precondiciones: P es una posicion valida de la lista
 */


tPosL findItem (tParticipantName d, tList L);
// Operacion que devuelve la posicion del primer elemento de la lista cuyo nombre de participante coincide con el indicado
/*
 Objetivo: Busca el primer elemento con cierto contenido en la lista
 Entradas:
            d: Contenido del elemento buscado
            L: Lista donde se buscara el elemento
 Salidas:
            p: Posicion del elemento encontrado o nulo si no se encuentra
 */

#endif