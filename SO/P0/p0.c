/*
 * PRÁCTICA 0 - SISTEMAS OPERATIVOS (GEI - UDC)
 * AUTHOR 1: Laura Palmeiro Castro
 * LOGIN 1: l.pcastro@udc.es
 * AUTHOR 2: Susana Vilarnovo Allal
 * LOGIN 2: s.vallal@udc.es
 * GRUPO 1.3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <fcntl.h>

#include "help.h"
#include "historial.h"

#define MAX 256

// Definicion de la lista de archivos abiertos:
typedef struct {
    int df;
    char *filename;
    int mode;
    char * mode_string;
} fileList;

// Funciones:

void AnadirAFicherosAbiertos(int df, int mode, char * filename, fileList *listaFicheros);
void authors(char * tr);
void pid(char *tr);
void chdirectory(char *dir);
void printDate();
void printTime();
void historic(char *tr, char *cmd, tHist hist);
void commandRep(char *input, char *trozos[], tHist *hist, fileList listaFicheros);
void listOpen(fileList *listaFicheros);
void Cmd_open (char * tr[], fileList *listaFicheros);
void Cmd_close (char *tr[], fileList *listaFicheros);
void Cmd_dup (char * tr[], fileList *listaFicheros);
void infosys();
void help(char *cmd);
int TrocearCadena(char *cadena, char *trozos[]);


// Implementacion de las funciones:
void imprimirPrompt() {
    printf("-> ");
}

void leerEntrada(char *input) {
    fgets(input, MAX, stdin);
}

void procesarCadena(char *input, char *trozos[], tHist *hist, fileList *listaFicheros);

void procesarEntrada(char * input, char * trozos, tHist *hist, fileList *listaFicheros) {
    tItemH auxItem;
    strcpy(auxItem.cmd, input);

    insertCommand(auxItem, hist);
    procesarCadena(input, &trozos, hist, listaFicheros);
}


int main() {
    char input[MAX], trozos[MAX];

    // Creacion del historial:
    tHist hist;
    createHist(&hist);

    // Creacion de la lista de archivos:
    fileList listaFicheros[MAX];

    /*
    // Ficheros estandar:
    int df1 = 0, df2 = 1, df3 = 2;
    int modo1 = fcntl(df1, F_GETFL), modo2 = fcntl(df2, F_GETFL), modo3 = fcntl(df3, F_GETFL);
    char *nameFile1 = "entrada estandar", *nameFile2 = "salida estandar", *nameFile3 = "error estandar";

     * AnadirAFicherosAbiertos(df1, modo1, nameFile1, listaFicheros);
     * AnadirAFicherosAbiertos(df2, modo2, nameFile2, listaFicheros);
     * AnadirAFicherosAbiertos(df3, modo3, nameFile3, listaFicheros);
     */

    while(true) {
        imprimirPrompt();
        leerEntrada(input);
        procesarEntrada(input, trozos, &hist, listaFicheros);
    }
    return 0;
}


/*
 * Authors [-l|-n]: muestra por pantalla los nombres y los logins de las autoras del codigo
 */
void authors(char * tr) {
    char name1[] = "Laura Palmeiro Castro";
    char login1[] = "l.pcastro@udc.es";
    char name2[] = "Susana Vilarnovo Allal";
    char login2[] = "s.vallal@udc.es";

    if(tr == 0)
        printf("%s: %s\n%s: %s\n", name1, login1, name2, login2);
    else if (!strcmp(tr, "-l"))
        printf("%s\n%s\n", login1, login2);
    else if(!strcmp(tr, "-n"))
        printf("%s\n%s\n", name1, name2);
}


/*
 * pid [-p]: muestra por pantalla el pid
 */
void pid(char * tr) {

    if(tr == 0) // Conseguir el pid
        printf("Pid de shell: %d\n", getpid());
    else if(!strcmp(tr, "-p")) // Conseguir el pid del padre:
        printf("Pid del padre del shell: %d\n", getppid());
}


/*
 * chdir [dir]: cambia el directorio del shell a dir, o muestra por pantalla el directorio actual
 */
void chdirectory(char * dir) {
    char cwd[MAX];

    if(dir == 0) {
        if (getcwd(cwd, sizeof cwd) == NULL)
            perror("Imposible mostrar directorio.\n");
        else
            printf("%s\n", cwd);
    } else {
        if (chdir(dir) != 0)
            perror("Imposible cambiar directorio.\n");
    }
}


/*
 * date: muestra por pantalla la fecha actual con el formato: DD/MM/YYYY
 */
void printDate() {
    time_t t = time(NULL);
    struct tm actual_date = *localtime(&t);
    char fecha[MAX];
    char *formato = "%d/%m/%Y\n";

    strftime(fecha, sizeof fecha, formato, &actual_date);
    printf("%s", fecha);
}


/*
 * time: muestra por pantalla la hora actual con el formato: hh:mm:ss
 */
void printTime() {
    time_t t = time(NULL);
    struct tm actual_time = *localtime(&t);
    char time[MAX];
    char *formato = "%H:%M:%S\n";

    strftime(time, sizeof time, formato, &actual_time);
    printf("%s", time);
}


/*
 * hist [-c|-N] muestra por pantalla el historial
 * hist -c: vacía el historial
 * hist -N: muestra por pantalla los primeros N comandos
 */
void historic(char * tr, char *cmd, tHist hist) {
    tPosH p;
    tItemH itemH;

    if (tr == 0) {
        p = first(hist);

        for(int i = 0; p != HNULL; i++, p = p->next) {
            itemH = getItem(p, hist);
            printf("%d->%s\n", i, p->data.cmd);
        }

    } else if(!strcmp(tr, "-c"))
        clearHist(&hist);
    else {
        int n = atoi(cmd);
        p = first(hist);

        for(int i = 0; p != HNULL && i < n; i++, p = p->next)
            printf("%d->%s\n", i, p->data.cmd);
    }
}

/*
 * command N: repite el comando N
 */
void commandRep(char *input, char *trozos[], tHist *hist, fileList listaFicheros) {
    int n = atoi(trozos[1]);

    if(n == 0 || isEmptyHist(*hist))
        printf("Comando no valido\n");
    else {
        tPosH p = first(*hist);
        tItemH item;
        char auxEntry[MAXENTRY];

        for(int i = 0; i <= n; i++) {
            if(p->next != HNULL)
                p = p->next;
            else {
                printf("No hay elemento %d en el historico\n", n);
                p = HNULL;
                break;
            }
        }

        if(p != HNULL) {
            item = getItem(p, *hist);
            printf("Ejecutando hist (%d): %s", n, item.cmd);

            strcpy(auxEntry, item.cmd);
            procesarCadena(auxEntry, trozos, hist, &listaFicheros);
        }
    }
}


// Funcion auxiliar para ficheros abiertos:
void AnadirAFicherosAbiertos(int df, int mode, char * filename, fileList *listaFicheros) {
    int i;
    int flags = fcntl(df, F_GETFL);
    long max_df = sysconf(_SC_OPEN_MAX);

    fileList newFile;
    newFile.df = df;
    strcpy(newFile.filename, filename);
    newFile.mode = mode;

    if(flags != -1) {
        if(flags & O_CREAT)
            strcpy(newFile.mode_string, "O_CREAT");
        else if(flags & O_EXCL)
            strcpy(newFile.mode_string, "O_EXCL");
        else if(flags & O_RDONLY)
            strcpy(newFile.mode_string, "O_RDONLY");
        else if(flags & O_WRONLY)
            strcpy(newFile.mode_string, "O_WRONLY");
        else if(flags & O_RDWR)
            strcpy(newFile.mode_string, "O_RDWR");
        else if(flags & O_APPEND)
            strcpy(newFile.mode_string, "O_APPEND");
        else if(flags & O_TRUNC)
            strcpy(newFile.mode_string, "O_TRUNC");
    }

    for(i = 0; i <= max_df; i++);
    listaFicheros[i] = newFile;
}

/*
 * listopen: muestra por pantalla una lista de los archivos abiertos con sus descriptores y su modo
 */
void listOpen(fileList *listaFicheros) {
    long max_df = sysconf(_SC_OPEN_MAX);

    for(int i = 0; i <= max_df; i++) {
        int flags = fcntl(listaFicheros[i].df, F_GETFL);

        if(flags != -1) {
            if(flags & O_CREAT)
                strcpy(listaFicheros[i].mode_string, "O_CREAT");
            else if(flags & O_EXCL)
                strcpy(listaFicheros[i].mode_string, "O_EXCL");
            else if(flags & O_RDONLY)
                strcpy(listaFicheros[i].mode_string, "O_RDONLY");
            else if(flags & O_WRONLY)
                strcpy(listaFicheros[i].mode_string, "O_WRONLY");
            else if(flags & O_RDWR)
                strcpy(listaFicheros[i].mode_string, "O_RDWR");
            else if(flags & O_APPEND)
                strcpy(listaFicheros[i].mode_string, "O_APPEND");
            else if(flags & O_TRUNC)
                strcpy(listaFicheros[i].mode_string, "O_TRUNC");
        }

        printf("descriptor: %d -> %s %s", listaFicheros[i].df, listaFicheros[i].filename, listaFicheros[i].mode_string);
    }
}


/*
 * open [file] mode: abre el archivo mandado por parametro (con el modo especificado) y lo anade a la lista de archivos abiertos
 */
void Cmd_open (char * tr[], fileList *listaFicheros) {
    int i, df, mode = 0;

    if (tr[0] == NULL) { /*no hay parametro*/
        //..............ListarFicherosAbiertos...............
        listOpen(listaFicheros);
        return;
    }
    for (i = 1; tr[i] != NULL; i++)
        if (!strcmp(tr[i], "cr")) mode |= O_CREAT;
        else if (!strcmp(tr[i], "ex")) mode |= O_EXCL;
        else if (!strcmp(tr[i], "ro")) mode |= O_RDONLY;
        else if (!strcmp(tr[i], "wo")) mode |= O_WRONLY;
        else if (!strcmp(tr[i], "rw")) mode |= O_RDWR;
        else if (!strcmp(tr[i], "ap")) mode |= O_APPEND;
        else if (!strcmp(tr[i], "tr")) mode |= O_TRUNC;
        else break;

    if ((df = open(tr[0], mode, 0777)) == -1)
        perror("Imposible abrir fichero\n");
    else {
        //...........AnadirAFicherosAbiertos (descriptor...modo...nombre....)....
        AnadirAFicherosAbiertos(df, mode, tr[0], listaFicheros);
        printf("Anadida entrada %d a la tabla ficheros abiertos\n", df);
    }

}


/*
 * close [df]: cierra el archivo asociado al descriptor mandado como parametro
 */
void Cmd_close (char *tr[], fileList *listaFicheros) {
    int df;
    long max_df = sysconf(_SC_OPEN_MAX);

    if (tr[0]==NULL || (df=atoi(tr[0]))<0) { /*no hay parametro*/
      //..............ListarFicherosAbiertos............... /*o el descriptor es menor que 0*/
      listOpen(listaFicheros);
      return;
    }

    if (close(df)==-1)
        perror("Imposible cerrar descriptor\n");
    else {
        //........EliminarDeFicherosAbiertos......
        for(int i = df; i <= max_df; i++)
            listaFicheros[i] = listaFicheros[i + 1];
    }
}


/*
 * dup [df]: duplica el archivo asociado al descriptor mandado como parametro
 */
void Cmd_dup (char * tr[], fileList *listaFicheros) {
    int df, duplicado;
    char aux[MAXENTRY],*p;

    if (tr[0]==NULL || (df=atoi(tr[0]))<0) { /*no hay parametro*/
        listOpen(listaFicheros);                 /*o el descriptor es menor que 0*/
        return;
    }


    p = tr[0]; //.....NombreFicheroDescriptor(df).......;
    if(p == NULL) {
        perror("Imposible duplicar descriptor\n");
        return;
    }

    duplicado = dup(df);

    if(duplicado == -1) {
        perror("Imposible duplicar descriptor\n");
        return;
    }

    sprintf (aux,"dup %d (%s)",df, p);
    //.......AnadirAFicherosAbiertos......duplicado......aux.....fcntl(duplicado,F_GETFL).....;
    AnadirAFicherosAbiertos(duplicado, fcntl(duplicado, F_GETFL), aux, listaFicheros);
}


/*
 * infosys: muestra por pantalla la informacion del equipo ejecutando el shell
 */
void infosys() {
    struct utsname info;

    if(uname(&info) == 0)
        printf("%s %s %s %s %s\n", info.nodename, info.machine, info.sysname, info.release, info.version);
    else
        perror("Imposible mostrar información\n");
}


/*
 * help [cmd]: muestra por pantalla una lista de los posibles comando a ejecutar y una breve descripcion de su uso
 */
void help(char *cmd) {
    if(strcmp(cmd, "authors") == 0)
        printf("%s\n", HELP_AUTHORS);
    else if(strcmp(cmd, "pid") == 0)
        printf("%s\n", HELP_PID);
    else if(strcmp(cmd, "chdir") == 0)
        printf("%s\n", HELP_CHDIR);
    else if(strcmp(cmd, "date") == 0)
        printf("%s\n", HELP_DATE);
    else if(strcmp(cmd, "time") == 0)
        printf("%s\n", HELP_TIME);
    else if(strcmp(cmd, "hist") == 0)
        printf("%s\n", HELP_HIST);
    else if(strcmp(cmd, "command") == 0)
        printf("%s\n", HELP_COMMAND);
    else if(strcmp(cmd, "open") == 0)
        printf("%s\n", HELP_OPEN);
    else if(strcmp(cmd, "close") == 0)
        printf("%s\n", HELP_CLOSE);
    else if(strcmp(cmd, "dup") == 0)
        printf("%s\n", HELP_DUP);
    else if(strcmp(cmd, "listopen") == 0)
        printf("%s\n", HELP_LISTOPEN);
    else if(strcmp(cmd, "infosys") == 0)
        printf("%s\n", HELP_INFOSYS);
    else if(strcmp(cmd, "help") == 0)
        printf("%s\n", HELP_HELP);
    else if(strcmp(cmd, "quit") == 0 || strcmp(cmd, "bye") == 0 || strcmp(cmd, "exit") == 0)
        printf("%s\n", HELP_EXIT);
}


int TrocearCadena(char *cadena, char *trozos[]) {
    int i = 1;

    if ((trozos[0] = strtok(cadena, "\n\t")) == NULL)
        return 0;
    while ((trozos[i] = strtok(NULL, "\n\t")) != NULL)
        i++;
    return i;
}


int option(char * input[]) {
    if(strcmp(input[0], "authors") == 0)
        return 0;
    else if (strcmp(input[0], "pid") == 0)
        return 1;
    else if (strcmp(input[0], "chdir") == 0)
        return 2;
    else if (strcmp(input[0], "date") == 0)
        return 3;
    else if (strcmp(input[0], "time") == 0)
        return 4;
    else if (strcmp(input[0], "hist") == 0)
        return 5;
    else if (strcmp(input[0], "command") == 0)
        return 6;
    else if (strcmp(input[0], "listopen") == 0)
        return 7;
    else if (strcmp(input[0], "open") == 0)
        return 8;
    else if (strcmp(input[0], "close") == 0)
        return 9;
    else if (strcmp(input[0], "dup") == 0)
        return 10;
    else if (strcmp(input[0], "infosys") == 0)
        return 11;
    else if (strcmp(input[0], "help") == 0)
        return 12;
    else if (strcmp(input[0], "quit") == 0 || strcmp(input[0], "exit") == 0 || strcmp(input[0], "bye") == 0)
        return 13;
    return -1;
}


void procesarCadena(char *input, char *trozos[], tHist *hist, fileList *listaFicheros) {
    int cnt = TrocearCadena(input, trozos);

    switch (option(&trozos[0])) {
        case 0: // authors
            if(cnt == 1)
                authors(0);
            else
                authors(trozos[1]);
            break;
        case 1: // pid
            if(cnt == 1)
                pid(0);
            else
                pid(trozos[1]);
            break;
        case 2: // chdir
            if(cnt == 1)
                chdirectory(0);
            else
                chdirectory(trozos[1]);
            break;
        case 3: // Date
            printDate();
            break;
        case 4: // Time
            printTime();
            break;
        case 5: // Historic
            if(cnt == 1)
                historic(0, trozos[1], *hist);
            else if(cnt == 2)
                historic(trozos[1], trozos[1], *hist);
            break;
        case 6: // Command
            if(cnt == 1)
                procesarCadena("hist", trozos, hist, listaFicheros);
            else if(cnt == 2)
                commandRep(input, trozos, hist, *listaFicheros);
            break;
        case 7: // Lista de ficheros abiertos
            listOpen(listaFicheros);
            break;
        case 8: // Open
            Cmd_open(trozos, listaFicheros);
            break;
        case 9: // Close
            Cmd_close(trozos, listaFicheros);
            break;
        case 10: // Dup
            Cmd_dup(trozos, listaFicheros);
            break;
        case 11: // infosys
            infosys();
            break;
        case 12: // help
            if(cnt == 1)
                printf("%s\n", HELP_TEXT);
            else
                help(trozos[1]);
            break;
        case 13: // salida
            exit(0);
            break;
        default:
            printf("Comando no reconocido, vuelva a intentarlo\n");
    }
}
