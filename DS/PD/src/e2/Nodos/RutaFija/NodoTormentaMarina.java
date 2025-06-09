package e2.Nodos.RutaFija;

import e2.Flota;
import e2.Nodos.Nodo;

public class NodoTormentaMarina extends NodoRutaFija {
    private final int fuerzaTormenta;
    public NodoTormentaMarina(String nombreNodo, int fuerzaTormenta, Nodo hijo) {
        super(nombreNodo, hijo);
        this.fuerzaTormenta = fuerzaTormenta;
    }

    @Override
    public Nodo incursion(Flota flota) {
        if(flota.getLineaVision() < fuerzaTormenta)
            flota.setHP(flota.getHP() - 10);

        return getHijo();
    }

    @Override
    public int minNodos() {
        return 1 + getHijo().minNodos();
    }

    @Override
    public String mapaNodo() {
        return ("(" + getNombreNodo() + " Maelstrom, " + getHijo().mapaNodo() + ")");
    }
}
