package e1;

import e1.Cliente.ClienteEstandar;
import e1.Cliente.ClientePreferente;
import e1.Cliente.ClienteVIP;
import e1.Cuenta.CuentaCorriente;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class CuentaCorrienteTest {
    ClienteEstandar clienteEstandar;
    ClientePreferente clientePreferente;
    ClienteVIP clienteVIP;

    CuentaCorriente cuentaCorrienteEstandar;
    CuentaCorriente cuentaCorrientePreferente;
    CuentaCorriente cuentaCorrienteVIP;

    @BeforeEach
    void setUp() {
        clienteEstandar = new ClienteEstandar("12345678N", 100000, 0, 0.04, 300);
        clientePreferente = new ClientePreferente("98765432A", 50000, -100000, 0.02, 100);
        clienteVIP = new ClienteVIP("84071013L", 0, -1000000000, 0, 0);

        cuentaCorrienteEstandar = new CuentaCorriente("ES123456789", 0, clienteEstandar);
        cuentaCorrientePreferente = new CuentaCorriente("ES987654321", 0, clientePreferente);
        cuentaCorrienteVIP = new CuentaCorriente("ES234567891", 0, clienteVIP);
    }

    @Test
    void ingresarDinero() {
        // Cliente estandar:
        cuentaCorrienteEstandar.ingresarDinero(50000, clienteEstandar);
        assertEquals(50000, cuentaCorrienteEstandar.consultarSaldo());
        // Ingresar una cantidad negativa
        assertThrows(IllegalArgumentException.class, () -> cuentaCorrienteEstandar.ingresarDinero(-40000, clienteEstandar));


        // Cliente preferente:
        cuentaCorrientePreferente.ingresarDinero(20000, clientePreferente);
        assertEquals(20000, cuentaCorrientePreferente.consultarSaldo());
        // Ingresar una cantidad negativa
        assertThrows(IllegalArgumentException.class, () -> cuentaCorrientePreferente.ingresarDinero(-40000, clientePreferente));


        // Cliente VIP:
        cuentaCorrienteVIP.ingresarDinero(40000, clienteVIP);
        assertEquals(40000, cuentaCorrienteVIP.consultarSaldo());
        // Ingresar una cantidad negativa
        assertThrows(IllegalArgumentException.class, () -> cuentaCorrienteVIP.ingresarDinero(-40000, clienteVIP));

    }

    @Test
    void retirarDinero() {

        // Cliente estandar:

        cuentaCorrienteEstandar.ingresarDinero(50000, clienteEstandar);
        // Retiro normal
        cuentaCorrienteEstandar.retirarDinero(20000, clienteEstandar);
        assertEquals(30000, cuentaCorrienteEstandar.consultarSaldo());
        // Intentar retirar una cantidad no permitida
        assertThrows(IllegalArgumentException.class, () -> cuentaCorrienteEstandar.retirarDinero(40000, clienteEstandar));
        // Vaciar la cuenta
        cuentaCorrienteEstandar.retirarDinero(30000, clienteEstandar);
        assertEquals(0, cuentaCorrienteEstandar.consultarSaldo());
        // Intentar retirar una cantidad negativa:
        assertThrows(IllegalArgumentException.class, () -> cuentaCorrienteEstandar.retirarDinero(-40000, clienteEstandar));


        // Cliente preferente:

        cuentaCorrientePreferente.ingresarDinero(200000, clientePreferente);
        // Retiro normal
        cuentaCorrientePreferente.retirarDinero(100000, clientePreferente);
        assertEquals(100000, cuentaCorrientePreferente.consultarSaldo());
        // Vaciar la cuenta
        cuentaCorrientePreferente.retirarDinero(100000, clientePreferente);
        assertEquals(0, cuentaCorrientePreferente.consultarSaldo());
        // Alcanzar la deuda maxima
        cuentaCorrientePreferente.retirarDinero(100000, clientePreferente);
        assertEquals(-100000, cuentaCorrientePreferente.consultarSaldo());
        // Superar la deuda maxima
        assertThrows(IllegalArgumentException.class, () -> cuentaCorrientePreferente.retirarDinero(400000, clientePreferente));
        // Intentar retirar una cantidad negativa:
        assertThrows(IllegalArgumentException.class, () -> cuentaCorrientePreferente.retirarDinero(-400000, clientePreferente));


        // Cliente VIP:

        cuentaCorrienteVIP.ingresarDinero(40000, clienteVIP);
        // Retiro normal
        cuentaCorrienteVIP.retirarDinero(30000, clienteVIP);
        assertEquals(10000, cuentaCorrienteVIP.consultarSaldo());
        // Vaciar la cuenta:
        cuentaCorrienteVIP.retirarDinero(10000, clienteVIP);
        assertEquals(0, cuentaCorrienteVIP.consultarSaldo());
        // Alcanzar deuda:
        cuentaCorrienteVIP.retirarDinero(9000000, clienteVIP);
        assertEquals(-9000000, cuentaCorrienteVIP.consultarSaldo());
        // Intentar retirar una cantidad negativa:
        assertThrows(IllegalArgumentException.class, () -> cuentaCorrienteVIP.retirarDinero(-400000, clienteVIP));
    }
}