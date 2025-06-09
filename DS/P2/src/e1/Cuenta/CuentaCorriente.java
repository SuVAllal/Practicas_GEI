package e1.Cuenta;

import e1.Cliente.Cliente;

public class CuentaCorriente extends CuentaBancaria {
    public CuentaCorriente(String IBAN, long saldo, Cliente cliente) {
        super(IBAN, saldo, cliente);
    }

    @Override
    public void ingresarDinero(long ingreso, Cliente cliente) {
        if(ingreso < 0)
            throw new IllegalArgumentException("No se puede ingresar la cantidad indicada\n");

        long cantidad = getSaldo() + ingreso;
        setSaldo(cantidad);
    }

    @Override
    public void retirarDinero(long retiro, Cliente cliente) {
        long cantidad = getSaldo() - retiro;

        if(cantidad < cliente.getDeuda() || retiro < 0)
            throw new IllegalArgumentException("No se puede retirar la cantidad indicada\n");

        setSaldo(cantidad);
    }

}