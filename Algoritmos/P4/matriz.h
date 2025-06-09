#ifndef P4_MATRIZ_H
#define P4_MATRIZ_H

#define TAM_MAX 1000

typedef int ** matriz;

matriz crearMatriz(int n);
void iniMatriz(matriz m, int n);
void liberarMatriz(matriz m, int n);

#endif //P4_MATRIZ_H
