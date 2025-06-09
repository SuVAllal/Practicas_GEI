package e2.Nodos.RutaFija;

import e2.Flota;
import e2.Nodos.Nodo;

public class NodoAtaqueAereo extends NodoRutaFija {
    private final int poderAereo;

    public NodoAtaqueAereo(String nombreNodo, int poderAereo, Nodo hijo) {
        super(nombreNodo, hijo);
        this.poderAereo = poderAereo;
    }

    @Override
    public Nodo incursion(Flota flota) {
        int dano = poderAereo - (2 * flota.getAntiaereos() + flota.getBlindaje());
        flota.setHP(flota.getHP() - dano);

        return getHijo();
    }

    @Override
    public int minNodos() {
        return 1 + getHijo().minNodos();
    }

    @Override
    public String mapaNodo() {
        return ("(" + getNombreNodo() + " AirRaid, " + getHijo().mapaNodo() + ")");
    }
}
