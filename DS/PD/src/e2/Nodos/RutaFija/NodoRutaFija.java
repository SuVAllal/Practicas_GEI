package e2.Nodos.RutaFija;

import e2.Flota;
import e2.Nodos.Nodo;

public abstract class NodoRutaFija extends Nodo {
    private final Nodo hijo;
    public NodoRutaFija(String nombreNodo, Nodo hijo) {
        super(nombreNodo);
        this.hijo = hijo;
    }

    public Nodo getHijo() {
        return hijo;
    }

    @Override
    public abstract Nodo incursion(Flota flota);

    @Override
    public abstract int minNodos();

    @Override
    public abstract String mapaNodo();

}
