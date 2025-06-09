package e2.Nodos;

import e2.Flota;

public class NodoFin extends Nodo {

    public NodoFin(String nombreNodo) {
        super(nombreNodo);
    }

    @Override
    public Nodo incursion(Flota flota) {
        return null;
    }

    @Override
    public int minNodos() {
        return 1;
    }

    @Override
    public String mapaNodo() {
        return ("(" + getNombreNodo() + " End)");
    }
}
