package e1.Cuenta;

import e1.Cliente.Cliente;

public abstract class CuentaBancaria {
    private final String IBAN;
    private long saldo;
    private final Cliente cliente;

    public CuentaBancaria(String IBAN, long saldo, Cliente cliente) {
        this.IBAN = IBAN;
        this.saldo = saldo;
        this.cliente = cliente;
    }

    public long getSaldo() {
        return saldo;
    }

    public void setSaldo(long saldo) {
        this.saldo = saldo;
    }

    public long consultarSaldo() {
        return saldo;
    }

    public abstract void ingresarDinero(long ingreso, Cliente cliente);
    public abstract void retirarDinero(long retiro, Cliente cliente);

}

