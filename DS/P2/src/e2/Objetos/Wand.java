package e2.Objetos;

public class Wand extends Objeto implements ObjetoAtaque, ObjetoDefensa {
    private final int valorDefensa;
    private final int minFuerza;

    private final int valorAtaque;
    private final int minMana;
    private int usos;
    boolean primerUso;

    public Wand(String codename, int valorAtaque, int minMana, int usos, int valorDefensa, int minFuerza) {
        super(codename);
        this.valorAtaque = valorAtaque;
        this.minMana = minMana;
        this.usos = usos;
        this.primerUso = true;
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

    @Override
    public int getValorAtaque() {
        if(primerUso) {
            primerUso = false;
            return 2 * valorAtaque;
        } else
            return valorAtaque;
    }

    @Override
    public int getManaMin() {
        return minMana;
    }

    @Override
    public int getUsos() {
        return usos;
    }

    @Override
    public void reducirUsos() {
        if(usos > 0)
            usos--;
    }
}
