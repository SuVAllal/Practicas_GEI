package e1;

public class StringUtilities {

    // isValidString: Determina si un String que se pasa como parametro es valido
    public static boolean isValidString(String input, String allowedInput, int size) {

        if(input == null || input.isEmpty()) // Si la entrada es nula o esta vacia devuelve falso
            return false;

        for(char c : input.toCharArray()) { // Recorremos la entrada caracter a caracter
            if(!Character.isDigit(c) && allowedInput.indexOf(c) == -1) // Si los caracteres de entrada no son digitos ni son validos devuelve falso
                return false;
        }

        return input.length() >= size; // Comprobamos la condicion de la longitud y finalmente devolvemos si se cumple o no
    }

    // lowercaseFirst: Reordena los caracteres de un String de forma que las minusculas aparezcan primero
    public static String lowercaseFirst(String input) {
        StringBuilder result = new StringBuilder(); // Creamos el StringBuilder para la salida

        for(char c : input.toCharArray()) { // Recorremos la entrada caracter a caracter
            if(Character.isLowerCase(c)) // Incluimos al string de salida primero las minusculas
                result.append(c);
        }

        // Y despues las mayusculas:
        for(char c : input.toCharArray()) {
            if(Character.isUpperCase(c))
                result.append(c);
        }

        // Devolvemos el StringBuilder convertido a String
        return result.toString();
    }

    // Valida si un texto es valido segun un conjunto de requisitos
    public static boolean checkTextStats(String input, int min, int max) {
        if(input == null || input.isEmpty() || max <= 0 || min <= 0) // Si la entrada es nula, vacia o los valores max y min son negativos lanza una excepcion
            throw new IllegalArgumentException("El texto no debe estar vacio.\n");

        String[] palabras = input.split(" "); // Guardamos la entrada separada por espacios en un array
        int palabraLength = 0; // Variable para guardar la longitud de cada palabra
        int maxLength = 0; // Variable para guardar la longitud de la palabra mas larga

        for(String palabra : palabras) { // Recorremos el array de palabras
            palabraLength += palabra.length(); // Longitud de la palabra actual

            // Comparamos la palabra actual con la palabra de mayor longitud hasta el momento
            if(palabra.length() > maxLength) // Si la palabra actual es mayor, se guarda como la nueva longitud maxima
                maxLength = palabra.length();
        }

        float media = (float)palabraLength / palabras.length; // Calculamos la longitud media de las palabras

        return ((media <= max) && (media >= min) && maxLength < 2*media); // Devuelve true si cumple todas las condiciones
    }
}
