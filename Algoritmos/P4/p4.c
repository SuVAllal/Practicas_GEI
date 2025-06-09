#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include "matriz.h"

#define INT_MAX 99999;

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


// Funcion auxiliar para imprimir matrices:
void print_matrix(matriz m, int n) {
    int i, j;

    for(i = 0; i < n; i++) {
        printf("[");
        for(j = 0; j < n; j++)
            printf("%2d", m[i][j]);
        printf(" ]\n");
    }
}


// Algoritmo a estudiar:
void dijkstra(matriz grafo, matriz distancias, int tam) {
    int n, i, j, min, v = 0;
    int *noVisitados = malloc(tam*sizeof(int));

    for(n = 0; n < tam; n++) {
        for(i = 0; i < tam; i++) {
            noVisitados[i] = 1;
            distancias[n][i] = grafo[n][i];
        }
        noVisitados[n] = 0;

        for(j = 1; j < tam; j++) {
            min = INT_MAX;

            for(i = 0; i < tam; i++) {
                if(noVisitados[i] == 1 && distancias[n][i] < min) {
                    min = distancias[n][i];
                    v = i;
                }
            }

            noVisitados[v] = 0;
            for(i = 0; i < tam; i++) {
                if(noVisitados[i] == 1 && distancias[n][i] > distancias[n][v] + grafo[v][i])
                    distancias[n][i] = distancias[n][v] + grafo[v][i];
            }
        }
    }
    free(noVisitados);
}


// Funcion auxiliar para rellenar una matriz con unos valores dados:
void iniMatrizTest(const int v[], matriz m, int n) {
    int i, j, k = 0;

    for(i = 0; i < n; i++) {
        for(j = i+1; j < n; j++) {
            m[i][j] = v[k];
            k++;
        }
    }

    for(i = 0; i < n; i++) {
        for(j = 0; j <= i; j++) {
            if(i==j)
                m[i][j] = 0;
            else
                m[i][j] = m[j][i];
        }
    }
}

// Test para verificar el correcto funcionamiento del algoritmo:
void test_dijkstra(int size, int v[]) {
    matriz m, distancias;

    m = crearMatriz(size);
    distancias = crearMatriz(size);
    iniMatrizTest(v, m, size);

    printf("Matriz de adyacencia:\n");
    print_matrix(m, size);
    printf("\n");

    // Ejecutamos el algortimo:
    dijkstra(m, distancias, size);

    printf("Matriz con las distancias minimas:\n");
    print_matrix(distancias, size);

    liberarMatriz(m, size);
    liberarMatriz(distancias, size);

}

void test() {
    int size1 = 5, size2 = 4;
    int v1[] = {1, 8, 4, 7, 2, 6, 5, 9, 5, 3};
    int v2[] = {1, 4, 7, 2, 8, 3};

    // Figura 2: Primer ejemplo
    printf("Test 1:\nFigura 2: Primer ejemplo:\n");
    test_dijkstra(size1, v1);

    printf("\n\n");

    // Figura 3: Segundo ejemplo
    printf("Test 2:\nFigura 3: Segundo ejemplo:\n");
    test_dijkstra(size2, v2);
}


// Funcion auxiliar para calcular los tiempos de ejecucion de los algoritmos a probar
iteration * getTiempos(void (*func)(matriz, matriz, int), int num_it) {
    int n, j, i;
    int k = 10000; // Constante k, usada en caso de no superar el umbral de confianza
    double t1, t2, ta, tb, t = 0;
    iteration * it = malloc(num_it*sizeof (iteration));

    for(n = 7, j = 0; j < num_it && n < 1000 && t < 1000000.0; n *= 2, j++) { // n sigue una progresion geometrica de razon 2
        matriz m, distancias;
        it[j].reps = 0;
        it[j].size = n;

        // Generamos las matrices y calculamos los tiempos
        m = crearMatriz(n);
        distancias = crearMatriz(n);
        iniMatriz(m, n);

        ta = microsegundos();
        func(m, distancias, n);
        tb = microsegundos();
        t = tb - ta;

        // En caso de no superar el umbral de confianza:
        if(t < 500) {
            ta = microsegundos();
            for(i = 0; i < k; i++) {
                iniMatriz(m, n);
                func(m, distancias, n);
            }
            tb = microsegundos();
            t1 = tb - ta;

            ta = microsegundos();
            for(i = 0; i < k; i++) {
                iniMatriz(m, n);
            }
            tb = microsegundos();
            t2 = tb - ta;

            t = (t1 - t2)/k;
            it[j].reps = k; // Repetimos el algoritmo k veces
        }
        it[j].time = t; // Guardamos el tiempo de cada iteracion
        liberarMatriz(m, n);
        liberarMatriz(distancias, n);
    }
    return it;
}


// Funciones para imprimir las tablas:
void printTablaCuadratica(iteration * it, int size_it) { // O(n²)
    int i;
    double cotaSubestimada, cotaAjustada, cotaSobrestimada;

    printf("%15s %15s %15s %15s %15s\n", "n", "t(n)", "t(n)/n^2.5", "t(n)/n^2.75", "t(n)/n^3");

    for(i = 0; i < size_it; i++) {
        cotaSubestimada = pow(it[i].size, 2.5);
        cotaAjustada = pow(it[i].size, 2.75);
        cotaSobrestimada = pow(it[i].size, 3);

        if(it[i].reps)
            printf("(%5d)*", it[i].reps);
        else
            printf("        ");
        printf("%8d %15.4f %15f %15f %15f\n", it[i].size, it[i].time, it[i].time/cotaSubestimada, it[i].time/cotaAjustada, it[i].time/cotaSobrestimada);
    }
    free(it);
}


int main() {
    int num_it = 8;
    iteration * it;

    // Validar los tests:
    test();

    // Obtener tablas:
    printf("\n\nObtencion de las tablas:\n\n");
    it = getTiempos(dijkstra, num_it);
    printTablaCuadratica(it, num_it);

}
