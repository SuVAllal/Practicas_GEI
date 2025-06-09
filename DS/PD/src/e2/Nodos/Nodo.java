package e2.Nodos;

import e2.Flota;

public abstract class Nodo {
    private final String nombreNodo;

    public Nodo(String nombreNodo) {
        this.nombreNodo = nombreNodo;
    }

    public String getNombreNodo() {
        return nombreNodo;
    }

    public abstract Nodo incursion(Flota flota);

    public abstract int minNodos();

    public abstract String mapaNodo();
}
