package e1;

import e1.Cliente.ClienteEstandar;
import e1.Cliente.ClientePreferente;
import e1.Cliente.ClienteVIP;
import e1.Cuenta.CuentaPlazo;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

class CuentaPlazoTest {
    ClienteEstandar clienteEstandar;
    ClientePreferente clientePreferente;
    ClienteVIP clienteVIP;

    CuentaPlazo cuentaPlazoEstandar;
    CuentaPlazo cuentaPlazoPreferente;
    CuentaPlazo cuentaPlazoVIP;

    @BeforeEach
    void setUp() {
        clienteEstandar = new ClienteEstandar("12345678N", 100000, 0, 0.04, 300);
        clientePreferente = new ClientePreferente("98765432A", 50000, -100000, 0.02, 100);
        clienteVIP = new ClienteVIP("84071013L", 0, -1000000000, 0, 0);

        cuentaPlazoEstandar = new CuentaPlazo("ES123456789", 0, clienteEstandar);
        cuentaPlazoPreferente = new CuentaPlazo("ES987654321", 0, clientePreferente);
        cuentaPlazoVIP = new CuentaPlazo("ES234567891", 0, clienteVIP);
    }

    @Test
    void ingresarDinero() {
        // Cliente estandar:

        // Ingreso normal:
        cuentaPlazoEstandar.ingresarDinero(500000, clienteEstandar);
        assertEquals(500000, cuentaPlazoEstandar.consultarSaldo());
        // Ingreso por debajo del minimo:
        assertThrows(IllegalArgumentException.class, () -> cuentaPlazoEstandar.ingresarDinero(50000, clienteEstandar));
        // Ingreso negativo
        assertThrows(IllegalArgumentException.class, () -> cuentaPlazoEstandar.ingresarDinero(-50000, clienteEstandar));


        // Cliente preferente:
        cuentaPlazoPreferente.ingresarDinero(60000, clientePreferente);
        assertEquals(60000, cuentaPlazoPreferente.consultarSaldo());
        // Ingreso por debajo del minimo:
        assertThrows(IllegalArgumentException.class, () -> cuentaPlazoPreferente.ingresarDinero(1000, clientePreferente));
        // Ingreso negativo
        assertThrows(IllegalArgumentException.class, () -> cuentaPlazoPreferente.ingresarDinero(-1000, clientePreferente));


        // Cliente VIP:
        cuentaPlazoVIP.ingresarDinero(500000, clienteVIP);
        assertEquals(500000, cuentaPlazoVIP.consultarSaldo());
        // Ingreso negativo
        assertThrows(IllegalArgumentException.class, () -> cuentaPlazoVIP.ingresarDinero(-1000, clienteVIP));

    }

    @Test
    void retirarDinero() {
        // Cliente estandar:

        cuentaPlazoEstandar.ingresarDinero(100000, clienteEstandar);
        // Retiro normal
        cuentaPlazoEstandar.retirarDinero(1000, clienteEstandar);
        assertEquals(98700, cuentaPlazoEstandar.consultarSaldo());
        // Intentar retirar una cantidad no permitida
        assertThrows(IllegalArgumentException.class, () -> cuentaPlazoEstandar.retirarDinero(500000, clienteEstandar));
        // Intentar retirar una cantidad negativa:
        assertThrows(IllegalArgumentException.class, () -> cuentaPlazoEstandar.retirarDinero(-500000, clienteEstandar));


        // Cliente preferente:

        cuentaPlazoPreferente.ingresarDinero(100000, clientePreferente);
        // Retiro normal
        cuentaPlazoPreferente.retirarDinero(1000, clientePreferente);
        assertEquals(98900, cuentaPlazoPreferente.consultarSaldo());
        // Superar la deuda maxima
        assertThrows(IllegalArgumentException.class, () -> cuentaPlazoPreferente.retirarDinero(400000, clientePreferente));
        // Intentar retirar una cantidad negativa:
        assertThrows(IllegalArgumentException.class, () -> cuentaPlazoPreferente.retirarDinero(-400000, clientePreferente));


        // Cliente VIP:

        cuentaPlazoVIP.ingresarDinero(40000, clienteVIP);
        // Retiro normal
        cuentaPlazoVIP.retirarDinero(30000, clienteVIP);
        assertEquals(10000, cuentaPlazoVIP.consultarSaldo());
        // Vaciar la cuenta:
        cuentaPlazoVIP.retirarDinero(10000, clienteVIP);
        assertEquals(0, cuentaPlazoVIP.consultarSaldo());
        // Alcanzar deuda:
        cuentaPlazoVIP.retirarDinero(9000000, clienteVIP);
        assertEquals(-9000000, cuentaPlazoVIP.consultarSaldo());
        // Intentar retirar una cantidad negativa:
        assertThrows(IllegalArgumentException.class, () -> cuentaPlazoVIP.retirarDinero(-400000, clienteVIP));
    }
}