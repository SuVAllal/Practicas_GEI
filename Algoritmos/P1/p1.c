/*
 * GEI (ALGORITMOS) - UDC
 * P1 - Suma de la subsecuencia maxima
 *
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

// Primer algoritmo
int sumaSubMax1(int v[], int n) {
	int i, j;
    int estaSuma, sumaMax = 0;

	for (i = 0; i < n; i++) {
		estaSuma = 0;

		for (j = i; j < n; j++) {
			estaSuma += v[j];

			if (estaSuma > sumaMax)
				sumaMax = estaSuma;
		}
	}
	return sumaMax;
}

// Segundo algoritmo
int sumaSubMax2(int v[], int n) {
	int j;
    int estaSuma = 0;
	int sumaMax = 0;

	for (j = 0; j < n; j++) {
		estaSuma += v[j];

		if (estaSuma > sumaMax)
			sumaMax = estaSuma;
		else if (estaSuma < 0)
			estaSuma = 0;
	}
	return sumaMax;
}


// Función auxiliar para imprimir vectores:
void listar_vector(int v[], int n) {
	int i;

    for(i = 0; i < n; i++)
		printf("%3d ", v[i]);
}

// Funciones necesarias para inicializar un vector con numeros pseudoaleatorios en el rango [-n, n]
void inicializar_semilla() {
    srand(time(NULL));
}

void aleatorio(int v[], int n) {
    int i;
    int m = 2 * n + 1;
    for(i = 0; i < n; i++)
        v[i] = (rand() % m) - n;
}

// Primer test para probar los algoritmos con vectores fijos
void test1() {
    int i;
	int sizeVector = 5;
	int nVector = 6;
	int v[][5] = {{-9, 2, -5, -4, 6}, {4, 0, 9, 2, 5}, {-2, -1, -9, -7, -1}, {9, -2, 1, -7, -8}, {15, -2, -5, -4, 16}, {7, -5, 6, 7, -7}};

    printf("Test 1:\n\n");
	//sumaSubMax1
	printf("Test de sumaSubMax1:\n");
    printf("%s %20s %s\n", "Secuencia", "", "Resultado");
	//printf("Secuencia\t\tResultado\n");
	for (i = 0; i < nVector; i++) {
		listar_vector(v[i], sizeVector);
		printf("%15s%d\n", "", sumaSubMax1(v[i], sizeVector));
	}

	//sumaSubMax2
	printf("\nTest de sumaSubMax2: \n");
    printf("%s %20s %s\n", "Secuencia", "", "Resultado");

	for (i = 0; i < nVector; i++) {
		listar_vector(v[i], sizeVector);
		printf("%15s%d\n", "", sumaSubMax2(v[i], sizeVector));
	}
}

// Segundo test para probar los algoritmos con vectores aleatorios
void test2() {
    int i, a, b;
    int v[9];

    printf("Test 2:\n\n");
    printf("%40s%15s%15s\n", "", "sumaSubMax1", "sumaSubMax2");

    for(i = 0; i < 10; i++) {
        aleatorio(v, 9);
        listar_vector(v, 9);
        a = sumaSubMax1(v, 9);
        b = sumaSubMax2(v, 9);
        printf("%15d%15d\n", a, b);
    }
}

// Funcion auxiliar para calcular los tiempos de ejecucion de los algoritmos a probar
iteration * getTiempos(int (*func)(int[], int), int num_it) {
    int n, j, i;
    int k = 10000; // Constante k, usada en caso de no superar el umbral de confianza
    double t1, t2, ta, tb, t;
    iteration * it = malloc(num_it*sizeof (iteration));

    for(n = 500, j = 0; j < num_it; n *= 2, j++) { // n sigue una progresion geometrica de razon 2
        int * v = malloc(sizeof(int) * n);
        it[j].reps = 0;
        it[j].size = n;

        // Generamos los vectores y calculamos los tiempos
        aleatorio(v, n);
        ta = microsegundos();
        func(v, n);
        tb = microsegundos();
        t = tb - ta;

        // En caso de no superar el umbral de confianza:
        if(t < 500) {
            ta = microsegundos();
            for(i = 0; i < k; i++) {
                aleatorio(v, n);
                func(v, n);
            }
            tb = microsegundos();
            t1 = tb - ta;

            ta = microsegundos();
            for(i = 0; i < k; i++)
                aleatorio(v, n);
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

// Funcion para imprimir la tabla con los tiempos del primer algoritmo (sumaSubMax1), con complejidad cuadratica, O(n²)
void printTabla1(iteration * it, int size_it) {
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
        printf("%8d %15.3f %15f %15f %15f\n", it[i].size, it[i].time, it[i].time/cotaSubestimada, it[i].time/cotaAjustada, it[i].time/cotaSobrestimada);
    }
    free(it);
}

// Funcion para imprimir la tabla con los tiempos del segundo algoritmo (sumaSubMax2), con complejidad lineal, O(n)
void printTabla2(iteration * it, int size_it) {
    int i;
    double cotaSubestimada, cotaAjustada, cotaSobrestimada;

    printf("%15s %15s %15s %15s %15s\n", "n", "t(n)", "t(n)/n^0.8", "t(n)/n^1", "t(n)/n^1.2");

    for(i = 0; i < size_it; i++) {
        cotaSubestimada = pow(it[i].size, 0.8);
        cotaAjustada = pow(it[i].size, 1);
        cotaSobrestimada = pow(it[i].size, 1.2);

        if(it[i].reps)
            printf("(%5d)*", it[i].reps);
        else
            printf("        ");
        printf("%8d %15.3f %15f %15f %15f\n", it[i].size, it[i].time, it[i].time/cotaSubestimada, it[i].time/cotaAjustada, it[i].time/cotaSobrestimada);
    }
    free(it);
}


int main() {
    // Creacion de las iteraciones de los algoritmos (7 filas de datos)
    iteration * it;
    int size_it = 7;

    // Inicializamos el vector con los numeros pseudoaleatorios
    inicializar_semilla();

    // Realizamos los tests sobre los algoritmos
    test1();
    printf("\n\n");
    test2();
    printf("\n\n");

    // Tablas sumaSubMax1
    it = getTiempos(sumaSubMax1, size_it);
    printf("SumaSubMax1\n");
    printTabla1(it, size_it);
    printf("\n\n");

    it = getTiempos(sumaSubMax1, size_it);
    printf("SumaSubMax1\n");
    printTabla1(it, size_it);
    printf("\n\n");

    it = getTiempos(sumaSubMax1, size_it);
    printf("SumaSubMax1\n");
    printTabla1(it, size_it);
    printf("\n\n");

    // Tablas sumaSubMax2
    it = getTiempos(sumaSubMax2, size_it);
    printf("SumaSubMax2\n");
    printTabla2(it, size_it);
    printf("\n\n");

    it = getTiempos(sumaSubMax2, size_it);
    printf("SumaSubMax2\n");
    printTabla2(it, size_it);
    printf("\n\n");

    it = getTiempos(sumaSubMax2, size_it);
    printf("SumaSubMax2\n");
    printTabla2(it, size_it);
    printf("\n\n");

    return 0;
}
