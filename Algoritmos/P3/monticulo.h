#ifndef P3_MONTICULO_H
#define P3_MONTICULO_H

// Definicion del monticulo:
#define TAM 256000

struct monticulo {
    int ultimo;
    int vector[TAM];
};

typedef struct monticulo * pmonticulo;


// Definicion de las funciones:
void inicializarMonticulo(pmonticulo * m);
void Hundir(pmonticulo m, int i);
void crearMonticulo(const int v[], int n, pmonticulo m);
int quitarMenor(pmonticulo m);
//void ordenarPorMonticulos(int v[], int n);
void borrarMonticulo(pmonticulo * m);

#endif //P3_MONTICULO_H