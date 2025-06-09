package e2;

public class Flota {
    private int HP;
    private final int Blindaje;
    private final int PoderFuego;
    private final int Antiaereos;
    private final int LineaVision;

    public Flota(int HP, int blindaje, int poderFuego, int antiaereos, int lineaVision) {
        this.HP = HP;
        Blindaje = blindaje;
        PoderFuego = poderFuego;
        Antiaereos = antiaereos;
        LineaVision = lineaVision;
    }

    public int getHP() {
        return HP;
    }

    public void setHP(int HP) {
        this.HP = HP;
    }

    public int getBlindaje() {
        return Blindaje;
    }

    public int getPoderFuego() {
        return PoderFuego;
    }

    public int getAntiaereos() {
        return Antiaereos;
    }

    public int getLineaVision() {
        return LineaVision;
    }

}
