/*
 * GEI (ALGORITMOS) - UDC
 * P3 - Monticulos de minimos
 * Alumno 1: Laura Palmeiro Castro
 * Alumno 2: Emilio Pulpeiro Rodriguez
 * Alumno 3: Susana Vilarnovo Allal
 * GRUPO 1.3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <stdbool.h>

#include "monticulo.h"


// Definicion del struct para cada iteracion de los algoritmos
typedef struct {
    int reps;
    double time;
    int size;
} iteration;


// Funcion para la obtencion de la hora del sistema
double microsegundos() {
    struct timeval t;
    if(gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}


// Funcion auxiliar para imprimir el monticulo y los vectores:
void listar_monticulo(pmonticulo m) {
    int i;

    for(i = 0; i <= m->ultimo; i++)
        printf("%3d ", m->vector[i]);
}

void listar_vector(int v[], int n) {
    int i;

    for(i = 0; i < n; i++)
        printf("%3d ", v[i]);
}


// Funcion necesaria para inicializar un vector con numeros pseudoaleatorios
void inicializar_semilla() {
    srand(time(NULL));
}


// Funciones para generar los vectores:
void aleatorio(int v[], int n) { // Genera vector aleatorio
    int i;
    int m = 2 * n + 1;

    for(i = 0; i < n; i++)
        v[i] = (rand() % m) - n;
}


void descendente(int v[], int n) { // Genera vector descendente
    int i;

    for(i = 0; i < n; i++)
        v[i] = n - i;
}


void ascendente(int v[], int n) { // Genera vector ascendente
    int i;

    for(i = 0; i < n; i++)
        v[i] = i + 1;
}

// Funcion para verificar que el vector dado esta ordenado:
bool ordenado(const int v[], int n) {
    int i;

    for(i = 0; i < n-1; i++) {
        if(v[i] > v[i+1])
            return false;
    }
    return true;
}


// Funcion auxiliar para medir los tiempos de crearMonticulo:
void crear_monticulo(int v[], int n) {
    pmonticulo m;

    inicializarMonticulo(&m);
    crearMonticulo(v, n, m);
    borrarMonticulo(&m);
}


// Algoritmo de ordenacion por monticulos:
void ordenarPorMonticulos(int v[], int n) {
    int i;
    pmonticulo m;

    inicializarMonticulo(&m);
    crearMonticulo(v, n, m);

    for(i = 0; i < n; i++)
        v[i] = quitarMenor(m);

    borrarMonticulo(&m);
}


// Implementacion de los tests:
void test() {
    int size = 10;
    pmonticulo m;

    inicializarMonticulo(&m);

    int * random;
    random = malloc(size * sizeof(int));
    aleatorio(random, size);

    printf("Test 1: crearMonticulo():\n");
    printf("Vector a usar para crear el monticulo de minimos:\n");
    listar_vector(random, size);
    printf("\n");
    crearMonticulo(random, size, m);
    printf("Monticulo creado:\n");
    listar_monticulo(m);
    printf("\n\n");

    printf("Test 2: quitarMenor():\n");
    printf("El menor eliminado ha sido: %d\n", quitarMenor(m));
    printf("El nuevo monticulo es:\n");
    listar_monticulo(m);
    printf("\n\n");

    free(random);
    borrarMonticulo(&m);

}

void testOrdenacion() {
    int size = 10;
    int * asc, * desc, * random;

    asc = malloc(size * sizeof(int));
    desc = malloc(size * sizeof(int));
    random = malloc(size * sizeof(int));

    ascendente(asc, size);
    descendente(desc, size);
    aleatorio(random, size);

    printf("Test 3: ordenacionPorMonticulos():\n");

    printf("Inicializacion ascendente:\n");
    listar_vector(asc, size);
    printf("\n");
    printf("ordenado? %d\n", ordenado(asc, size));
    ordenarPorMonticulos(asc, size);
    listar_vector(asc, size);
    printf("\n");
    printf("ordenado? %d\n\n", ordenado(asc, size));

    printf("Inicializacion descendente:\n");
    listar_vector(desc, size);
    printf("\n");
    printf("ordenado? %d\n", ordenado(desc, size));
    ordenarPorMonticulos(desc, size);
    listar_vector(desc, size);
    printf("\n");
    printf("ordenado? %d\n\n", ordenado(desc, size));

    printf("Inicializacion aleatoria:\n");
    listar_vector(random, size);
    printf("\n");
    printf("ordenado? %d\n", ordenado(random, size));
    ordenarPorMonticulos(random, size);
    listar_vector(random, size);
    printf("\n");
    printf("ordenado? %d\n\n", ordenado(random, size));

    free(asc);
    free(desc);
    free(random);

}


// Funcion auxiliar para calcular los tiempos de ejecucion de los algoritmos a probar
iteration * getTiempos(void (*func)(int[], int), void (*tVector)(int[], int), int num_it) {
    int n, j, i;
    int k = 10000; // Constante k, usada en caso de no superar el umbral de confianza
    double t1, t2, ta, tb, t = 0;
    iteration * it = malloc(num_it*sizeof (iteration));

    for(n = 500, j = 0; j < num_it && t < 1000000.0; n *= 2, j++) { // n sigue una progresion geometrica de razon 2
        int * v = malloc(sizeof(int) * n);
        it[j].reps = 0;
        it[j].size = n;

        // Generamos los vectores y calculamos los tiempos
        tVector(v, n);
        ta = microsegundos();
        func(v, n);
        tb = microsegundos();
        t = tb - ta;

        // En caso de no superar el umbral de confianza:
        if(t < 500) {
            ta = microsegundos();
            for(i = 0; i < k; i++) {
                tVector(v, n);
                func(v, n);
            }
            tb = microsegundos();
            t1 = tb - ta;

            ta = microsegundos();
            for(i = 0; i < k; i++) {
                tVector(v, n);
            }
            tb = microsegundos();
            t2 = tb - ta;

            t = (t1 - t2)/k;
            it[j].reps = k; // Repetimos el algoritmo k veces
        }
        it[j].time = t; // Guardamos el tiempo de cada iteracion
        free(v);
    }
    return it;
}


// Funciones para imprimir las tablas:
void printTablaLineal(iteration * it, int size_it) { // O(n)
    int i;
    double cotaSubestimada, cotaAjustada, cotaSobrestimada;

    printf("%15s %15s %15s %15s %15s\n",
           "n", "t(n)", "t(n)/n^0.8", "t(n)/n^1", "t(n)/n^1.2");

    for(i = 0; i < size_it; i++) {
        cotaSubestimada = pow(it[i].size, 0.8);
        cotaAjustada = pow(it[i].size, 1);
        cotaSobrestimada = pow(it[i].size, 1.2);

        if(it[i].reps)
            printf("(%5d)*", it[i].reps);
        else
            printf("        ");
        printf("%8d %15.4f %15f %15f %15f\n", it[i].size, it[i].time,
               it[i].time/cotaSubestimada, it[i].time/cotaAjustada,
               it[i].time/cotaSobrestimada);
    }
    free(it);
}

void printTablaNLogN(iteration * it, int size_it) { // O(nlogn)
    int i;
    double cotaSubestimada, cotaAjustada, cotaSobrestimada;

    printf("%15s %15s %15s %15s %15s\n",
           "n", "t(n)", "t(n)/n", "t(n)/nlogn", "t(n)/n^1.2");

    for(i = 0; i < size_it; i++) {
        cotaSubestimada = pow(it[i].size, 1);
        cotaAjustada = it[i].size * log2(it[i].size);
        cotaSobrestimada = pow(it[i].size, 1.2);

        if(it[i].reps)
            printf("(%5d)*", it[i].reps);
        else
            printf("        ");
        printf("%8d %15.4f %15f %15f %15f\n", it[i].size, it[i].time,
               it[i].time/cotaSubestimada, it[i].time/cotaAjustada,
               it[i].time/cotaSobrestimada);
    }
    free(it);
}


int main() {
    int num_it = 10;
    iteration * it;

    inicializar_semilla();

    // Validar los tests:
    printf("Validacion de los tests:\n\n");
    test();
    testOrdenacion();
    printf("\n");

    // Obtener tablas:
    printf("Obtencion de las tablas:\n\n");
    printf("Creacion del monticulo de minimos:\n");
    it = getTiempos(crear_monticulo, aleatorio, num_it);
    printTablaLineal(it, num_it);
    printf("\n\n");

    printf("Ordenacion del vector por monticulos: vector ascendente\n");
    it = getTiempos(ordenarPorMonticulos, ascendente, num_it);
    printTablaNLogN(it, num_it);
    printf("\n");

    printf("Ordenacion del vector por monticulos: vector descendente\n");
    it = getTiempos(ordenarPorMonticulos, descendente, num_it);
    printTablaNLogN(it, num_it);
    printf("\n");

    printf("Ordenacion del vector por monticulos: vector desordenado\n");
    it = getTiempos(ordenarPorMonticulos, aleatorio, num_it);
    printTablaNLogN(it, num_it);
    printf("\n");
}