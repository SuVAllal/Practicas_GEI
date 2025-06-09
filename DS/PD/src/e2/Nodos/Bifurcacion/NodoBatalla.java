package e2.Nodos.Bifurcacion;

import e2.Flota;
import e2.Nodos.Nodo;

public class NodoBatalla extends NodoBifurcacion {
    private final Flota flotaEnemiga;
    public NodoBatalla(String nombreNodo, Flota flotaEnemiga, Nodo hijoIzq, Nodo hijoDer) {
        super(nombreNodo, hijoIzq, hijoDer);
        this.flotaEnemiga = flotaEnemiga;
    }

    @Override
    public Nodo incursion(Flota flota) {
        int danoFlota = 0, danoFlotaEnemiga = 0;

        if(flotaEnemiga.getPoderFuego() > flota.getBlindaje())
            danoFlota = flotaEnemiga.getPoderFuego() - flota.getBlindaje();
        if(flota.getPoderFuego() > flotaEnemiga.getBlindaje())
            danoFlotaEnemiga = flota.getPoderFuego() - flotaEnemiga.getBlindaje();

        flota.setHP(flota.getHP() - danoFlota);
        flotaEnemiga.setHP(flotaEnemiga.getHP() - danoFlotaEnemiga);

        if(flotaEnemiga.getHP() <= 0)
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
        return ("(" + getNombreNodo() + " Battle, " + getHijoIzq().mapaNodo() + ", " + getHijoDer().mapaNodo() + ")");
    }
}
