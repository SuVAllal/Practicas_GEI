package e2.Objetos;

public class Armor extends Objeto implements ObjetoDefensa {
    private final int valorDefensa;
    private final int minFuerza;

    public Armor(String codename, int valorDefensa, int minFuerza) {
        super(codename);
        this.valorDefensa = valorDefensa;
        this.minFuerza = minFuerza;
    }

    @Override
    public int getValorDefensa() {
        return valorDefensa;
    }

    @Override
    public int getFuerzaMin() {
        return minFuerza;
    }
}
