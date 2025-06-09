package e1.Cliente;

public abstract class Cliente {
    private final String DNI;
    private final long minIngreso;
    private final long deuda;
    private final double comision;
    private final long minComision;

    public Cliente(String DNI, long minIngreso, long deuda, double comision, long minComision) {
        this.DNI = DNI;
        this.minIngreso = minIngreso;
        this.deuda = deuda;
        this.comision = comision;
        this.minComision = minComision;
    }

    public long getMinIngreso() {
        return minIngreso;
    }

    public long getDeuda() {
        return deuda;
    }

    public double getComision() {
        return comision;
    }

    public long getMinComision() {
        return minComision;
    }
}
