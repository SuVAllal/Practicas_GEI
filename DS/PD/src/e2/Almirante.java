package e2;

import e2.Nodos.Nodo;


public class Almirante {
    public String simularIncursion(Nodo mapa, Flota flota) {
        StringBuilder salida = new StringBuilder();
        Nodo nodoActual = mapa;
        Nodo nodoAnterior = null;
        boolean exitoIncursion = false;

        while(nodoActual != null && flota.getHP() > 0) {
            nodoAnterior = nodoActual;
            nodoActual = nodoActual.incursion(flota);
        }

        if(flota.getHP() > 0)
            exitoIncursion = true;

        salida.append("Sortie Result:\n")
                .append(exitoIncursion ? "SUCCESS\n":"FAIL\n")
                .append("Last Visited Node: ").append(nodoAnterior != null ? nodoAnterior.getNombreNodo() : "None")
                .append("\nFinal HP: ").append(flota.getHP());

        return salida.toString();
    }

    public String minimoNodos(Nodo mapa) {
        return "Smallest Node Count to End: " + mapa.minNodos();
    }

    public String mapaTexto(Nodo mapa) {
        return mapa.mapaNodo();
    }
}
