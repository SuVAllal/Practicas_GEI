/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 1
 * AUTHOR 1: Vilarnovo Allal, Susana LOGIN 1: s.vallal
 * AUTHOR 2: Palmeiro Castro, Laura LOGIN 2: l.pcastro
 * GROUP: 2.4
 * DATE: 17 / 02 / 23
 */

#ifndef STATIC_LIST_H
#define STATIC_LIST_H

#include "types.h"
#include <string.h>

// DEFINICION DE CONSTANTES Y TIPOS:
#define MAX 25
#define LNULL (-1)

typedef int tPosL;
typedef struct {
    tItemL data[MAX];
    tPosL LastPos;
} tList;

// FUNCIONES:


void createEmptyList (tList *L);
// Operacion para crear una lista vacia
/*
 Objetivo: Crear una lista vacia e inicializarla
 Salidas: Lista vacia
 Precondiciones: Lista sin datos
 */


bool isEmptyList (tList L);
// Operacion para determinar si la lista esta vacia
/*
 Objetivo: Saber si una lista esta vacía
 Entradas:
            L: Lista a comprobar
 Salidas:
            True si la lista esta vacia y false si no lo esta
 */


tPosL first (tList L);
// Operacion para devolver la posicion del primer elemento de la lista
/*
 Objetivo: Devolver la posicion del primer elemento de la lista
 Entradas:
            L: Lista a manipular
 Salidas:
            Posicion del primer elemento de la lista
 Precondiciones: La lista no esta vacia
 */


tPosL last (tList L);
// Operacion para devolver la posicion del ultimo elemento de la lista
/*
 Objetivo: Devolver la posicion del ultimo elemento de la lista
 Entradas:
            L: Lista a manipular
 Salidas:
            Posición del ultimo elemento de la lista
 Precondiciones: La lista no esta vacia
 */


tPosL next (tPosL p, tList L);
// Operacion para devolver la posicion en la lista del elemento siguiente al de la posicion indicada (o LNULL si no hay posicion siguiente)
/*
 Objetivo: Devolver la posicion en la lista del elemento siguiente al de la posicion indicada
 Entradas:
            p: Posicion del elemento indicado
            L: Lista a manipular
 Salidas:
            Posicion del elemento siguiente al de la posicion indicada o nulo si es el ultimo
 Precondiciones: p es una posicion valida de la lista
 */


tPosL previous (tPosL p, tList L);
// Operacion para devolver la posicion en la lista del elemento anterior al de la posicion indicada (o LNULL si no hay posicion anterior)
/*
 Objetivo: Devolver la posicion en la lista del elemento anterior al de la posicion indicada
 Entradas:
            p: Posicion del elemento indicado
            L: Lista a manipular
 Salidas:
            Posicion del elemento anterior al de la posicion indicada o nulo si es el primero
 Precondiciones: p es una posicion valida de la lista
 */


bool insertItem (tItemL d, tPosL p, tList *L);
// Operacion que inserta un elemento en la lista antes de la posicion indicada
/*
 Objetivo: Insertar un elemento en la lista justo antes de la posicion indicada. Si la posicion es nula, añade un elemento al final de la lista
 Entradas:
            d: Contenido del elemento a insertar
            p: Posicion de referencia para la insercion
            L: Lista donde vamos a insertar
 Salidas:
            Lista con el nuevo elemento insertado. Devuelve verdadero si se pudo insertar o falso en caso contrario
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
 Salidas: Lista con el elemento borrado
 Precondiciones: p es una posicion valida de la lista
 Postcondiciones: Las posiciones de los elementos de la lista posteriores a p pueden haber variado
 */


tItemL getItem (tPosL p, tList L);
// Operacion para devolver el contenido del elemento de la lista que ocupa la posicion indicada
/*
 Objetivo: Devolver el contenido del elemento de la lista que ocupa la posicion indicada
 Entradas:
            p: Posicion del elemento buscado
            L: Lista donde realizar la busqueda
 Salidas:
            Contenido del elemento de la posicion indicada
 Precondiciones: p es una posicion valida en la lista
 */


void updateItem (tItemL d, tPosL p, tList *L);
// Operacion para modificar el contenido del elemento situado en la posicion indicada
/*
 Objetivo: Modifica el contenido del elemento situado en la posicion indicada
 Entradas:
            d: Nuevo contenido a asignar al elemento en p
            p: Posicion del elemento que queremos modificar
            L: Lista a modificar
 Salidas:
            Lista con el contenido modificado del elemento de la posicion indicada
 Precondiciones: p es una posicion valida de la lista
 */


tPosL findItem (tParticipantName d, tList L);
// Operacion que devuelve la posicion del primer elemento de la lista cuyo nombre de participante coincide con el indicado
/*
 Objetivo: Devolver la posicion del primer elemento de la lista cuyo nombre de participante coincide con el indicado
 Entradas:
            d: Contenido del elemento buscado
            L: Lista donde realizar la busqueda
 Salidas:
            p: Posicion del primer elemento cuyo nombre de participante coincide con el indicado
 */

#endif
