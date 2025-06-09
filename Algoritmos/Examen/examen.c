/*
 * GEI (ALGORITMOS) - UDC
 * EXAMEN PRACTICO
 *
 * Susana Vilarnovo Allal
 * GRUPO 1.3
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

// Declaracion de las funciones:
void ordenar(int v[], int n);
void aleatorio(int v[], int n);
void ascendente(int v[], int n);
void descendente(int v[], int n);

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

// Algoritmo de ordenacion:
void auxOrd(int v[], int izq, int der) {
    int piv, i, j, aux;

    if (izq < der) {
        piv = v[izq+1];
        // Intercambiar:
        v[izq+1] = v[izq];
        v[izq] = piv;

        i = izq + 1;
        j = der;

        while(i <= j) {
            while(i <= der && v[i] < piv)
                i += 1;
            while(v[j] > piv)
                j -= 1;

            if(i <= j) {
                //Intercambiar:
                aux = v[i];
                v[i] = v[j];
                v[j] = aux;

                i += 1;
                j -= 1;
            }
        }
        // Intercambiar:
        aux = v[izq];
        v[izq] = v[j];
        v[j] = aux;

        auxOrd(v, izq, j-1);
        auxOrd(v, j+1, der);
    }
}

void ordenar(int v[], int n) {
    auxOrd(v, 0, n-1);
}

// Funcion auxiliar para imprimir los vectores:
void listar_vector(int v[], int n) {
    int i;

    for(i = 0; i < n; i++)
        printf("%3d ", v[i]);
}

// Funcion necesaria para inicializar un vector con numeros pseudoaleatorios
void inicializar_semilla() {
    srand(time(NULL));
}

// Funciones auxiliares para generar los vectores:
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

// Funcion auxiliar para verificar que el vector dado esta ordenado:
bool ordenado(const int v[], int n) {
    int i;

    for(i = 0; i < n-1; i++) {
        if(v[i] > v[i+1])
            return false;
    }
    return true;
}

void test (void (*function)(int[], int), int sizeIt) {
    int * asc, * desc, *random;

    asc = malloc(sizeIt * sizeof(int)); // Vector ascendente
    desc = malloc(sizeIt * sizeof(int)); // Vector descendente
    random = malloc(sizeIt * sizeof(int)); // Vector aleatorio

    // Inicializamos los vectores:
    ascendente(asc, sizeIt);
    descendente(desc, sizeIt);
    aleatorio(random, sizeIt);

    // Vector aleatorio
    printf("Inicializacion aleatoria\n");
    listar_vector(random, sizeIt);
    printf("\n");
    printf("ordenado? %d\n", ordenado(random, sizeIt));
    function(random, sizeIt);
    listar_vector(random, sizeIt);
    printf("\n");
    printf("ordenado? %d\n", ordenado(random, sizeIt));

    // Vector descendente
    printf("Inicializacion descendente\n");
    listar_vector(desc, sizeIt);
    printf("\n");
    printf("ordenado? %d\n", ordenado(desc, sizeIt));
    function(desc, sizeIt);
    listar_vector(desc, sizeIt);
    printf("\n");
    printf("ordenado? %d\n", ordenado(desc, sizeIt));

    // Vector ascendente
    printf("Inicializacion ascendente\n");
    listar_vector(asc, sizeIt);
    printf("\n");
    printf("ordenado? %d\n", ordenado(asc, sizeIt));
    function(asc, sizeIt);
    listar_vector(asc, sizeIt);
    printf("\n");
    printf("ordenado? %d\n", ordenado(asc, sizeIt));

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
            for(i = 0; i < k; i++)
                tVector(v, n);
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
void printTablaCuadratica(iteration * it, int size_it) { // O(n²)
    int i;
    double cotaSubestimada, cotaAjustada, cotaSobrestimada;

    printf("%15s %15s %15s %15s %15s\n", "n", "t(n)", "t(n)/n^1.8", "t(n)/n^2", "t(n)/n^2.2");

    for(i = 0; i < size_it; i++) {
        cotaSubestimada = pow(it[i].size, 1.8);
        cotaAjustada = pow(it[i].size, 2);
        cotaSobrestimada = pow(it[i].size, 2.2);

        if(it[i].reps)
            printf("(%5d)*", it[i].reps);
        else
            printf("        ");
        printf("%8d %15.4f %15f %15f %15f\n", it[i].size, it[i].time, it[i].time/cotaSubestimada, it[i].time/cotaAjustada, it[i].time/cotaSobrestimada);
    }
    free(it);
}

void printTablaNLogN(iteration * it, int size_it) { // O(nlogn)
    int i;
    double cotaSubestimada, cotaAjustada, cotaSobrestimada;

    printf("%15s %15s %15s %20s %15s\n", "n", "t(n)", "t(n)/n", "t(n)/n^(0.94)logn", "t(n)/n^1.5");

    for(i = 0; i < size_it; i++) {
        cotaSubestimada = pow(it[i].size, 1);
        cotaAjustada = pow(it[i].size, 0.94) * log2(it[i].size);
        cotaSobrestimada = pow(it[i].size, 1.5);

        if(it[i].reps)
            printf("(%5d)*", it[i].reps);
        else
            printf("        ");
        printf("%8d %15.4f %15f %15f %18f\n", it[i].size, it[i].time, it[i].time/cotaSubestimada, it[i].time/cotaAjustada, it[i].time/cotaSobrestimada);
    }
    free(it);
}

int main() {
    iteration * it;
    int size_it = 8; // Rango de n: [500, 64000]
    int n = 10; // Tamano de los vectores

    inicializar_semilla();

    // Validar los tests:
    printf("Test: ordenacion\n");
    test(ordenar, n);
    printf("\n\n");

    // Obtener las tablas:
    printf("Ordenacion: vector aleatorio\n");
    it = getTiempos(ordenar, aleatorio, size_it);
    printTablaNLogN(it, size_it);
    printf("\n");

    printf("Ordenacion: vector descendente\n");
    it = getTiempos(ordenar, descendente, size_it);
    printTablaCuadratica(it, size_it);
    printf("\n");

    printf("Ordenacion: vector ascendente\n");
    it = getTiempos(ordenar, ascendente, size_it);
    printTablaCuadratica(it, size_it);
    printf("\n");

    return 0;
}
