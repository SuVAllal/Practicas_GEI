package e2.Nodos.Bifurcacion;

import e2.Flota;
import e2.Nodos.Nodo;

public abstract class NodoBifurcacion extends Nodo {
    private final Nodo hijoIzq;
    private final Nodo hijoDer;
    public NodoBifurcacion(String nombreNodo, Nodo hijoIzq, Nodo hijoDer) {
        super(nombreNodo);
        this.hijoIzq = hijoIzq;
        this.hijoDer = hijoDer;
    }

    public Nodo getHijoIzq() {
        return this.hijoIzq;
    }

    public Nodo getHijoDer() {
        return this.hijoDer;
    }

    public abstract Nodo incursion(Flota flota);

    public abstract int minNodos();

    public abstract String mapaNodo();

}
