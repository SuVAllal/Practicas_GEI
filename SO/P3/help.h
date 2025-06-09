/*
 * P3 - SISTEMAS OPERATIVOS (UDC)
 * Alumno 1: Susana Vilarnovo Allal (s.vallal@udc.es)
 * Alumno 2: Laura Palmeiro Castro (l.pcastro@udc.es)
 */

#ifndef P3_HELP_H
#define P3_HELP_H

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

#define HELP_CREATE "create [-f] [name]\n\
Crea un directorio o un fichero (-f)\n"

#define HELP_STAT "stat [-long][-link][-acc] name1 name2 ...      lista ficheros;\n\
-long: listado largo\n\
-acc: acesstime\n\
-link: si es enlace simbolico, el path contenido\n"

#define HELP_LIST "list [-reca] [-recb] [-hid][-long][-link][-acc] n1 n2 ...     lista contenidos de directorios\n\
-hid: incluye los ficheros ocultos\n\
-recb: recursivo (antes)\n\
-reca: recursivo (despues)\n\
resto parametros como stat\n"

#define HELP_DELETE "delete [name1 name2 ...]\n\
Borra ficheros o directorios vacios\n"

#define HELP_DELTREE "deltree [name1 name2 ...]\n\
Borra ficheros o directorios no vacios recursivamente\n"

#define HELP_MALLOC "malloc [-free] [tam]\n\
asigna un bloque de memoria de tamano tam con malloc\n\
-free: desasigna un bloque de memoria de tamano tam asignado con malloc\n"

#define HELP_SHARED "shared [-free|-create|-delkey] cl [tam]\n\
asigna memoria compartida con clave cl en el programa\n\
-create cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam\n\
-free cl: desmapea el bloque de memoria compartida de clave cl\n\
-delkey cl: elimina del sistema (sin desmapear) la clave de memoria\n"

#define HELP_MMAP "mmap [-free] fich prm\n\
mapea el fichero fich con permisos prm\n\
-free fich: desmapea el ficherofich\n"

#define HELP_READ "read fiche addr cont\n\
Lee cont bytes desde fich a la direccion addr\n"

#define HELP_WRITE "write [-o] fiche addr cont\n\
Escribe cont bytes desde la direccion addr a fich (-o sobreescribe)\n"

#define HELP_MEMDUMP "memdump addr cont\n\
Vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr\n"

#define HELP_MEMFILL "memfill addr cont byte\n\
Llena la memoria a partir de addr con byte\n"

#define HELP_MEM "mem [-blocks|-funcs|-vars|-all|-pmap] ...\n\
Muestra detalles de la memoria del proceso\n\
-blocks: los bloques de memoria asignados\n\
-funcs: las direcciones de las funciones\n\
-vars: las direcciones de las variables\n\
:-all: todo\n\
-pmap: muestra la salida del comando pmap(o similar)\n"

#define HELP_RECURSE "recurse [n]\n\
Invoca a la funcion recursiva n veces\n"

#define HELP_UID "uid [-get|-set] [-l] [id] 	 Accede a las credenciales del proceso que ejecuta el shell \n\
-get: muestra las credenciales \n\
-set id: establece la credencial al valor numerico id \n\
-set -l id: establece la credencial a login id \n"

#define HELP_SHOWVAR "showvar var	Muestra el valor y las direcciones de la variable de entorno var \n"

#define HELP_CHANGEVAR "changevar [-a|-e|-p] var valor	Cambia el valor de una variable de entorno \n\
-a: accede por el tercer arg de main \n\
-e: accede mediante environ \n\
-p: accede mediante putenv \n"

#define HELP_SUBSVAR "subsvar [-a|-e] var1 var2 valor	Sustituye la variable de entorno var1 \n\
con var2=valor \n\
-a: accede por el tercer arg de main \n\
-e: accede mediante environ \n"

#define HELP_SHOWENV "showenv [-environ|-addr] 	 Muestra el entorno del proceso \n\
-environ: accede usando environ (en lugar del tercer arg de main) \n\
-addr: muestra el valor y donde se almacenan environ y el 3er arg main \n"

#define HELP_FORK "fork 	El shell hace fork y queda en espera a que su hijo termine \n"

#define HELP_EXEC "exec VAR1 VAR2 ..prog args....[@pri]	Ejecuta, sin crear proceso,prog con argumentos \n\
en un entorno que contiene solo las variables VAR1, VAR2... \n"

#define HELP_JOBS "jobs 	Lista los procesos en segundo plano \n"

#define HELP_DELJOBS "deljobs [-term][-sig]	Elimina los procesos de la lista procesos en sp \n\
-term: los terminados \n\
-sig: los terminados por senal \n"

#define HELP_JOB "job [-fg] pid	Muestra informacion del proceso pid. \n\
-fg: lo pasa a primer plano \n"

#define HELP_HELP "help [cmd|-lt|-T] \n\
Muestra ayuda sobre los comandos \n\
-lt: lista topics de ayuda \n\
-T topic: lista comandos sobre ese topic \n\
cmd: info sobre el comando cmd\n"

#define HELP_EXIT "quit, exit, bye \n\
Termina la ejecuion del shell\n"

#define HELP_TEXT "Comandos disponibles: \n\
authors, pid, chdir, date, time, hist, command, open, close, dup, listopen, infosys, create, stat, list, delete, deltree, malloc, shared, mmap, read, write, memdump, memfill, mem, recurse, uid, showvar, changevar, subsvar, shpwenv, fork, exec, jobs, deljobs, job, help, exit, bye, quit\n"

#endif //P3_HELP_H
