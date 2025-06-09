package e2;

import java.util.Arrays;

public class ImmutableMatrix {
    private final int[][] matrix;

    // METODOS CONSTRUCTORES:

    // Un constructor que acepta un array bidimensional de valores enteros:
    public ImmutableMatrix(int[][] arr) {
        if(arr == null || !isRagged(arr)) // Si el array es nulo o irregular lanza una excepcion
            throw new IllegalArgumentException("El array proporcionado no es valido.\n");

        this.matrix = copyArray(arr); // Copiamos el array en la matriz

    }

    // Funcion auxiliar para comprobar que el array pasado por parametro no es irregular (ragged):
    private static boolean isRagged(int[][] arr) {
        int longitud = arr[0].length; // Guarda la longitud de la primera fila de la matriz (posicion 0 del array)

        // Comprobamos fila a fila que todas tengan la misma longitud
        for (int[] ints : arr) {
            if (ints.length != longitud)
                return false;
        }
        return true;
    }

    // Funcion auxiliar para copiar el array (util para el metodo toArray2D):
    private int[][] copyArray(int[][] arr) {
        // Creacion de la matriz a devolver:
        int filas = arr.length;
        int columnas = arr[0].length;
        int[][] copia = new int[filas][columnas];

        // Copiamos los valores del array en su posicion correcta en la matriz:
        for(int i = 0; i < filas; i++)
            copia[i] = Arrays.copyOf(arr[i], arr[i].length);

        // Devolvemos el array convertido en matriz
        return copia;
    }


    // Un constructor que acepta dos valores enteros (filas y columnas) y crea una matriz con esas dimensiones:
    public ImmutableMatrix(int filas, int columnas) {

        if(filas <= 0 || columnas <= 0) // Si en numero de filas o columnas es negativo lanza una excepcion
            throw new IllegalArgumentException("Las filas y las columnas deben ser valores enteros positivos.\n");

        // Creamos la matriz
        this.matrix = new int[filas][columnas];

        // Rellenamos la matriz con valores crecientes desde el 1
        int cnt = 1;
        for(int i = 0; i < filas; i++) {
            for(int j = 0; j < columnas; j++)
                matrix[i][j] = cnt++;
        }

    }


    // METODOS:

    // Devuelve una representacion en String del objeto
    public String toString() {
        // Creacion del StringBuilder
        StringBuilder matrixResult = new StringBuilder();

        // Guardamos las filas de la matriz en el string
        for (int[] ints : matrix) {
            matrixResult.append(Arrays.toString(ints));
            matrixResult.append("\n");
        }

        // Devolvemos el StringBuilder convertido a String
        return matrixResult.toString();
    }


    // at: Devuelve el elemento en la posicion indicada
    public int at(int fila, int columna) {
        // Si las coordenadas no son validas lanza una excepcion
        if(fila < 0 || columna < 0 || fila > matrix.length || columna > matrix[0].length)
            throw new IllegalArgumentException("La posicion indicada no es valida.\n");

        // Devolvemos el elemento correspondiente a las coordenadas dadas
        return matrix[fila][columna];
    }


    // rowCount: Devuelve el numero de filas de la matriz
    public int rowCount() {
        return matrix.length;
    }


    // columnCount: Devuelve el numero de columnas de la matriz
    public int columnCount() {
        return matrix[0].length;
    }


    // toArray2D: Devuelve una representacion de la amtriz como un array 2D.
    public int[][] toArray2D() {
        return copyArray(matrix);
    }


    // reverse: Devuelve otra matriz con las filas en orden inverso
    public ImmutableMatrix reverse() {
        // Creamos la matriz inversa
        int filas = matrix.length;
        int columnas = matrix[0].length;
        int[][] reversed = new int[filas][columnas];

        // Recorremos la matriz original guardando los valores invertidos en la nueva matriz
        for(int i = 0; i < filas; i++) {
            for(int j = 0; j < columnas; j++)
                reversed[i][j] = matrix[i][columnas - j - 1];
        }

        // Devuelve la matriz inversa
        return new ImmutableMatrix(reversed);
    }


    // transpose: Devuelve la matriz traspuesta
    public ImmutableMatrix transpose() {
        // Creamos la matriz traspuesta
        int filas = matrix.length;
        int columnas = matrix[0].length;

        int[][] transposed = new int[columnas][filas];

        // Guardamos las filas originales como columnas en la nueva matriz y las columnas originales como filas traspuestas
        for(int i = 0; i < filas; i++) {
            for(int j = 0; j < columnas; j++)
                transposed[j][i] = matrix[i][j];
        }

        // Devolvemos la matriz traspuesta
        return new ImmutableMatrix(transposed);
    }


    // reshape: Devuelve otra matriz reorganizada con el numero de columnas indicado
    public ImmutableMatrix reshape(int newColumns) {
        int filas = matrix.length;
        int columnas = matrix[0].length;

        // Si no se cumple la condicion de matriz regular lanza una excepcion
        if ((filas * columnas % newColumns) != 0)
            throw new IllegalArgumentException("No es posible reorganizar la matriz.\n");

        // Calculamos el nuevo numero de filas y creamos la nueva matriz
        int newFilas = (filas * columnas) / newColumns;
        int[][] reshaped = new int[newFilas][newColumns];

        // Recorremos la matriz original guardando sus mismos datos en la posicion correcta de la nueva matriz
        int index = 0;
        for (int i = 0; i < newFilas; i++) {
            for (int j = 0; j < newColumns; j++) {
                reshaped[i][j] = matrix[index / columnas][index % columnas];
                index++;
            }
        }

        // Devolvemos la nueva matriz
        return new ImmutableMatrix(reshaped);
    }

}
