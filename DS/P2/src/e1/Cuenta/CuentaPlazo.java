package e1.Cuenta;

import e1.Cliente.Cliente;

public class CuentaPlazo extends CuentaBancaria {
    public CuentaPlazo(String IBAN, long saldo, Cliente cliente) {
        super(IBAN, saldo, cliente);
    }

    @Override
    public void ingresarDinero(long ingreso, Cliente cliente) {
        long cantidad;

        if(ingreso < cliente.getMinIngreso())
            throw new IllegalArgumentException("No se puede ingresar, la cantidad no supera el ingreso minimo\n");

        cantidad = getSaldo() + ingreso;
        setSaldo(cantidad);
    }


    @Override
    public void retirarDinero(long retiro, Cliente cliente) {
        long cantidad, penalizacion, comision;

        penalizacion = (long)(retiro * cliente.getComision());
        comision = Math.max(penalizacion, cliente.getMinComision());
        cantidad = getSaldo() - (retiro + comision);

        if(retiro < 0 || cantidad < cliente.getDeuda())
            throw new IllegalArgumentException("No se puede retirar la cantidad indicada\n");

        setSaldo(cantidad);
    }
}