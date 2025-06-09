package e2.Nodos.Bifurcacion;

import e2.Flota;
import e2.Nodos.Nodo;

public class NodoAvistamiento extends NodoBifurcacion {
    private final int distancia;
    public NodoAvistamiento(String nombreNodo, int distancia, Nodo hijoIzq, Nodo hijoDer) {
        super(nombreNodo, hijoIzq, hijoDer);
        this.distancia = distancia;
    }

    @Override
    public Nodo incursion(Flota flota) {
        if(flota.getLineaVision() >= distancia)
            return getHijoIzq();
        else
            return getHijoDer();
    }

    @Override
    public int minNodos() {
        return 1 + Math.min(getHijoIzq().minNodos(), getHijoDer().minNodos());
    }

    @Override
    public String mapaNodo() {
        return ("(" + getNombreNodo() + " WaypointSpotting, " + getHijoIzq().mapaNodo() + ", " + getHijoDer().mapaNodo() + ")");
    }
}
