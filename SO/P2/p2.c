/*
 * P2 - SISTEMAS OPERATIVOS (UDC)
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
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/wait.h>

#include <sys/mman.h>
#include <sys/shm.h>
#include <errno.h>

#include "help.h"
#include "historic.h"
#include "memlist.h"

// Definicion de tipos:

#define MAXDATA 4096
#define MAX 1024
#define MAXTROZOS 256
#define TAMANO 2048
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

// Variables globales inicializadas:
int int_globalI = 0;
char char_globalI = 'a';
double double_globalI = 1.5;

// Variables globales sin inicializar:
int int_globalN;
char char_globalN;
double double_globalN;


// Cabeceras de las funciones:
void imprimirPrompt();
void leerEntrada(char * input);
void procesarEntrada(char * input, char * tr[], tList *hist, fileList *listaFicheros, tListM * ListMem);
void procesarCadena(char * input, char * tr[], tList *hist, fileList *listaFicheros, tListM * ListMem);
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
void Cmd_command(char * tr[], tList *hist, fileList *listaFicheros, tListM * ListMem);
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

// Funciones P2:
void Cmd_malloc(char * tr[], tListM * ListMem);
void Cmd_shared(char * tr[], tListM * M);
void Cmd_mmap(char * tr[], tListM * ListMem);
void Cmd_read(char * tr[]);
void Cmd_write(char * tr[]);
void Cmd_memdump(char * tr[]);
void Cmd_memfill(char * tr[]);
void Cmd_mem(char * tr[], tListM * M);
void Cmd_recurse(int n);


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
        int i;
        for(i = 0; i <= last(*hist); i++)
            printf("%d->%s", i, getItem(i, *hist));
    } else if(!strcmp(tr[1], "-c")) {
        deleteList(hist);
    } else {
        long int p = strtol(&tr[1][1], NULL, 10);

        if(p <= last(*hist)) {
            int i;
            for(i = 0; i < p && i <= last(*hist); i++)
                printf("%d->%s", i, getItem(i, *hist));
        } else
            printf("No hay elemento %ld en el historico\n", p);
    }
}


/*
 * command [-N]     Repite el comando N (del historico)
 */
void Cmd_command(char * tr[], tList *hist, fileList *listaFicheros, tListM * ListMem) {
    if(tr[1] == NULL) {
        int i;
        for(i = 0; i <= last(*hist); i++)
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
                procesarCadena(comando, tr, hist, listaFicheros, ListMem);
            }
        }
    }
}


// Funciones auxiliares para listopen, open, close y dup:
void insert_file(int df, int modo, char *nameFile, fileList *listaFicheros) {
    int i = 0;

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
    else if(!strcmp(tr[1], "malloc"))
        printf("%s\n", HELP_MALLOC);
    else if(!strcmp(tr[1], "shared"))
        printf("%s\n", HELP_SHARED);
    else if(!strcmp(tr[1], "mmap"))
        printf("%s\n", HELP_MMAP);
    else if(!strcmp(tr[1], "read"))
        printf("%s\n", HELP_READ);
    else if(!strcmp(tr[1], "write"))
        printf("%s\n", HELP_WRITE);
    else if(!strcmp(tr[1], "memdump"))
        printf("%s\n", HELP_MEMDUMP);
    else if(!strcmp(tr[1], "memfill"))
        printf("%s\n", HELP_MEMFILL);
    else if(!strcmp(tr[1], "mem"))
        printf("%s\n", HELP_MEM);
    else if(!strcmp(tr[1], "recurse"))
        printf("%s\n", HELP_RECURSE);
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
    int i;
    struct optionBool opciones;

    opciones.longOption = false;
    opciones.linkOption = false;
    opciones.accOption = false;

    if (tr[1] == NULL) {
        Cmd_chdir(tr);
        return;
    }

    for (i = 1; tr[i] != NULL; i++) {
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


// Implementacion funciones P2:

// Funcion auxiliar para convertir un char en un puntero:
void * cadtop(char * string) {
    void *p;
    sscanf(string, "%p", &p);
    return p;
}

// FUNCIONES DE IMPRESION:
// Mostrar lista completa
void printMemoryList(tListM M) {
    int pos;
    dataMem item;

    printf("******Lista de bloques asignados para el proceso %d\n", getpid());

    if(isEmptyMem(M))
        printf("\b");
    else {
        for(pos = firstMem(M); pos <= lastMem(M); pos++) {
            item = getDataMem(pos, M);

            if(item.cmdType == MALLOC)
                printf("\t%p\t\t%zu %s malloc\n", item.dir, item.size, item.date);

            if(item.cmdType == SHARED)
                printf("\t%p\t\t%zu %s shared (key: %d)\n", item.dir, item.size, item.date, item.Union.key);

            if(item.cmdType == MMAP)
                printf("\t%p\t\t%zu %s %s  (descriptor %d)\n", item.dir, item.size, item.date, item.Union.fichero.filename, item.Union.fichero.df);
        }
    }
}

// Imprimir lista malloc
void printMalloc(tListM M) {
    int pos;
    dataMem item;

    printf("******Lista de bloques asignados para el proceso %d\n", getpid());

    if(isEmptyMem(M))
        printf("\b");
    else {
        for(pos = firstMem(M); pos <= lastMem(M); pos++) {
            item = getDataMem(pos, M);

            if(item.cmdType == MALLOC)
                printf("\t%p\t\t%zu %s malloc\n", item.dir, item.size, item.date);
        }
    }
}

// Imprimir lista shared
void printShared(tListM M) {
    int pos;
    dataMem item;

    printf("******Lista de bloques asignados para el proceso %d\n", getpid());

    if(isEmptyMem(M))
        printf("\b");
    else {
        for(pos = firstMem(M); pos <= lastMem(M); pos++) {
            item = getDataMem(pos, M);

            if(item.cmdType == SHARED)
                printf("\t%p\t\t%zu %s shared (key: %d)\n", item.dir, item.size, item.date, item.Union.key);
        }
    }
}

// Imprimir lista mmap:
void printMmap(tListM M) {
    int pos;
    dataMem item;

    printf("******Lista de bloques asignados para el proceso %d\n", getpid());

    if(isEmptyMem(M))
        printf("\b");
    else {
        for(pos = firstMem(M); pos <= lastMem(M); pos++) {
            item = getDataMem(pos, M);

            if(item.cmdType == MMAP)
                printf("\t%p\t\t%zu %s %s  (descriptor %d)\n", item.dir, item.size, item.date, item.Union.fichero.filename, item.Union.fichero.df);
        }
    }
}
// ------------


/*
 * malloc [-free] [tam]	asigna un bloque memoria de tamano tam con malloc
 */

// -free: desasigna un bloque de memoria de tamano tam asignado con malloc
void Cmd_deallocateMalloc(char * tr[], tListM * M) {
    size_t tam;
    dataMem item;
    int pos;

    if(tr[2] == NULL)
        printMalloc(*M);
    else {
        tam = strtol(tr[2], NULL, 10);

        if(tam == 0)
            printf("No se borran bloques de 0 bytes\n");
        else {
            for(pos = firstMem(*M); pos <= lastMem(*M); pos++) {
                item = getDataMem(pos, *M);

                if(item.size == tam) {
                    if(item.dir == NULL) {
                        perror("Direccion no valida de borrado.");
                        return;
                    } else {
                        printf("Borrado malloc de tamano %ld situado en %p\n", item.size, item.dir);
                        deleteItemM(pos, M);
                        free(item.dir);
                        break;
                    }
                }
            }
        }
    }
}

// Insercion de elementos malloc en la lista (malloc):
void Cmd_malloc(char * tr[], tListM *M) {
    size_t tam;
    dataMem item;
    time_t tiempo = time(0);
    struct tm * tlocal = localtime(&tiempo);

    if(tr[1] == NULL)
        printMalloc(*M);
    else if(!strcmp(tr[1], "-free"))
        Cmd_deallocateMalloc(tr, M);
    else {
        tam = strtol(tr[1], NULL, 10);

        if(tam == 0)
            printf("No se asignan bloques de 0 bytes\n");
        else {
            item.cmdType = MALLOC;
            item.size = tam;
            item.dir = malloc(tam);
            strftime(item.date, 128, "%b %d %H:%M", tlocal);

            if(item.dir == NULL) {
                perror("No se pudo conseguir la direccion.");
                return;
            } else {
                if(insertMem(item, M))
                    printf("Asignados %zu bytes en %p\n", item.size, item.dir);
                else
                    perror("Imposible hacer malloc.");
            }
        }
    }
}



/*
 * shared [-free|-create|-delkey] cl [tam]	asigna memoria compartida con clave cl en el programa
 */

// Funciones auxiliares para memoria compartida:

// Insercion de memoria compartida en la lista (shared):
void insert_memShared(void * p, size_t tam, key_t clave, tListM *M) {
    dataMem item;
    int aux;
    struct shmid_ds s;
    time_t tiempo = time(0);
    struct tm * tlocal = localtime(&tiempo);

    s.shm_segsz = tam;

    item.size = s.shm_segsz;
    item.dir = p;
    item.cmdType = SHARED;
    item.Union.key = clave;
    strftime(item.date, 128, "%b %d %H:%M", tlocal);

    if(p != NULL) {
        if(insertMem(item, M))
            printf("Asignados %lu bytes en %p\n", item.size, p);
        else
            printf("No se puede insertar el elemento en la lista de memoria compartida\n");
    }
}

// Obtencion de memoria compartida:
void * ObtenerMemoriaShmget (key_t clave, size_t tam, tListM *M) {
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;

    dataMem item;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);

    if (tam)     /*tam distito de 0 indica crear */
        flags=flags | IPC_CREAT | IPC_EXCL;

    if (clave==IPC_PRIVATE) {  /*no nos vale*/
        errno=EINVAL;
        printf("%d no es una clave valida\n", clave);
        return NULL;
    }

    if ((id=shmget(clave, tam, flags))==-1) {
        printf("Imposible asignar memoria compartida con clave %d: %s\n", clave, strerror(errno));
        return (NULL);
    }

    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        printf("No se pudo crear el segmento de memoria compartida: %s\n", strerror(errno));
        return (NULL);
    }

    if(shmctl (id,IPC_STAT,&s) != -1)
        insert_memShared(p, tam, clave, M); /** Guardar en la lista, p.e.  InsertarNodoShared (&L, p, s.shm_segsz, clave); */
    else
        printf("Imposible asignar memoria compartida con la clave %lu: %s", (unsigned long)clave, strerror(errno));

    return (p);
}

// shared -create:
void SharedCreate (char *tr[], tListM *M) {
    key_t cl;
    size_t tam = 0;
    void *p;

    if (tr[2]==NULL || tr[3]==NULL) {
        printShared(*M);
        return;
    }

    cl=(key_t) strtoul(tr[2],NULL,10);

    if(tr[3] != NULL)
        tam=(size_t) strtoul(tr[3],NULL,10);

    if (tam==0) {
        printf("No se asignan bloques de 0 bytes\n");
        return;
    }

    if ((p=ObtenerMemoriaShmget(cl,tam, M))==NULL)
        printf("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}

// shared -delkey:
void SharedDelkey (char *args[]) {
    key_t clave;
    int id;
    char *key=args[2];

    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("      delkey necesita clave_valida\n");
        return;
    }
    if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1) {
        perror ("shmctl: imposible eliminar id de memoria compartida\n");
        return;
    }
}

// shared -free
void SharedFree(char *tr[], tListM *M) {
    void *p;
    key_t cl;
    int suc, pos;
    dataMem item;

    if(tr[2] != NULL) {
        cl = (key_t)atoi(tr[2]);

        for(pos = firstMem(*M); pos <= lastMem(*M); pos++) {
            item = getDataMem(pos, *M);

            if(item.cmdType == SHARED && item.Union.key == cl) {
                p = item.dir;
                suc = shmdt(p);

                if(suc != -1) {
                    deleteItemM(pos, M);
                    break;
                } else
                    perror("shmdt: imposible eliminar memoria compartida.");
            } else
                printf("No hay bloque de esa clave mapeado en el proceso\n");
        }
    } else
        printShared(*M);
}

// shared allocate:
void Cmd_sharedAlloc(char * tr[], tListM *M) {
    void *p;
    key_t key;
    int id, aux;
    char * clave = tr[1];
    struct shmid_ds s;

    if((key =(key_t) strtoul(clave, NULL, 10)) == IPC_PRIVATE) {
        printf("      shared clave_valida\n");
        return;
    }

    if((id= shmget(key, 0, 0666)) == -1) {
        perror("shmget: imposible obtener memoria compartida.");
        return;
    }

    if((p= shmat(id, NULL, 0)) !=(void *) -1) {
        shmctl(id, IPC_STAT, &s);
        printf("Memoria compartida de clave %d en %p\n", key, p);
        insert_memShared(p, s.shm_segsz, key, M);
    } else {
        aux = errno;
        if(s.shm_segsz)
            shmctl(id, IPC_RMID, NULL);
        errno = aux;
        perror("Imposible obtener memoria shmget");
    }
}


void Cmd_shared(char * tr[], tListM * M) {
   if(tr[1] != NULL) {
       if(!strcmp(tr[1], "-free"))
           SharedFree(tr, M);
       else if(!strcmp(tr[1], "-delkey"))
           SharedDelkey(tr);
       else if(!strcmp(tr[1], "-create"))
           SharedCreate(tr, M);
       else
           Cmd_sharedAlloc(tr, M);
   }  else
       printShared(*M);
}


/*
 * mmap [-free] fich prm	mapea el fichero fich con permisos prm
 */

// Funciones auxiliares para mmap:
void * MapearFichero (char * fichero, int protection) {
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;

    if (protection&PROT_WRITE)
        modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
        return NULL;
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
        return NULL;
/* Guardar en la lista    InsertarNodoMmap (&L,p, s.st_size,df,fichero); */
    return p;
}

// -free fich: desmapea el ficherofich
void mmapFree(char * tr[], tListM *M) {
    int pos;
    dataMem item;

    for(pos = firstMem(*M); pos <= lastMem(*M); pos++) {
        item = getDataMem(pos, *M);

        if(!strcmp(item.Union.fichero.filename, tr[2])) {
            if(munmap(item.dir, item.size) == -1)
                perror("No se pudo desmapear.");
            else {
                printf("Desmapeado fichero %s\n", item.Union.fichero.filename);
                close(item.Union.fichero.df);
                deleteItemM(pos, M);
                return;
            }
        } else {
            printf("El fichero %s no esta mapeado\n", tr[2]);
        }
    }
}

void Cmd_mmap(char *tr[], tListM *M) {
    char *perm;
    void *p;
    int protection=0;

    dataMem item;
    time_t tiempo = time(0);
    struct tm * tlocal = localtime(&tiempo);
    struct stat file;
    stat(tr[1], &file);

    if (tr[1]==NULL || (!strcmp(tr[1], "-free") && tr[2] == NULL))
    {printMmap(*M); return;}
    if(!strcmp(tr[1], "-free") && tr[2] != NULL) {
        mmapFree(tr, M);
        return;
    } else {
        if ((perm=tr[2])!=NULL && strlen(perm)<4) {
            if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
            if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
            if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
        }
        if ((p=MapearFichero(tr[1],protection))==NULL)
            perror ("Imposible mapear fichero");
        else {
            item.cmdType = MMAP;
            item.size = file.st_size;
            item.dir = p;
            strftime(item.date, 128, "%b %d %H:%M",tlocal);
            item.Union.fichero.df = (int)open(tr[1], O_CREAT, O_RDONLY);
            strcpy(item.Union.fichero.filename, tr[1]);

            if(insertMem(item, M))
                printf ("fichero %s mapeado en %p\n", tr[1], p);
            else
                printf("No se ha podido mapear el archivo\n");
        }
    }
}


/*
 * read fiche addr cont 	Lee cont bytes desde fich a la direccion addr
 */

// Funcion auxiliar para read:
ssize_t LeerFichero (char *f, void *p, size_t cont) {
    struct stat s;
    ssize_t  n;
    int df,aux;

    if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
        return -1;
    if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
        cont = s.st_size;
    if ((n=read(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return n;
}

void Cmd_read(char * tr[]) {
    void *p;
    size_t cont=-1;  /* -1 indica leer todo el fichero*/
    ssize_t n;
    if (tr[1]==NULL || tr[2]==NULL){
        printf ("faltan parametros\n");
        return;
    }
    p= cadtop(tr[2]);
    //p=(void *) strtoul(tr[2], NULL, 16);  /*convertimos de cadena a puntero*/
    if (tr[3]!=NULL)
        cont=(size_t) atoll(tr[3]);

    if ((n=LeerFichero(tr[1],p,cont))==-1)
        perror ("Imposible leer fichero");
    else
        printf ("leidos %lld bytes de %s en %p\n",(long long) n,tr[1],p);
}


/*
 * write [-o] fiche addr cont 	Escribe cont bytes desde la direccion addr a fich (-o sobreescribe)
 */
void Cmd_write(char * tr[]) {
    char overwrite = 0;
    void* direc;
    int cont, fd;
    if (tr[1] == NULL) {
        printf("faltan parametros\n");
        return;
    }
    if (!strcmp("-o",tr[1])) overwrite=1;
    else if (tr[1][0] == '-'){
        printf("Opcion no reconocida\n");
        return;
    }

    if (tr[1+overwrite]==NULL){
        perror("Archivo no especificado.");
        return;
    }

    if (tr[2+overwrite]==NULL){
        perror("Direccion no especificada.");
        return;
    }

    if (tr[3+overwrite]==NULL){
        perror("cont no especificado.");
        return;
    }

    direc = (void *) strtoul(tr[2+overwrite],NULL,16);
    cont = atoi(tr[3+overwrite]);
    if (overwrite){
        fd = open(tr[1+overwrite],O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    }
    else{
        fd = open(tr[1+overwrite],O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    }
    if (fd == -1) perror("Error");
    if (write(fd, direc, cont)==-1){
        close(fd);
        perror("Error: ");
    } else {
        printf("escritos %d bytes desde %p \n",cont,direc);
        close(fd);
    }
}


/*
 * memdump addr cont 	Vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr
 */

// Funcion auxiliar para memdump:
void printMemdump(const char * p, int nbytes) {
    int i, j;
    unsigned char c;

    for(i = 0; i < nbytes; i += 25) {
        for (j = i; j < i + 25 && j < nbytes; j++) {
            c = *(p + j);

            if (c >= 32 && c <= 127)
                printf("%c ", c);
            else
                printf(" ");
        }
        printf("\n");

        for (j = i; j < i + 25 && j < nbytes; j++) {
            c = *(p + j);
            printf("%02X ", c);
        }
        printf("\n");
    }
}

void Cmd_memdump(char * tr[]) {
    char * p;
    int nbytes = 25, i, offset;
    unsigned char c;

    if(tr[1] == NULL)
        return;
    else {
        if(sscanf(tr[1], "0x%p", &p) == 0 || p == NULL) {
            perror("Direccion invalida.");
            return;
        } else {
            if(tr[2] != NULL)
                nbytes = atoi(tr[2]);

            printf("Volcando %d bytes desde la direccion %p\n", nbytes, p);
            printMemdump(p, nbytes);
        }
    }
}


/*
 * memfill addr cont byte 	Llena la memoria a partir de addr con byte
 */

// Funcion auxiliar para memfill:
void LlenarMemoria (void *p, size_t cont, unsigned char byte) {
    unsigned char *arr=(unsigned char *) p;
    size_t i;

    printf("Llenando %ld bytes de memoria con el byte %c(%x) a partir de la direccion %p\n", cont, byte, byte, p);

    for (i=0; i<cont;i++)
        arr[i]=byte;
}

void Cmd_memfill(char * tr[]) {
    size_t nbytes = 128;
    unsigned char byte = 'A';
    void * p;

    if(tr[1] == NULL)
        return;
    else
        p = (void *)strtoul(tr[1], NULL, 16);

    if(tr[2] != NULL)
        nbytes = atoi(tr[2]);

    if(tr[3] != NULL)
        byte = tr[3][0];

    LlenarMemoria(p, nbytes, byte);
}

/*
 * mem [-blocks|-funcs|-vars|-all|-pmap] ..	Muestra muestra detalles de la memoria del proceso
 */

// Funcion auxiliar para -pmap (mem):
void Do_MemPmap (void) /*sin argumentos*/
{ pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};

    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){ /*proceso hijo*/
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
            perror("cannot execute vmmap (Mac-OS)");

        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)");

        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
        if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
            perror("cannot execute procmap (OpenBSD)");

        exit(1);
    }
    waitpid (pid,NULL,0);
}

void Cmd_mem(char * tr[], tListM * M) {
    bool blocks = false, funcs = false, vars = false, pmap = false;

    // Variables locales:
    int int_local = 2;
    char char_local = 'b';
    double double_local = 2.3;

    // Variables estaticas inicializadas:
    static int int_slocalI = 4;
    static char char_slocalI = 'c';
    static double double_slocalI = 3.5;

    // Variables estaticas no inicializadas:
    static int int_slocalN;
    static char char_slocalN;
    static double double_slocalN;


    if(tr[1] == NULL || !strcmp(tr[1], "-all")) {
        blocks = true;
        funcs = true;
        vars = true;
    } else if(!strcmp(tr[1], "-blocks"))
        blocks = true;
    else if(!strcmp(tr[1], "-funcs"))
        funcs = true;
    else if(!strcmp(tr[1], "-vars"))
        vars = true;
    else if(!strcmp(tr[1], "-pmap"))
        pmap = true;
    else
        printf("Opcion %s no contemplada\n", tr[1]);

    if(vars) {
        printf("Variables locales\t%p\t%p\t%p\n", &int_local, &char_local, &double_local);
        printf("Variables globales\t%p\t%p\t%p\n", &int_globalI, &char_globalI, &double_globalI);
        printf("Var (N.I.) globales\t%p\t%p\t%p\n", &int_globalN, &char_globalN, &double_globalN);
        printf("Variables staticas\t%p\t%p\t%p\n", &int_slocalI, &char_slocalI, &double_slocalI);
        printf("Var (N.I.) staticas\t%p\t%p\t%p\n", &int_slocalN, &char_slocalN, &double_slocalN);
    }

    if(funcs) {
        printf("Funciones programa \t%p\t%p\t%p\n", &Cmd_chdir, &Cmd_authors, &Cmd_pid);
        printf("Funciones libreria \t%p\t%p\t%p\n", &scanf, &free, &strcat);
    }

    if(blocks) {
        printMemoryList(*M);
    }

    if(pmap)
        Do_MemPmap();

    vars = false;
    funcs = false;
    blocks = false;
    pmap = false;
}


/*
 * recurse [n]	Invoca a la funcion recursiva n veces
 */
void Cmd_recurse(int n) {
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

    if (n>0)
        Cmd_recurse(n-1);
}


// Implementacion de las funciones basicas:

void imprimirPrompt() {
    printf("-> ");
}

void leerEntrada(char * input) {
    fgets(input, MAX, stdin);
}

void procesarEntrada(char * input, char * tr[], tList *hist, fileList *listaFicheros, tListM * ListMem) {
    char * aux = malloc(sizeof(input));
    strcpy(aux, input);

    insertItem(aux, hist);
    TrocearCadena(input, tr);
    procesarCadena(input, tr, hist, listaFicheros, ListMem);

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
    else if(!strcmp(tr[0], "malloc"))
        return 19;
    else if(!strcmp(tr[0], "shared"))
        return 20;
    else if(!strcmp(tr[0], "mmap"))
        return 21;
    else if(!strcmp(tr[0], "read"))
        return 22;
    else if(!strcmp(tr[0], "write"))
        return 23;
    else if(!strcmp(tr[0], "memdump"))
        return 24;
    else if(!strcmp(tr[0], "memfill"))
        return 25;
    else if(!strcmp(tr[0], "mem"))
        return 26;
    else if(!strcmp(tr[0], "recurse"))
        return 27;
    return -1;
}

void procesarCadena(char * input, char * tr[], tList *hist, fileList *listaFicheros, tListM * ListMem) {
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
                Cmd_command(tr, hist, listaFicheros, ListMem);
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
                freeList(ListMem);
                deleteList(hist);
                deleteMem(ListMem);
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
            case 19:
                Cmd_malloc(tr, ListMem);
                break;
            case 20:
                Cmd_shared(tr, ListMem);
                break;
            case 21:
                Cmd_mmap(tr, ListMem);
                break;
            case 22:
                Cmd_read(tr);
                break;
            case 23:
                Cmd_write(tr);
                break;
            case 24:
                Cmd_memdump(tr);
                break;
            case 25:
                Cmd_memfill(tr);
                break;
            case 26:
                Cmd_mem(tr, ListMem);
                break;
            case 27:
                if(tr[1] == NULL)
                    break;
                else
                    Cmd_recurse(atoi(tr[1]));
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
    tListM ListMem;
    createEmptyList(&hist);
    createEmptyMem(&ListMem);

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
        procesarEntrada(input, tr, &hist, listaFicheros, &ListMem);
    }
}
