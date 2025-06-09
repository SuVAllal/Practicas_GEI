/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Vilarnovo Allal, Susana LOGIN 1: s.vallal
 * AUTHOR 2: Palmeiro Castro, Laura LOGIN 2: l.pcastro
 * GROUP: 2.4
 * DATE: 30 / 03 / 23
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "jury_list.h"

#define MAX_BUFFER 255

#define ERROR(message) printf("+ Error: " message "\n");

// Funcion auxiliar para convertir un string en un entero
int string2int (char *n) {
    return atoi(n);
}

// Funcion auxiliar para convertir un string en un booleano
tEUParticipant string2tEUParticipant(char *loc) {
    if (strcmp (loc, "eu") == 0) return true;
    return false;
}

// Funcion auxiliar para convertir un booleano en un string
char* tEUParticipant2string(tEUParticipant loc) {
    if (loc) return "eu";
    return "non-eu";
}


/*
 Objetivo: Crear un nuevo jurado
 Entrada:
            *J: Lista donde se insertará el nuevo jurado
            nameJ: Nombre del jurado
            totalvoters: Número de miembros del jurado
 Salidas:
            Mensaje de error si no se pudo insertar o hubo algún problema para insertarlo
            Mensaje informando de que el jurado se ha creado y muestra el nombre del jurado y el número total de miembros del jurado
            */
void Create(tListJ *J, tJuryName nameJ, char* totalvoters) {

    //Comprobamos si ya existe el jurado:
    if (findItemJ(nameJ, *J) != NULLJ) {
        ERROR("Create not possible");
    } else { // Si no existe creamos uno nuevo e implementamos sus valores
        tItemJ jury;

        //Valores del nuevo jurado:
        strcpy(jury.juryName, nameJ); // Nombre del jurado
        jury.validVotes = 0; // Votos validos inicializados a 0
        jury.nullVotes = 0; // Votos nulos inicializados a 0
        jury.totalVoters = string2int(totalvoters); // Numero de miembros del jurado
        createEmptyListP(&jury.participantList); // Creacion de la lista VACIA de participantes asignada a ese jurado

        //Comprobamos si se puede añadir a la lista de jurados y, en caso de que se pueda, lo implementamos de forma ordenada
        if(insertItemJ(jury, J) == false) {
            ERROR("Create not possible"); // Si no se puede imprimimos error
        } else {
            printf("* Create: jury %s totalvoters %d\n", nameJ, string2int(totalvoters));
        }
    }

}



/*
 Objetivo: Dar de alta a un participante en un jurado
 Entrada:
            *J: Lista a manipular
            nameJ: nombre del jurado al que se le añadirá el participante
            nameP: Nombre del participante
            location: Indica la categoria del participante, "eu" o "non-eu"
 Salidas:
            Mensaje de error si el participante ya existe en ese jurado o hay algún problema al añadirlo
            Mensaje informando de que el participante se pudo añadir, indicando el nombre del jurado, el del participante, y si pertenece o no a la Unión Europea
 PreCD: El participante se inicializará con 0 votos
 */
void New(tListJ *J, tJuryName nameJ, tParticipantName nameP, char* location) {

    // Comprobamos si la lista de jurados esta vacia
    if(isEmptyListJ(*J)){
        ERROR("New not possible");
        return;
    }

    tPosJ juryPos; // Posicion del jurado
    tItemJ jury; // Informacion del jurado

    juryPos = findItemJ(nameJ, *J);
    jury = getItemJ(juryPos, *J);


    // Creacion del participante
    tItemP newP;
    // Inicializacion de los valores del participante
    strcpy(newP.participantName, nameP);
    newP.numVotes = 0;
    newP.EuParticipant = string2tEUParticipant(location);

    // Comprobamos si ya existe:
    if (findItemP(nameP, jury.participantList) != NULLP) {
        ERROR("New not possible");
        return;
    }

    // Insertamos el participante nuevo al jurado
    if(insertItemP(newP, &jury.participantList) == false) {
        ERROR("New not possible"); // Si no se puede insertar imprimimos mensaje de error
    } else { // Si se pudo insertar actualizamos la lista de jurados e imprimimos mensaje de exito
        updateItemJ(jury, juryPos, J);
        printf("* New: jury %s participant %s location %s\n", nameJ, nameP, location);
    }
}


/*
 Objetivo: Sumar un voto para un participante en un jurado
 Entrada:
            *J: Lista a manipular
            nameJ: Nombre del jurado
            nameP: nombre del participante al que se quiere sumar un voto
  Salidas:
            Mensaje de error si no existe el jurado
            Mensaje informando de que se pudo realizar el voto.
  PostCD: Si el participante no existe para ese jurado contabiliza el voto como nulo y mostrará un mensaje de error.
  */
void Vote(tListJ *J, tJuryName nameJ, tParticipantName nameP) {
    // Posicion y datos del jurado
    tPosJ juryPos;
    tItemJ jury;
    // Posicion y datos del participante
    tPosP participantPos;
    tItemP participant;


    //Comprobamos si existe el jurado
    if ((isEmptyListJ(*J)) || findItemJ(nameJ, *J) == NULLJ) {
        ERROR("Vote not possible");
        return;
    }

    //Buscamos el jurado dado
    juryPos = findItemJ(nameJ, *J);
    jury = getItemJ(juryPos, *J);


    if((isEmptyListP(jury.participantList)) || findItemP(nameP, jury.participantList) == NULLP) { // Si la lista de participantes o el participante dado no existen:
        jury.nullVotes++; // Aumentamos en 1 la cantidad de votos nulos
        updateItemJ(jury, juryPos, J); // Actualizamos la lista de jurados
        printf("+ Error: Vote not possible. Participant %s not found in jury %s. NULLVOTE\n", nameP, nameJ);
    } else { // Si existe:
        //Buscamos el participante dado
        participantPos = findItemP(nameP, jury.participantList);
        participant = getItemP(participantPos, jury.participantList);

        //Sumamos el voto al participante y al numero de votos validos del jurado
        participant.numVotes++;
        jury.validVotes++;

        //Actualizamos las listas
        updateItemP(participant, participantPos, &jury.participantList);
        updateItemJ(jury, juryPos, J);

        printf("* Vote: jury %s participant %s location %s numvotes %d\n", nameJ, nameP, tEUParticipant2string(participant.EuParticipant), participant.numVotes);
    }
}


/*
 Objetivo: Descalificar a un participante eliminandolo de todas las listas de jurados
 Entradas:
            *J: Lista a manipular
            nameP: Nombre del participante que queremos descalificar
 Salidas:
            Mensaje de error si la lista del jurado esta vacia o no existe el participante
            Mensaje informando que el participante se pudo descalificar
 PreCD: Los jurados se separaran entre si mediante una linea en blanco
 */
void Disqualify(tListJ *J, tParticipantName nameP) {

    //Comprobamos si la lista de jurados esta vacia
    if(isEmptyListJ(*J)) {
        ERROR("Disqualify not possible");
        return;
    }

    //Variables necesarias del jurado
    tPosJ juryPos;
    tItemJ jury;

    //Variables necesarias del participante
    tPosP participantPos;
    tItemP participant;

    //Recorremos la lista de jurados buscando el participante a eliminar en cada uno
    for(juryPos= firstJ(*J); juryPos != NULLJ; juryPos = nextJ(juryPos, *J)) {
        jury = getItemJ(juryPos, *J); //Conseguimos la informacion del jurado

        printf("Jury %s\n", jury.juryName); //Imprimimos el nombre del jurado

        //Si la lista de participantes esta vacia:
        if(isEmptyListP(jury.participantList) || findItemP(nameP, jury.participantList) == NULLP)
            printf("No participant %s\n\n", nameP);
        else { //Si no:
            participantPos = findItemP(nameP, jury.participantList); //Encontramos la posicion del participante en la lista

            //Si se encontro el participante lo eliminamos, si no se encontro no se realiza ninguna operacion, continuamos con el bucle
            if(participantPos != NULLP) {
                participant = getItemP(participantPos, jury.participantList); //Conseguimos los datos del participante

                jury.nullVotes += participant.numVotes; //Sus votos pasan a ser nulos
                jury.validVotes -= participant.numVotes; //Sus votos recibidos ya no son considerados validos
                deleteAtPositionP(participantPos, &jury.participantList); //Lo eliminamos

                //Imprimimos el mensaje de exito y actualizamos la lista de jurados
                printf("Participant %s disqualified\n\n", nameP);
                updateItemJ(jury, juryPos, J);
            }
        }
    }
}


/*
 Objetivo: Eliminar los jurados con 0 votos válidos
 Entrada:
            *J: Lista a modificar
 Salidas:
            Mensaje de error si no hay jurados que se puedan eliminar o si la lista de jurados esta vacía
            Mensaje informando de la eliminacion del jurado
            */
void Remove(tListJ *J) {

    //Variables necesarias del jurado
    tItemJ jury;
    tPosJ juryPos;

    //Booleano utilizado en caso de que no haya ningun jurado a eliminar
    bool removed = false;

    //Si la lista de jurados esta vacia
    if(isEmptyListJ(*J)) {
        ERROR("Remove not possible");
        return;
    }

    //Recorremos la lista de jurados
    for(juryPos = firstJ(*J); juryPos != NULLJ; juryPos = nextJ(juryPos, *J)) {
        jury = getItemJ(juryPos, *J); //Conseguimos la informacion del jurado

        if (jury.validVotes == 0) { //Si no tiene votos validos borramos el jurado, sino continuamos con el bucle
            printf("* Remove: jury %s\n", jury.juryName); //Imprimimos mensaje de exito

            //Recorremos la lista de participantes eliminando cada participante, liberando esa memoria ocupada
            for (tPosP participantPos = firstP(jury.participantList); !isEmptyListP(jury.participantList); deleteAtPositionP(participantPos, &jury.participantList));
            updateItemJ(jury, juryPos, J); //Una vez eliminada la lista de participantes actualizamos el jurado

            juryPos = previousJ(juryPos, *J); //Nos posicionamos una posicion antes del jurado a eliminar

            deleteAtPositionJ(nextJ(juryPos, *J), J); //Borramos su siguiente, es decir, la posicion/jurado ya preparado para ser eliminado

            removed = true; //Si se ha llegado a eliminar algun jurado el booleano pasa a ser TRUE
        }
    }

    if(!removed){ //Si no se ha llegado a eliminar a ningun jurado imprimimos:
        ERROR("Remove not possible");
    }
}


/*
 Objetivo: Mostrar estadisticas de participacion y voto
 Entrada:
            J: Lista de la que se cogeran los datos para las estadisticas
 Salidas:
            Mensaje de error si la lista de jurados esta vacia
            "No participants" si la lista de participantes de algun jurado esta vacia
            Si no se dan los casos anteriores mostrara las estadisticas
PreCD: Las estadísticas de cada jurado se separarán mediante una línea en blanco
 */
void Stats(tListJ J) {

    // Comprobamos si la lista esta vacia:
    if (isEmptyListJ(J)) {
        ERROR("Stats not possible");
    } else { // Si no esta vacia:

        // Datos y posicion del jurado
        tPosJ juryPos;
        tItemJ jury;

        // Datos y posicion del participante
        tPosP participantPos; //Posicion del participante en la lista del jurado
        tItemP participant; //Datos del participante

        // Variables necesarias para la estadistica
        unsigned int votosTotales;
        float pVotosP, pPart;

        //Recorremos la lista de jurados:
        for(juryPos = firstJ(J); juryPos != NULLJ; juryPos = nextJ(juryPos, J)) {
            jury = getItemJ(juryPos, J); // Conseguimos la informacion del jurado

            printf("Jury %s\n", jury.juryName); // Imprimimos el nombre del jurado
            votosTotales = jury.validVotes + jury.nullVotes; // Calculamos el numero de votos total, la suma de los votos validos y nulos

            if(isEmptyListP(jury.participantList)) // Si la lista de participantes del jurado esta vacia
                printf("No participants\n");
            else { // Si no lo esta:

                // Bucle para imprimir la lista de participantes
                for (participantPos = firstP(jury.participantList); participantPos != NULLP; participantPos = nextP(participantPos, jury.participantList)) {
                    participant = getItemP(participantPos, jury.participantList);

                    if(jury.validVotes == 0) // Si no hay votos validos el porcentaje es 0, evitamos dividir por 0
                        pVotosP = 0;
                    else
                        pVotosP = ((float)participant.numVotes/(float)jury.validVotes)*100; // Calculo del porcentaje de votos recibidos por el participante

                    //Escribimos los datos del usuario
                    printf("Participant %s location %s numvotes %d (%.2f%%)\n", participant.participantName,
                           tEUParticipant2string(participant.EuParticipant), participant.numVotes, pVotosP);

                }
            }
            printf("Nullvotes %d\n", jury.nullVotes); // Imprimimos el numero de votos nulos

            if(jury.totalVoters == 0) //Si no hay miembros en el jurado el porcentaje es 0, evitamos dividir por 0
                pPart = 0;
            else
                pPart = ((float)votosTotales/(float)jury.totalVoters)*100; // Calculo del porcentaje de participacion

            printf("Participation: %d votes from %d voters (%.2f%%)\n\n", votosTotales, jury.totalVoters, pPart);
        }
    }
}



//Funcion auxiliar para saber si hay empate en cada una de las categorias ("eu", "non-eu")
bool hasTie(tItemP winner, tListP L) {
    // Variables necesarias:
    tPosP p;
    tItemP participant;
    bool empate = false;

    for(p = firstP(L); p != NULLP; p = nextP(p, L)) { // Recorremos la lista de participantes
        participant = getItemP(p, L);

        // Si el participante pertenece a la categoria "eu"
        if(participant.EuParticipant == true && participant.EuParticipant == winner.EuParticipant) {
            // Si encontramos un participante distinto al ganador y con su mismo numero de votos hay empate
            if((winner.numVotes == participant.numVotes) && (strcmp(winner.participantName, participant.participantName) != 0)) {
                empate = true;
                break;
            }
        } else if (participant.EuParticipant == false && participant.EuParticipant == winner.EuParticipant){ // Idem para la categoria "non-eu"
            if((winner.numVotes == participant.numVotes) && (strcmp(winner.participantName, participant.participantName) != 0)) {
                empate = true;
                break;
            }
        }
    }
    return empate;
}

/*
 Objetivo: Muestra el ganador de cada categoria para cada jurado
 Entrada:
            J: Lista en la que se consultaran los datos
 Salidas:
            Mensaje de error si la lista de jurados esta vacia
            Mensaje indicando que no existe ganador si no hay participantes, o hay empate
            Mensaje indicando el ganador si lo hay, o solo hay un participante con 0 votos
 PreCD: Las listas de ganadores de cada jurado se separaran mediante una linea en blanco
 */
void Winners(tListJ J) {

    //Comprobamos si la lista de jurados esta vacia
    if(isEmptyListJ(J)){
        ERROR("Winners not possible");
        return;
    }

    //Variables del jurado y del participante necesarias
    tItemJ jury;
    tPosJ juryPos;
    tItemP participant;
    tPosP participantPos;

    //Recorremos la lista de jurados
    for(juryPos = firstJ(J); juryPos != NULLJ; juryPos = nextJ(juryPos, J)) {
        jury = getItemJ(juryPos, J); //Conseguimos la informacion del jurado

        printf("Jury %s\n", jury.juryName); //Imprimimos el nombre del jurado

        if(isEmptyListP(jury.participantList)) { //Si la lista de participantes esta vacia no hay ganador
            printf("Location eu: No winner\n");
            printf("Location non-eu: No winner\n\n");
            continue;
        }

        //Si no esta vacia, buscamos el ganador:

        //Variables utilizadas para saber si existen participantes en las categorias "eu" y "non-eu", inicializadas a 0
        int hasEu, hasNonEu;
        hasEu = 0;
        hasNonEu = 0;

        //Informacion del ganador de cada categoria, con sus votos inicializados a un numero negativo para asegurar su sustitucion por algun posible ganador
        tItemP ganador_Eu, ganador_NonEu;
        ganador_Eu.numVotes = -1;
        ganador_NonEu.numVotes = -1;

        //Recorremos la lista de participantes
        for(participantPos = firstP(jury.participantList); participantPos != NULLP; participantPos = nextP(participantPos, jury.participantList)) {
            participant = getItemP(participantPos, jury.participantList); //Conseguimos la informacion de cada participante

            // Si el participante pertenece a la categoria "eu" y su numero de votos es mayor que el del ganador:
            if(participant.EuParticipant){
                hasEu++; // Aumentamos el numero de participantes europeos
                if(ganador_Eu.numVotes <= participant.numVotes) {
                    // Convertimos el participante en el nuevo ganador de la categoria "eu"
                    strcpy(ganador_Eu.participantName, participant.participantName);
                    ganador_Eu.numVotes = participant.numVotes;
                    ganador_Eu.EuParticipant = participant.EuParticipant;
                }
            } else { // Si el participante pertenece a la categoria "non-eu" y su numero de votos es mayor que el del ganador:
                hasNonEu++; // Aumentamos el numero de participantes no europeos
                if(ganador_NonEu.numVotes <= participant.numVotes) {
                    // Convertimos el participante en el nuevo ganador de la categoria "non-eu"
                    strcpy(ganador_NonEu.participantName, participant.participantName);
                    ganador_NonEu.numVotes = participant.numVotes;
                    ganador_NonEu.EuParticipant = participant.EuParticipant;
                }
            }
        }

        // Si hay participantes en la categoria "eu" y no hay empate imprimimos al ganador "eu"
        if(hasEu > 0 && !hasTie(ganador_Eu, jury.participantList))
            printf("Location eu: Participant %s numvotes %d\n", ganador_Eu.participantName, ganador_Eu.numVotes);
        else // Si no se cumple alguna de las condiciones no hay ganador
            printf("Location eu: No winner\n");

        // Idem para la categoria "non-eu":
        if(hasNonEu > 0 && !hasTie(ganador_NonEu, jury.participantList))
            printf("Location non-eu: Participant %s numvotes %d\n\n", ganador_NonEu.participantName, ganador_NonEu.numVotes);
        else
            printf("Location non-eu: No winner\n\n");
    }
}



void processCommand(tListJ *J, char *commandNumber, char command, char *param1, char *param2, char *param3) {

    switch (command) {
        case 'C':
            //Comando Create
            printf("********************\n");
            printf("%s %c: jury %s totalvoters %s\n", commandNumber, command, param1, param2);
            Create(J, param1, param2);
            break;
        case 'N':
            //Comando New
            printf("********************\n");
            printf("%s %c: jury %s participant %s location %s\n", commandNumber, command, param1, param2, param3);
            New(J, param1, param2, param3);
            break;
        case 'V':
            //Comando Vote:
            printf("********************\n");
            printf("%s %c: jury %s participant %s\n", commandNumber, command, param1, param2);
            Vote(J, param1, param2);
            break;
        case 'D':
            //Comando Disqualify:
            printf("********************\n");
            printf("%s %c: participant %s\n", commandNumber, command, param1);
            Disqualify(J, param1);
            break;
        case 'S':
            //Comando Stats:
            printf("********************\n");
            printf("%s %c:\n", commandNumber, command);
            Stats(*J);
            break;
        case 'R':
            //Comando Remove:
            printf("********************\n");
            printf("%s %c:\n", commandNumber, command);
            Remove(J);
            break;
        case 'W':
            //Comando Winners:
            printf("********************\n");
            printf("%s %c:\n", commandNumber, command);
            Winners(*J);
            break;
        default:
            break;
    }
}



void readTasks(tListJ *J, char *filename) {

    FILE *f = NULL;
    char *commandNumber, *command, *param1, *param2, *param3;
    const char delimiters[] = " \n\r";
    char buffer[MAX_BUFFER];

    f = fopen(filename, "r");

    if (f != NULL) {

        while (fgets(buffer, MAX_BUFFER, f)) {
            commandNumber = strtok(buffer, delimiters);
            command = strtok(NULL, delimiters);
            param1 = strtok(NULL, delimiters);
            param2 = strtok(NULL, delimiters);
            param3 = strtok(NULL, delimiters);

            processCommand(J, commandNumber, command[0], param1, param2, param3);
        }

        fclose(f);

    } else {
        printf("Cannot open file %s.\n", filename);
    }
}



int main(int nargs, char **args) {

    char *file_name = "create.txt";

    //Creacion de la lista de jurados
    tListJ J;
    createEmptyListJ(&J);

    if (nargs > 1) {
        file_name = args[1];
    } else {
        #ifdef INPUT_FILE
        file_name = INPUT_FILE;
        #endif
    }

    readTasks(&J, file_name);

    return 0;
}
