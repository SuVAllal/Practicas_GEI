/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Vilarnovo Allal, Susana LOGIN 1: s.vallal
 * AUTHOR 2: Palmeiro Castro, Laura LOGIN 2: l.pcastro
 * GROUP: 2.4
 * DATE: 30 / 03 / 23
 */

#ifndef JURY_LIST_H
#define JURY_LIST_H

#include "types.h"
#include "participant_list.h"

// DEFINICION DE CONSTANTES Y TIPOS:
#define MAX 10
#define NULLJ (-1)

typedef int tPosJ;

typedef struct {
    tJuryName juryName;
    tNumVotes totalVoters;
    tNumVotes validVotes;
    tNumVotes nullVotes;
    tListP participantList;
} tItemJ;

typedef struct {
    tItemJ dataJ[MAX];
    tPosJ LastPosJ;
} tListJ;

// FUNCIONES:

void createEmptyListJ (tListJ *J);
// Operacion para crear una lista vacia
/*
 Objetivo: Crear una lista vacia e inicializarla
 Salidas: Lista vacia
 PreCD: La lista no contiene datos
 */

bool isEmptyListJ (tListJ J);
// Operacion para determinar si la lista esta vacia
/*
 Objetivo: Determina si la lista esta vacia
 Entradas:
            J: Lista a comprobar
 Salidas:
            True: La lista esta vacia
            False: La lista tiene datos
 */

tPosJ firstJ (tListJ J);
// Operacion para devolver la posicion del primer elemento de la lista
/*
 Objetivo: Devuelve la posicion del primer elemento de la lista
 Entradas:
            J: Lista a manipular
 Salidas:
            Posicion del primer elemento de la lista
 PreCD: La lista no esta vacia
 */

tPosJ lastJ (tListJ J);
// Operacion para devolver la posicion del ultimo elemento de la lista
/*
 Objetivo: Devolver la posicion del ultimo elemento de la lista
 Entradas:
            J: Lista a manipular
 Salidas:
            Posición del ultimo elemento de la lista
 Precondiciones: La lista no esta vacia
 */

tPosJ nextJ (tPosJ p, tListJ J);
// Operacion para devolver la posicion en la lista del elemento siguiente al de la posicion indicada (o NULLJ si no hay posicion siguiente)
/*
 Objetivo: Devuelve la posicion del elemento siguiente al dado
 Entradas:
            p: Posicion dada
            J: Lista a manipular
 Salidas:
            p: Posicion del elemento siguiente o nulo si es el ultimo
 PreCD: p es una posicion valida de la lista
 */

tPosJ previousJ (tPosJ p, tListJ J);
// Operacion para devolver la posicion en la lista del elemento anterior al de la posicion indicada (o NULLJ si no hay posicion anterior)
/*
 Objetivo: Devuelve la posicion del elemento anterior al dado
 Entradas:
            p: Posicion dada
            J: Lista a manipular
 Salidas: Posicion del elemento anterior o nulo si es el primero
 PreCD: p es una posicion valida de la lista
 */

bool insertItemJ (tItemJ d, tListJ *J);
// Operacion que inserta un elemento en la lista
/*
 Objetivo: Insertar un elemento en la lista.
 Entradas:
            d: Contenido del elemento a insertar
            J: Lista donde se va a insertar el elemento
 Salidas:
            J: Lista con el elemento d insertado. True si se ha podido insertar o false si no se ha podido
 */

void deleteAtPositionJ (tPosJ p, tListJ *J);
// Operacion para eliminar de la lista el elemento que ocupa la posicion indicada
/*
 Objetivo: Eliminar de la lista el elemento que ocupa la posicion indicada
 Entradas:
            p: Posicion del elemento a eliminar
            J: Lista a modificar
 Salidas:
            J: Lista sin el elemento correspodiente a la posicion dada
 PreCD: p es una posicion valida de la lista
 PostCD: Las posiciones de los elementos de la lista posteriores a la posicion p pueden haber variado
 */

tItemJ getItemJ (tPosJ p, tListJ J);
// Operacion para devolver el contenido del elemento de la lista que ocupa la posicion indicada
/*
 Objetivo: Recupera el contenido de un elemento de la lista
 Entradas:
            p: Posicion del elemento buscado
            J: Lista donde se buscara el elemento
 Salidas:
 PreCD: p es una posicion valida en la lista
 */

void updateItemJ (tItemJ d, tPosJ p, tListJ *J);
// Operacion para actualizar el contenido del elemento situado en la posicion indicada
/*
 Objetivo: Actualiza el contenido de un elemento de la lista
 Entradas:
            d: Nuevo contenido a asignar al elemento en p
            p: Posicion del elemento que queremos modificar
            J: Lista a modificar
 Salidas:
            J: Lista con el contenido actualizado del elemento
 PreCD: p es una posicion valida de la lista
 */

tPosJ findItemJ (tJuryName d, tListJ J);
// Operacion que devuelve la posicion del primer elemento de la lista cuyo nombre de participante coincide con el dado
/*
 Objetivo: Busca el primer elemento con cierto contenido en la lista
 Entradas:
            d: Contenido del elemento buscado
            J: Lista donde se buscara el elemento
 Salidas:
            p: Posicion del elemento encontrado o nulo si no se encuentra
 */


#endif
