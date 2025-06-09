/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 1
 * AUTHOR 1: Vilarnovo Allal, Susana LOGIN 1: s.vallal
 * AUTHOR 2: Palmeiro Castro, Laura LOGIN 2: l.pcastro
 * GROUP: 2.4
 * DATE: 11 / 03 / 23
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define MAX_BUFFER 255

#ifdef DYNAMIC_LIST
#include "dynamic_list.h"
#endif
#ifdef STATIC_LIST
#include "static_list.h"
#endif

// Funcion auxiliar para convertir un string en un booleano
tEUParticipant  string2tEUParticipant(char *loc){
    if (strcmp(loc,"eu") == 0) return true;
    return false;
}

// Funcion auxiliar para convertir un booleano en un string
char* tEUParticipant2string(tEUParticipant loc){
    if (loc) return "eu";
    return "non-eu";
}


/* new (L, name, location) -> tList
 * Objetivo: dar de alta a un participante con el contador de votos a 0
 * Entrada:
 *              L: lista en la que se incluira al participante
 *              name: nombre del participante
 *              location: indicador de pertenencia a la Union Europea (eu o non-eu) del participante
 * Salida: lista con el nuevo participante incluido
 * Precondiciones: la lista esta creada
 */

void new(tList *L, tParticipantName name, char *location) {

    // Comprobamos si ya existe el usuario
    if (findItem(name, *L) != LNULL)
        printf("+ Error: New not possible\n");
    else { // Si no existe creamos uno nuevo e implementamos los valores
        tItemL new1;
        tPosL p;
        p = LNULL; // Posicionamos p una posicion despues de la ultima posicion de la lista

        //Declaramos los valores del nuevo participante
        strcpy(new1.participantName, name);
        new1.numVotes = 0;
        new1.EUParticipant = string2tEUParticipant(location);

        // Verificamos si se puede implementar en la lista y, en caso de que se pueda, lo implementamos en la última posición (anterior a p)
        if (insertItem(new1, p, L) == false)
            printf("+ Error: New not possible\n");
        else
            printf("* New: participant %s location %s\n", name, location);
    }
}


/* vote(L, nullvotes, name) -> tList
 * Objetivo: sumar un voto al participante indicado
 * Entrada:
 *              L: lista con los datos de los participantes
 *              nullvotes: votos nulos
 *              name: nombre del participante votado
 * Salida: lista con el numero de votos del participante actualizado
 */

void vote (tList *L, int *nullvotes, tParticipantName name) {
    tPosL q;

    if ((isEmptyList(*L)) || findItem(name, *L) == LNULL ) { //Si la lista esta vacia o no se encuentra el elemento devolvemos error
        printf("+ Error: Vote not possible. Participant %s not found. NULLVOTE\n", name);
        (*nullvotes)++;
    } else {
        q = findItem(name, *L); //Encontramos la posicion del participante buscado
        tItemL participante = getItem(q, *L); //Encontramos el struct del participante
        participante.numVotes++; //Le sumamos un voto
        updateItem(participante, q, L); //Actualizamos los valores del participante en la lista
        printf("* Vote: participant %s location %s numvotes %d\n", name, tEUParticipant2string(participante.EUParticipant), participante.numVotes);
    }
}


/* disqualify (L, nullvotes, name) -> tList
 * Objetivo: eliminar de la lista al participante indicado, convirtiendo sus votos en nulos
 * Entrada:
 *              L: lista de la que se eliminara al participante
 *              nullvotes: votos nulos
 *              name: nombre del participante eliminado
 * Salida: lista actualizada
 */

void disqualify (tList *L, int *nullvotes, tParticipantName name) {
    tPosL p;

    if ((isEmptyList(*L)) || findItem(name, *L) == LNULL ) //Si no se encuentra el participante buscado o la lista esta vacia devolvemos error
        printf("+ Error: Disqualify not possible\n");
    else {
        p = findItem(name, *L); //Posicion del participante a eliminar
        tItemL participante = getItem(p, *L); //Participante a eliminar

        (*nullvotes) += participante.numVotes; //Los votos del participante a eliminar pasan a ser votos nulos
        printf("* Disqualify: participant %s location %s\n", name, tEUParticipant2string(participante.EUParticipant));
        deleteAtPosition(p, L); //Eliminamos el participante
    }
}


/* stats(L, nullvotes, totalvoters) -> string
 * Objetivo: mostrar por pantalla las estadisticas de la lista de participantes
 * Entrada:
 *              L: lista con los datos de los participantes
 *              nullvotes: votos nulos
 *              totalvoters: numero total de votantes/miembros del jurado
 * Salida: estadisticas de participacion y de votos de cada uno de los participantes.
 */

// Funcion auxiliar para convertir un string en un entero
int string2int (char *n) {
    return atoi(n);
}


void stats (tList L, int nullvotes, char* totalvoters) {

    //Comprobamos si la lista esta vacia
    if(isEmptyList(L)) //Si esta vacia devolvemos error
        printf("+ Error: Stats not possible\n");
    else {
        tItemL participante;
        tPosL p, q;

        // Variables necesarias para la estadistica
        unsigned int votos, votosvalidos;
        float pVotos, pPart;

        votos = 0;

        // Bucle para contar el numero de votos totales
        for(p = first(L); p != LNULL; p = next(p, L)) {
            participante = getItem(p, L);
            votos += participante.numVotes;
        }

        votos += nullvotes; //A los votos de los participantes le sumamos los votos nulos para conseguir el numero de votos total

        // Bucle para imprimir la lista
        for(q = first(L); q != LNULL; q = next(q, L)) {
            //Obtenemos los datos del participante
            participante = getItem(q, L);

            votosvalidos = votos - nullvotes; //Obtenemos el numero de votos validos

            if(votosvalidos == 0) //Si no hay votos validos el porcentaje es 0, evitamos dividir por 0
                pVotos = 0;
            else
                pVotos = (participante.numVotes/(float)votosvalidos)*100; //Porcentaje de votos recibidos del participante

            //Escribimos los datos del usuario
            printf("Participant %s location %s numvotes %d (%.2f%%)\n", participante.participantName,
                   tEUParticipant2string(participante.EUParticipant), participante.numVotes, pVotos);
        }

        printf("Null votes %d", nullvotes); //Imprimimos el numero de votos nulos

        int totaljurado = string2int(totalvoters); //Convertimos el string dado con el numero de miembros del jurado a un entero con la funcion auxiliar

        if(totaljurado == 0) //Si no hay miembros en el jurado el porcentaje es 0, evitamos dividir por 0
            pPart = 0;
        else
            pPart = (votos/(float)totaljurado)*100; //Porcentaje de participacion

        // Escribimos los datos de participacion
        printf("\nParticipation: %d votes from %d voters (%.2f%%)\n", votos, totaljurado, pPart);

    }
}



//Funcion para procesar los comandos enviados
void processCommand(tList *L, int *nullvotes, char *commandNumber, char command, char *param1, char *param2) {

    switch (command) {
        case 'N':
            //Comando New
            printf("********************\n");
            printf("%s %c: participant %s location %s\n", commandNumber, command, param1, param2);
            new(L, param1, param2);
            break;
        case 'V':
            //Comando Vote
            printf("********************\n");
            printf("%s %c: participant %s\n", commandNumber, command, param1);
            vote(L, nullvotes, param1);
            break;
        case 'D':
            //Comando Disqualify
            printf("********************\n");
            printf("%s %c: participant %s\n", commandNumber, command, param1);
            disqualify(L, nullvotes, param1);
            break;
        case 'S':
            //Comando Stats
            printf("********************\n");
            printf("%s %c: totalvoters %s\n", commandNumber, command, param1);
            stats(*L, *nullvotes, param1);
            break;
        default:
            break;
    }
}

void readTasks(tList *L, int *nullvotes, char *filename) {
    FILE *f = NULL;
    char *commandNumber, *command, *param1, *param2;
    const char delimiters[] = " \n\r";
    char buffer[MAX_BUFFER];

    f = fopen(filename, "r");

    if (f != NULL) {

        while (fgets(buffer, MAX_BUFFER, f)) {
            commandNumber = strtok(buffer, delimiters);
            command = strtok(NULL, delimiters);
            param1 = strtok(NULL, delimiters);
            param2 = strtok(NULL, delimiters);

            processCommand(L, nullvotes, commandNumber, command[0], param1, param2);
        }

        fclose(f);

    } else {
        printf("Cannot open file %s.\n", filename);
    }
}


int main(int nargs, char **args) {

    char *file_name = "new.txt";

    //Creacion de la lista
    tList L;
    createEmptyList(&L);
    //Creacion de un contador de votos nulos
    int nullvote = 0;

    if (nargs > 1) {
        file_name = args[1];
    } else {
#ifdef INPUT_FILE
        file_name = INPUT_FILE;
#endif
    }

    readTasks(&L, &nullvote, file_name);

    return 0;
}