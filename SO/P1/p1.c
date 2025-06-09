/*
 * P1 - SISTEMAS OPERATIVOS (UDC)
 * Alumno 1: Susana Vilarnovo Allal (s.vallal@udc.es)
 * Alumno 2: Laura Palmeiro Castro (l.pcastro@udc.es)
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

#include "help.h"
#include "historic.h"

// Definicion de tipos:

#define MAXDATA 4096
#define MAX 1024
#define MAXTROZOS 256
// Tipos para open, close, dup y listopen:
#define O_CREAT 0100
#define O_EXCL 0200
#define O_TRUNC 01000

// Struct para la lista de ficheros
typedef struct fileList {
    int df;
    int modo;
    char * nameFile;
} fileList;

// Struct para las opciones de stat y list
typedef struct optionBool {
    bool longOption;
    bool linkOption;
    bool accOption;
    bool hidOption;
    bool recaOption;
    bool recbOption;
} optionBool;


// Cabeceras de las funciones:
void imprimirPrompt();
void leerEntrada(char * input);
void procesarEntrada(char * input, char * tr[], tList *hist, fileList *listaFicheros);
void procesarCadena(char * input, char * tr[], tList *hist, fileList *listaFicheros);
int TrocearCadena(char * cadena, char * trozos[]);
int option(char * tr[]);

// Cabeceras de los comandos:

//Funciones P0:
void Cmd_authors(char * tr[]);
void Cmd_pid(char * tr[]);
void Cmd_chdir(char * tr[]);
void Cmd_date();
void Cmd_time();
void Cmd_hist(char * tr[], tList *hist);
void Cmd_command(char * tr[], tList *hist, fileList *listaFicheros);
void insert_file(int df, int modo, char *nameFile, fileList *listaFicheros);
void remove_file(int df, fileList *listaFicheros);
void Cmd_listopen(char * tr[], fileList *listaFicheros);
void Cmd_open(char * tr[], fileList *listaFicheros);
void Cmd_close(char * tr[], fileList *listaFicheros);
void Cmd_dup(char * tr[], fileList *listaFicheros);
void Cmd_infosys();
void Cmd_help(char * tr[]);

// Funciones P1:
void Cmd_create(char * tr[]);
void Cmd_stat(char * tr[]);
void listDir(char * path, struct stat st, struct optionBool opciones);
void Cmd_list(char * tr[]);
void Cmd_delete(char *tr[]);
void Cmd_deltree(char *tr[]);


// Implementacion funciones P0:


/*
 * authors [-l|-n]      Muestra los nombres y/o logins de los autores
 */
void Cmd_authors(char * tr[]) {
    if(tr[1] == NULL) {
        printf("Laura Palmeiro Castro: l.pcastro@udc.es\n");
        printf("Susana Vilarnovo Allal: s.vallal@udc.es\n");
    } else if(!strcmp(tr[1], "-l")) {
        printf("l.pcastro@udc.es\n");
        printf("s.vallal@udc.es\n");
    } else if(!strcmp(tr[1], "-n")) {
        printf("Laura Palmeiro Castro\n");
        printf("Susana Vilarnovo Allal\n");
    }
}


/*
 * pid [-p]     Muestra el pid del shell o de su proceso padre
 */
void Cmd_pid(char * tr[]) {
    if(tr[1] == NULL)
        printf("Pid de shell: %d\n", getpid());
    else if(!strcmp(tr[1], "-p"))
        printf("Pid del padre del shell: %d\n", getppid());
}


/*
 * chdir [dir]      Cambia (o muestra) el directorio actual del shell
 */
void Cmd_chdir(char * tr[]) {
    char cwd[MAX];

    if(tr[1] == NULL) {
        if(getcwd(cwd, sizeof cwd) == NULL)
            perror("Imposible mostrar directorio\n");
        else
            printf("%s\n", cwd);
    } else {
        if(chdir(tr[1]) != 0)
            perror("Imposible cambiar directorio\n");
    }
}


/*
 * date     Muestra la fecha actual
 */
void Cmd_date() {
    time_t t = time(NULL);
    struct tm actual_date = *localtime(&t);
    char date[MAX];
    char * formato = "%d/%m/%Y\n";

    strftime(date, sizeof date, formato, &actual_date);
    printf("%s", date);
}


/*
 * time     Muestra la hora actual
 */
void Cmd_time() {
    time_t t = time(NULL);
    struct tm actual_time = *localtime(&t);
    char time[MAX];
    char * formato = "%H:%M:%S\n";

    strftime(time, sizeof time, formato, &actual_time);
    printf("%s", time);
}


/*
 * hist [-c|-N]     Muestra (o borra) el historico de comandos
 */
void Cmd_hist(char * tr[], tList *hist) {
    if(tr[1] == NULL) {
        for(int i = 0; i <= last(*hist); i++)
            printf("%d->%s", i, getItem(i, *hist));
    } else if(!strcmp(tr[1], "-c")) {
        deleteList(hist);
    } else {
        long int p = strtol(&tr[1][1], NULL, 10);

        if(p <= last(*hist)) {
            for(int i = 0; i < p && i <= last(*hist); i++)
                printf("%d->%s", i, getItem(i, *hist));
        } else
            printf("No hay elemento %ld en el historico\n", p);
    }
}


/*
 * command [-N]     Repite el comando N (del historico)
 */
void Cmd_command(char * tr[], tList *hist, fileList *listaFicheros) {
    if(tr[1] == NULL) {
        for(int i = 0; i <= last(*hist); i++)
            printf("%d->%s", i, getItem(i, *hist));
    } else {
        char aux[MAX];
        long int p = strtol(tr[1], NULL, 10);

        if(p >= 0 && p < last(*hist)) {
            strcpy(aux, getItem((int)p, *hist));
            char comando[MAX];
            strcpy(comando, aux);
            TrocearCadena(aux, tr);

            if(strcmp(tr[0], "command") != 0) {
                printf("Ejecutando hist (%ld): %s\n", p, getItem((int)p, *hist));
                procesarCadena(comando, tr, hist, listaFicheros);
            }
        }
    }
}

// Funciones auxiliares para listopen, open, close y dup:
void insert_file(int df, int modo, char *nameFile, fileList *listaFicheros) {
    int i;

    for(i = 0; i < MAXDATA && listaFicheros[i].nameFile != NULL; i++);
    if(i < MAXDATA && listaFicheros[i].nameFile == NULL) {
        listaFicheros[i].df = df;

        if(modo == O_CREAT)
            listaFicheros[i].modo = O_CREAT;
        else if(modo == O_EXCL)
            listaFicheros[i].modo = O_EXCL;
        else if(modo == O_TRUNC)
            listaFicheros[i].modo = O_TRUNC;
        else
            listaFicheros[i].modo = fcntl(listaFicheros[i].df, F_SETFL, modo);

        listaFicheros[i].nameFile = malloc(strlen(nameFile) + 1);
        strcpy(listaFicheros[i].nameFile, nameFile);

        return;
    } else {
        perror("No se pudo insertar en la lista");
    }
}

void remove_file(int df, fileList *listaFicheros) {
    int i, j;

    for(i = 0; i < MAXDATA && listaFicheros[i].df != df; i++);
    if(i < MAXDATA && listaFicheros[i].df == df) {
        free(listaFicheros[i].nameFile);
        for(j = i; j < MAXDATA; j++) {
            listaFicheros[j] = listaFicheros[j + 1];
        }
    }
}


/*
 * listopen [n]     Lista los ficheros abiertos (al menos n) del shell
 */
void Cmd_listopen(char *tr[], fileList *listaFicheros) {
    int i, modo;
    long int p;

    if(tr[1] == NULL) {
        p = MAXDATA;
    } else
        p = strtol(&tr[1][0], NULL, 10);

    for(i = 0; i < MAXDATA && i < p && listaFicheros[i].nameFile != NULL; i++) {

        modo = listaFicheros[i].modo;

        if(modo == O_CREAT)
            listaFicheros[i].modo = O_CREAT;
        else if(modo == O_EXCL)
            listaFicheros[i].modo = O_EXCL;
        else if(modo == O_TRUNC)
            listaFicheros[i].modo = O_TRUNC;
        else
            modo = fcntl(listaFicheros[i].df, F_GETFL);


        printf("Descriptor: %d -> %s ", listaFicheros[i].df, listaFicheros[i].nameFile);

        if(modo != -1) {
            if(modo & O_CREAT)
                printf("O_CREAT\n");
            else if(modo & O_EXCL)
                printf("O_EXCL\n");
            else if(modo & O_WRONLY)
                printf("O_WRONLY\n");
            else if(modo & O_RDWR)
                printf("O_RDWR\n");
            else if(modo & O_APPEND)
                printf("O_APPEND\n");
            else if(modo & O_TRUNC)
                printf("O_TRUNC\n");
            else //if (modo & O_RDONLY)
                printf("O_RDONLY\n");
        } else {
            continue;
        }
    }
}


/*
 * open fich m1 m2...       Abre el fichero fich. y lo anade a la lista de ficheros abiertos del shell
 */
void Cmd_open(char * tr[], fileList *listaFicheros) {
    int i, df, mode = 0;

    if(tr[1] == NULL) { // No hay parametro
        Cmd_listopen(tr, listaFicheros);
        return;
    }

    for (i=2; tr[i]!=NULL; i++)
        if (!strcmp(tr[i],"cr")) mode |= O_CREAT;
        else if (!strcmp(tr[i],"ex")) mode |= O_EXCL;
        else if (!strcmp(tr[i],"ro")) mode |= O_RDONLY;
        else if (!strcmp(tr[i],"wo")) mode |= O_WRONLY;
        else if (!strcmp(tr[i],"rw")) mode |= O_RDWR;
        else if (!strcmp(tr[i],"ap")) mode |= O_APPEND;
        else if (!strcmp(tr[i],"tr")) mode |= O_TRUNC;
        else break;

    if((df = open(tr[1], mode, 0777)) == -1)
        perror("Imposible abrir fichero");
    else {
        insert_file(df, mode, tr[1], listaFicheros);
        printf("Anadida entrada a la tabla de ficheros abiertos\n");
    }
}


/*
 * close df     Cierra el descriptor df y elimina el correspondiente fichero de la lista de ficheros abiertos
 */
void Cmd_close(char * tr[], fileList *listaFicheros) {
    int df;

    if(tr[1] == NULL || (df = atoi(tr[1])) < 0) { // No hay parametro o el descriptor es menor que 0
        Cmd_listopen(tr, listaFicheros);
        return;
    }

    if(close(df) == -1)
        perror("Imposible cerrar descriptor");
    else
        remove_file(df, listaFicheros);
}


/*
 * dup df       Duplica el descriptor de fichero df y anade una nueva entrada a la lista de ficheros abiertos
 */
void Cmd_dup(char * tr[], fileList *listaFicheros) {
    int i, modo;
    int df, duplicado;
    char aux[MAX], *p;

    if(tr[1] == NULL || (df = atoi(tr[1])) < 0) { // No hay parametro o el descriptor es menor que 0
        Cmd_listopen(tr, listaFicheros);
        return;
    }

    for(i = 0; i < MAXDATA && listaFicheros[i].df != df; i++);
    p = listaFicheros[i].nameFile;
    modo = listaFicheros[i].modo;

    if(modo == O_CREAT)
        listaFicheros[i].modo = O_CREAT;
    else if(modo == O_EXCL)
        listaFicheros[i].modo = O_EXCL;
    else if(modo == O_TRUNC)
        listaFicheros[i].modo = O_TRUNC;
    else
        modo = fcntl(listaFicheros[i].df, F_GETFL);

    duplicado = dup(df);
    if(p == NULL || duplicado == -1)
        perror("Imposible duplicar descriptor");
    else {
        sprintf(aux, "dup %d (%s)\n", df, p);
        insert_file(duplicado, modo, aux, listaFicheros);
    }
}


/*
 * infosys      Muestra informacion de la maquina donde corre el shell
 */
void Cmd_infosys() {
    struct utsname info;

    if(uname(&info) == 0)
        printf("%s %s %s %s %s\n", info.nodename, info.machine, info.sysname, info.release, info.version);
    else
        perror("Imposible mostrar informacion");
}


/*
 * help [cmd|-lt|-T]        Muestra ayuda sobre los comandos
 */
void Cmd_help(char * tr[]) {
    if(tr[1] == NULL)
        printf("%s\n", HELP_TEXT);
    else if(!strcmp(tr[1], "authors"))
        printf("%s\n", HELP_AUTHORS);
    else if(!strcmp(tr[1], "pid"))
        printf("%s\n", HELP_PID);
    else if(!strcmp(tr[1], "chdir"))
        printf("%s\n", HELP_CHDIR);
    else if(!strcmp(tr[1], "date"))
        printf("%s\n", HELP_DATE);
    else if(!strcmp(tr[1], "time"))
        printf("%s\n", HELP_TIME);
    else if(!strcmp(tr[1], "hist"))
        printf("%s\n", HELP_HIST);
    else if(!strcmp(tr[1], "command"))
        printf("%s\n", HELP_COMMAND);
    else if(!strcmp(tr[1], "open"))
        printf("%s\n", HELP_OPEN);
    else if(!strcmp(tr[1], "close"))
        printf("%s\n", HELP_CLOSE);
    else if(!strcmp(tr[1], "dup"))
        printf("%s\n", HELP_DUP);
    else if(!strcmp(tr[1], "listopen"))
        printf("%s\n", HELP_LISTOPEN);
    else if(!strcmp(tr[1], "infosys"))
        printf("%s\n", HELP_INFOSYS);
    else if(!strcmp(tr[1], "create"))
        printf("%s\n", HELP_CREATE);
    else if(!strcmp(tr[1], "stat"))
        printf("%s\n", HELP_STAT);
    else if(!strcmp(tr[1], "list"))
        printf("%s\n", HELP_LIST);
    else if(!strcmp(tr[1], "delete"))
        printf("%s\n", HELP_DELETE);
    else if(!strcmp(tr[1], "deltree"))
        printf("%s\n", HELP_DELTREE);
    else if(!strcmp(tr[1], "help"))
        printf("%s\n", HELP_HELP);
    else if(!strcmp(tr[1], "quit") || !strcmp(tr[1], "bye") || !strcmp(tr[1], "exit"))
        printf("%s\n", HELP_EXIT);
}


// Implementacion funciones P1:

char LetraTF (mode_t m) {
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}
/*las siguientes funciones devuelven los permisos de un fichero en formato rwx----*/
/*a partir del campo st_mode de la estructura stat */
/*las tres son correctas pero usan distintas estrategias de asignaciÃ³n de memoria*/

char * ConvierteModo (mode_t m, char *permisos) {
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}


char * ConvierteModo2 (mode_t m) {
    static char permisos[12];
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}

char * ConvierteModo3 (mode_t m) {
    char *permisos;

    if ((permisos=(char *) malloc (12))==NULL)
        return NULL;
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}


/*
 * create [-f][name]        Crea un directorio o un fichero (-f)
 */
void Cmd_create(char * tr[]) {
    int df;

    if (tr[1] == NULL)
        Cmd_chdir(tr);
    else if(!strcmp(tr[1], "-f")) {
        df = open(tr[2], O_CREAT | O_EXCL, 0664);
        if(df == -1)
            perror("Imposible crear fichero");
        else
            printf("Fichero %s creado\n", tr[2]);
        close(df);
    } else {
        df = mkdir(tr[1], 0775);
        if(df == -1)
            perror("Imposible crear directorio");
        else
            printf("Directorio %s creado\n", tr[1]);

    }
}


// Funcion auxiliar para stat y list:
void printStatInfo(const char * name, const struct stat *st, struct optionBool opciones) {
    struct passwd *userInfo;
    struct group *groupInfo;
    char linkName[100]="", userName[20], groupName[20], time[50], permisos[20];

    if(opciones.longOption) {
        if(opciones.accOption)
            strftime(time, sizeof(time), "%Y/%m/%d-%H:%M", localtime(&st->st_atime));
        else
            strftime(time, sizeof(time), "%Y/%m/%d-%H:%M", localtime(&st->st_mtime));
        printf("%s", time);

        if((userInfo = getpwuid(st->st_uid)) != NULL)
            sprintf(userName, "%s", userInfo->pw_name);
        else
            sprintf(userName, "%d", st->st_uid);

        if((groupInfo = getgrgid(st->st_gid)) != NULL)
            sprintf(groupName, "%s", groupInfo->gr_name);
        else
            sprintf(groupName, "%d", st->st_gid);

        printf("%2ld (%8ld) %s %s %s", (long)st->st_nlink, (long)st->st_ino, userName, groupName, ConvierteModo(st->st_mode, permisos));
    }

    printf("%9ld %s", st->st_size, name);

    if(opciones.linkOption && opciones.longOption && S_ISLNK(st->st_mode)) {

        if((readlink(name, linkName, sizeof(linkName) - 1)) != -1)
            printf("->%s", linkName);
        else
            perror("Imposible acceder al link");
    }

    printf("\n");
}


/*
 * stat [-long][-link][-acc] name1 name2 ...        lista ficheros;
 */
void Cmd_stat(char * tr[]) {
    struct optionBool opciones;

    opciones.longOption = false;
    opciones.linkOption = false;
    opciones.accOption = false;

    if (tr[1] == NULL) {
        Cmd_chdir(tr);
        return;
    }

    for (int i = 1; tr[i] != NULL; i++) {
        if (tr[i][0] == '-') {
            // Opcion detectada
            if (tr[i][2] == 'o')
                opciones.longOption = true;
            else if (tr[i][1] == 'a')
                opciones.accOption = true;
            else if (tr[i][2] == 'i')
                opciones.linkOption = true;
            else {
                fprintf(stderr, "Opcion no valida: %s\n", tr[i]);
                return;
            }
        } else {
            struct stat st;

            if (stat(tr[i], &st) == 0)
                printStatInfo(tr[i], &st, opciones);
            else
                perror("Imposible acceder al fichero o directorio");
        }
    }
}


// Funcion auxiliar para saber si un path es un directorio:
bool isDir(const char * path) {
    struct stat stado;

    if(lstat(path, &stado) != -1) {
        if((stado.st_mode & S_IFMT) == S_IFDIR)
            return true;
        else
            return false;
    } else
        return false;
}

// Funciones auxiliares para Cmd_list:

void listRec(char * path, struct stat st, struct optionBool opciones) {
    DIR *d2;
    struct dirent * ent2;
    char new_path[MAX];
    struct stat directorio;

    d2 = opendir(path);

    if (d2 != NULL) {
        if(chdir(path) == 0) {
            chdir("..");
            while((ent2 = readdir(d2)) != NULL) {

                sprintf(new_path, "%s/%s", path, ent2->d_name);

                if(isDir(new_path) && strcmp(ent2->d_name, ".") != 0 && strcmp(ent2->d_name, "..") != 0) {

                    stat(ent2->d_name, &directorio);

                    listDir(new_path, directorio, opciones);
                    chdir("..");
                }
            }

            closedir(d2);
        } else
            perror("No se pudo acceder al directorio");
    } else {
        perror("No se pudo abrir el subdirectorio");
        return;
    }
}


void listDir(char * path, struct stat st, struct optionBool opciones) {
    DIR *d;
    struct dirent *ent;
    char new_path[MAX];
    struct stat directorio;

    d = opendir(path);

    if (d == NULL) {
        perror("No se pudo abrir el directorio");
        return;
    } else {

        if (opciones.recbOption)
            listRec(path, st, opciones);

        printf("************%s\n", path);

        while ((ent = readdir(d)) != NULL) {

            if (!opciones.hidOption) {

                if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
                    continue;

                sprintf(new_path, "%s/%s", path, ent->d_name);
                lstat(new_path, &directorio);
                printStatInfo(ent->d_name, &directorio, opciones);

            } else if (opciones.hidOption || strcmp(ent->d_name, ".") != 0 || strcmp(ent->d_name, "..") != 0) {

                sprintf(new_path, "%s/%s", path, ent->d_name);

                if (lstat(new_path, &directorio) == -1)
                    perror("No se pudo abrir el directorio");
                printStatInfo(ent->d_name, &directorio, opciones);
            }
        }

        if (opciones.recaOption) {
            listRec(path, st, opciones);
        }
    }

    closedir(d);
}


/*
 * list [-reca][-recb][-hid][-long][-acc] n1 n2 ...     lista contenidos de directorios
 */
void Cmd_list(char *tr[]) {
    int i = 1, j = 0, argumentos = 1;
    struct optionBool opciones;
    struct stat stado;

    opciones.longOption = false;
    opciones.linkOption = false;
    opciones.accOption = false;
    opciones.hidOption = false;
    opciones.recaOption = false;
    opciones.recbOption = false;

    if (tr[1] == NULL) {
        Cmd_chdir(tr);
        return;
    }

    while (tr[i] != NULL && tr[i][0] == '-') {
        // Opcion detectada
        if(!strcmp(tr[i], "-long")) {
            opciones.longOption = true;
            argumentos++;
        }
        else if(!strcmp(tr[i], "-acc")) {
            opciones.accOption = true;
            argumentos++;
        }
        else if(!strcmp(tr[i], "-link")) {
            opciones.linkOption = true;
            argumentos++;
        }
        else if(!strcmp(tr[i], "-hid")) {
            opciones.hidOption = true;
            argumentos++;
        }
        else if(!strcmp(tr[i], "-reca")) {
            opciones.recaOption = true;
            argumentos++;
        }
        else if(!strcmp(tr[i], "-recb")) {
            opciones.recbOption = true;
            argumentos++;
        } else
            break;

        i++;
    }

    while(tr[argumentos] != NULL) {
        if(stat(tr[argumentos], &stado) == -1) {
            perror("No se pudo abrir");
            return;
        }

        if(isDir(tr[argumentos]))
            listDir(tr[argumentos], stado, opciones);
        else
            printStatInfo(tr[argumentos], &stado, opciones);

        argumentos++;
        j++;
    }

    if(j == 0)
        Cmd_chdir(tr);
}


/*
 * delete [name1 name2 ...]     Borra ficheros o directorios vacios
 */
void Cmd_delete(char *tr[]) {
    int i = 1;

    if(tr[1] == NULL) {
        Cmd_chdir(tr);
        return;
    }

    while(tr[i] != NULL) {
        if(isDir(tr[i])) { // Directorio
            if(rmdir(tr[i]) == -1)
                perror("Imposible borrar directorio");
        } else if(unlink(tr[i]) == -1) // Fichero
                perror("Imposible borrar fichero");
        i++;
    }

}


/*
 * deltree [name1 name2 ...]        Borra ficheros o directorios no vacios recursivamente
 */
void Cmd_deltree(char * tr[]) {
    int i = 1;

    if(tr[1] == NULL) {
        Cmd_chdir(tr);
        return;
    }

    while(tr[i] != NULL) {
        if(isDir(tr[i])) {
            DIR *d;
            struct dirent *ent;
            char path[MAX];

            d = opendir(tr[i]);

            if(d != NULL) {
                while ((ent = readdir(d)) != NULL) {
                    if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                        sprintf(path, "%s/%s", tr[i], ent->d_name);

                        if(isDir(path))
                            Cmd_deltree((char *[]){tr[0], path, NULL});
                        else {
                            if (unlink(path) == -1)
                                perror("Imposible borrar fichero");
                        }
                    }
                }
                closedir(d);
            }

            if(rmdir(tr[i]) == -1)
                perror("Imposible borrar directorio");

        } else {
            if (unlink(tr[i]) == -1)
                perror("Imposible borrar fichero");
        }


        i++;
    }
}


// Implementacion de las funciones basicas:

void imprimirPrompt() {
    printf("-> ");
}

void leerEntrada(char * input) {
    fgets(input, MAX, stdin);
}

void procesarEntrada(char * input, char * tr[], tList *hist, fileList *listaFicheros) {
    char * aux = malloc(sizeof(input));
    strcpy(aux, input);

    insertItem(aux, hist);
    TrocearCadena(input, tr);
    procesarCadena(input, tr, hist, listaFicheros);

    free(aux);
    aux = NULL;
}

int option(char * tr[]) {
    if(!strcmp(tr[0], "authors"))
        return 0;
    else if(!strcmp(tr[0], "pid"))
        return 1;
    else if(!strcmp(tr[0], "chdir"))
        return 2;
    else if(!strcmp(tr[0], "date"))
        return 3;
    else if(!strcmp(tr[0], "time"))
        return 4;
    else if(!strcmp(tr[0], "hist"))
        return 5;
    else if(!strcmp(tr[0], "command"))
        return 6;
    else if(!strcmp(tr[0], "listopen"))
        return 7;
    else if(!strcmp(tr[0], "open"))
        return 8;
    else if(!strcmp(tr[0], "close"))
        return 9;
    else if(!strcmp(tr[0], "dup"))
        return 10;
    else if(!strcmp(tr[0], "infosys"))
        return 11;
    else if(!strcmp(tr[0], "help"))
        return 12;
    else if(!strcmp(tr[0], "quit") || !strcmp(tr[0], "exit") || !strcmp(tr[0], "bye"))
        return 13;
    else if(!strcmp(tr[0], "create"))
        return 14;
    else if(!strcmp(tr[0], "stat"))
        return 15;
    else if(!strcmp(tr[0], "list"))
        return 16;
    else if(!strcmp(tr[0], "delete"))
        return 17;
    else if(!strcmp(tr[0], "deltree"))
        return 18;
    return -1;
}

void procesarCadena(char * input, char * tr[], tList *hist, fileList *listaFicheros) {
    if(tr[0] != NULL) {

        switch (option(&tr[0])) {
            case 0:
                Cmd_authors(tr);
                break;
            case 1:
                Cmd_pid(tr);
                break;
            case 2:
                Cmd_chdir(tr);
                break;
            case 3:
                Cmd_date();
                break;
            case 4:
                Cmd_time();
                break;
            case 5:
                Cmd_hist(tr, hist);
                break;
            case 6:
                Cmd_command(tr, hist, listaFicheros);
                break;
            case 7:
                Cmd_listopen(tr, listaFicheros);
                break;
            case 8:
                Cmd_open(tr, listaFicheros);
                break;
            case 9:
                Cmd_close(tr, listaFicheros);
                break;
            case 10:
                Cmd_dup(tr, listaFicheros);
                break;
            case 11:
                Cmd_infosys();
                break;
            case 12:
                Cmd_help(tr);
                break;
            case 13:
                deleteList(hist);
                exit(0);
            case 14:
                Cmd_create(tr);
                break;
            case 15:
                Cmd_stat(tr);
                break;
            case 16:
                Cmd_list(tr);
                break;
            case 17:
                Cmd_delete(tr);
                break;
            case 18:
                Cmd_deltree(tr);
                break;
            default:
                printf("Comando no reconocido, vuelva a intentarlo.\n");
        }
    }

}

int TrocearCadena(char * cadena, char * trozos[]) {
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

int main() {
    char input[MAX];
    char * tr[MAXTROZOS];

    fileList listaFicheros[MAXDATA];
    tList hist;
    createEmptyList(&hist);

    // Ficheros estandar:
    int dfFichero1 = 0, dfFichero2 = 1, dfFichero3 = 2;
    int modo1 = O_RDWR, modo2 = O_RDWR, modo3 = O_RDWR;
    char * nameFichero1 = "entrada estandar", *nameFichero2 = "salida estandar", *nameFichero3 = "error estandar";

    insert_file(dfFichero1, modo1, nameFichero1, listaFicheros);
    insert_file(dfFichero2, modo2, nameFichero2, listaFicheros);
    insert_file(dfFichero3, modo3, nameFichero3, listaFicheros);

    while(true) {
        imprimirPrompt();
        leerEntrada(input);
        procesarEntrada(input, tr, &hist, listaFicheros);
    }
}
