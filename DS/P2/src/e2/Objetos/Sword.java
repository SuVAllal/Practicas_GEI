package e2.Objetos;

public class Sword extends Objeto implements ObjetoDefensa, ObjetoAtaque {
    private final int valorDefensa;
    private final int minFuerza;

    private final int valorAtaque;
    private final int minMana;
    private int usos;

    public Sword(String codename, int valorAtaque, int minMana, int usos, int valorDefensa, int minFuerza) {
        super(codename);
        this.valorAtaque = valorAtaque;
        this.minMana = minMana;
        this.usos = usos;
        this.valorDefensa = valorDefensa;
        this.minFuerza = minFuerza;
    }

    @Override
    public int getValorAtaque() {
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

    @Override
    public int getValorDefensa() {
        return valorDefensa;
    }

    @Override
    public int getFuerzaMin() {
        return minFuerza;
    }
}
