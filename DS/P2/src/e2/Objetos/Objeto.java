package e2.Objetos;

public abstract class Objeto {
    private final String codename;

    public Objeto(String codename) {
        this.codename = codename;
    }

    public String getCodename() {
        return codename;
    }
}
