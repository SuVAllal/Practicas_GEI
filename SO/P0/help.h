#ifndef P0_HELP_H
#define P0_HELP_H

#define HELP_AUTHORS "authors [-n|-l] \n\
Muestra los nombres y/o logins de los autores\n"

#define HELP_PID "pid [-p] \n\
Muestra el pid del shell o de su proceso padre\n"

#define HELP_CHDIR "chdir [dir] \n\
Cambia (o muestra) el directorio actual del shell\n"

#define HELP_DATE "date \n\
Muestra la fecha actual\n"

#define HELP_TIME "time \n\
Muestra la hora actual\n"

#define HELP_HIST "hist [-c|-N] \n\
Muestra (o borra) el historico de comandos \n\
-N: muestra los N primeros \n\
-c: borra el historico\n"

#define HELP_COMMAND "command [-N] \n\
Repite el comando N (del historico)\n"

#define HELP_OPEN "open fich m1 m2... \n\
Abre el fichero fich. y lo anade a la lista de ficheros abiertos del shell \n\
m1, m2... es el modo de apertura (uno de los siguientes): \n\
cr: O_CREAT \n\
ap: O_APPEND \n\
ex: O_EXCL \n\
ro: O_RDONLY \n\
rw: O_RDWR \n\
wo: O_WRONLY \n\
tr: O_TRUNC\n"

#define HELP_CLOSE "close df \n\
Cierra el descriptor df y elimina el correspondiente fichero de la lista de ficheros abiertos\n"

#define HELP_DUP "dup df \n\
Duplica el descriptor de fichero df y anade una nueva entrada a la lista de ficheros abiertos\n"

#define HELP_LISTOPEN "listopen [n] \n\
Lista los ficheros abiertos (al menos n) del shell\n"

#define HELP_INFOSYS "infosys \n\
Muestra informacion de la maquina donde corre el shell\n"

#define HELP_HELP "help [cmd|-lt|-T] \n\
Muestra ayuda sobre los comandos \n\
-lt: lista topics de ayuda \n\
-T topic: lista comandos sobre ese topic \n\
cmd: info sobre el comando cmd\n"

#define HELP_EXIT "quit, exit, bye \n\
Termina la ejecuion del shell\n"

#define HELP_TEXT "Comandos disponibles: \n\
authors, pid, chdir, date, time, hist, command, open, close, dup, listopen, infosys, help, exit, bye, quit\n"

#endif //P0_HELP_H
