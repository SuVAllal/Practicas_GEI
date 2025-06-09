package e2.Objetos;

public class FireBallSpell extends Objeto implements ObjetoAtaque {
    private final int valorAtaque;
    private final int minMana;
    private int usos;

    public FireBallSpell(String codename, int valorAtaque, int minMana, int usos) {
        super(codename);
        this.valorAtaque = valorAtaque;
        this.minMana = minMana;
        this.usos = usos;
    }

    @Override
    public int getValorAtaque() {
        if(usos == 1)
            return valorAtaque - 1;
        else
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
