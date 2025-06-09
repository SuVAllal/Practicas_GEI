package e1;

import e1.Estados.Aprobada;
import e1.Estados.Ocupada;
import e1.Estados.PendAprob;
import e1.Estados.PendLimpieza;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;

import static org.junit.jupiter.api.Assertions.*;

class HotelTest {

    public static Hotel hotel = new Hotel("Mojo Dojo Casa House");

    @BeforeAll
    static void setUp() {
        hotel.anadirHabitacion("Juanito");
        hotel.anadirHabitacion("Manuel");
        hotel.anadirHabitacion("Maria");
        hotel.anadirHabitacion("Juanito");
        hotel.anadirHabitacion("Maria");
    }

    @Test
    void anadirHabitacion() {
        // Creamos una nueva habitacion en el hotel y comprobamos que sus datos son correctos:
        hotel.anadirHabitacion("Manuel");
        assertEquals(6, hotel.getHabitaciones().get(5).getNumHabitacion());
        assertEquals("Manuel", hotel.getHabitaciones().get(5).getSupervisor());
        assertEquals(Aprobada.getInstance(), hotel.getHabitaciones().get(5).getEstado());

    }

    @Test
    void reservarHabitacion() {
        // Reservamos una habitacion disponible:
        hotel.reservarHabitacion(3, "Fulanita");

        // Verificamos que los datos de la habitacion reservada se han actualizado:
        assertEquals("Fulanita", hotel.getHabitaciones().get(2).getCliente());
        assertEquals(Ocupada.getInstance(), hotel.getHabitaciones().get(2).getEstado());

        // Caso de querer reservar una habitacion no existente:
        assertThrows(IllegalArgumentException.class, () -> hotel.reservarHabitacion(9, "Patricia"));

        // Caso de querer reservar una habitacion que ya esta ocupada:
        assertThrows(IllegalStateException.class, () -> hotel.reservarHabitacion(3, "Sofia"));

        // Caso de querer reservar una habitacion pendiente de limpieza:
        hotel.liberarHabitacion(3);
        assertThrows(IllegalStateException.class, () -> hotel.reservarHabitacion(3, "Adrian"));

        // Caso de querer reservar una habitacion pendiente de aprobacion:
        hotel.limpiarHabitacion(3, "Pepe");
        assertThrows(IllegalStateException.class, () -> hotel.reservarHabitacion(3, "Lucia"));

        // Devolvemos la habitacion a su estado inicial:
        hotel.aprobarLimpieza(3, "Maria");
    }

    @Test
    void terminarReserva() {
        // Reservamos una habitacion:
        hotel.reservarHabitacion(2, "Francisco");

        // Terminamos la reserva, por lo que su estado es "Aprobada"
        hotel.terminarReserva(2);
        assertNull(hotel.getHabitaciones().get(1).getCliente());
        assertEquals(Aprobada.getInstance(), hotel.getHabitaciones().get(1).getEstado());

        // Caso de querer terminar la reserva de una habitacion no existente:
        assertThrows(IllegalArgumentException.class, () -> hotel.terminarReserva(9));

        // En caso de terminar la reserva de una habitacion vacia:
        assertThrows(IllegalStateException.class, () -> hotel.terminarReserva(2));

        // En caso de terminar la reserva de una habitacion pendiente de limpieza:
        hotel.reservarHabitacion(4, "Claudia");
        hotel.liberarHabitacion(4);
        assertThrows(IllegalStateException.class, () -> hotel.terminarReserva(4));

        // En caso de terminar la reserva de una habitacion pendiente de aprobacion:
        hotel.limpiarHabitacion(4, "Pepe");
        assertThrows(IllegalStateException.class, () -> hotel.terminarReserva(4));

        // Devolvemos la habitacion a su estado inicial
        hotel.aprobarLimpieza(4, "Juanito");
    }

    @Test
    void limpiarHabitacion() {
        // Caso de querer limpiar una habitacion aprobada
        assertThrows(IllegalStateException.class, () -> hotel.limpiarHabitacion(2, "Paula"));

        // Realizamos el proceso para conseguir una habitacion pendiente de limpieza:

        // Caso de querer limpiar una habitacion ocupada:
        hotel.reservarHabitacion(2, "Elley");
        assertThrows(IllegalStateException.class, () -> hotel.limpiarHabitacion(2, "Paula"));

        hotel.liberarHabitacion(2);
        // Limpiamos la habitacion y verificamos que se ha limpiado bien:
        hotel.limpiarHabitacion(2, "Pepe");
        assertEquals("Pepe", hotel.getHabitaciones().get(1).getLimpiador());
        assertEquals(PendAprob.getInstance(), hotel.getHabitaciones().get(1).getEstado());

        // Caso de querer limpiar una habitacion no existente:
        assertThrows(IllegalArgumentException.class, () -> hotel.limpiarHabitacion(9, "Pepe"));

        // En caso de querer limpiar una habitacion pendiente de aprobacion:
        assertThrows(IllegalStateException.class, () -> hotel.limpiarHabitacion(2, "Paula"));

        // Devolvemos la habitacion a su estado inicial:
        hotel.aprobarLimpieza(2, "Manuel");

    }

    @Test
    void liberarHabitacion() {
        // Caso de intentar liberar una habitacion aprobada:
        assertThrows(IllegalStateException.class, () -> hotel.liberarHabitacion(4));

        // Realizamos el proceso para obtener una habitacion ocupada:
        hotel.reservarHabitacion(4, "Francisca");
        // Liberamos la habitacion, por lo que su estado es "PendLimpieza":
        hotel.liberarHabitacion(4);
        assertNull(hotel.getHabitaciones().get(3).getCliente());
        assertNull(hotel.getHabitaciones().get(3).getLimpiador());
        assertEquals(PendLimpieza.getInstance(), hotel.getHabitaciones().get(3).getEstado());

        // Caso de querer liberar una habitacion no existente:
        assertThrows(IllegalArgumentException.class, () -> hotel.liberarHabitacion(9));

        // Caso de intentar liberar una habitacion PendLimpieza:
        assertThrows(IllegalStateException.class, () -> hotel.liberarHabitacion(4));

        // Caso de intentar liberar una habitacion PendAprob:
        hotel.limpiarHabitacion(4, "Paula");
        assertThrows(IllegalStateException.class, () -> hotel.liberarHabitacion(4));

        // Devolvemos la habitacion a su estado inicial
        hotel.aprobarLimpieza(4, "Juanito");
    }

    @Test
    void aprobarLimpieza() {
        // Caso de querer revocar la limpieza de una habitacion ya aprobada:
        hotel.aprobarLimpieza(4, "Juanito");
        assertEquals(PendLimpieza.getInstance(), hotel.getHabitaciones().get(3).getEstado());

        // Caso de intentar aprobar la limpieza de una habitacion Ocupada:
        hotel.reservarHabitacion(3, "Sam");
        assertThrows(IllegalStateException.class, () -> hotel.aprobarLimpieza(3, "Juanito"));

        // Caso de intentar aprobar la limpieza de una habitacion PendLimpieza:
        hotel.liberarHabitacion(3);
        assertThrows(IllegalStateException.class, () -> hotel.aprobarLimpieza(3, "Juanito"));

        // Verificamos el funcionamiento de aprobarLimpieza:
        hotel.limpiarHabitacion(3, "Paula");
        // Caso de que un supervisor apruebe una habitacion que no asigno el:
        assertThrows(IllegalArgumentException.class, () -> hotel.aprobarLimpieza(3, "Manuel"));
        // Caso de que el supervisor sea correcto:
        hotel.aprobarLimpieza(3, "Maria");
        assertNull(hotel.getHabitaciones().get(2).getLimpiador());
        assertEquals(Aprobada.getInstance(), hotel.getHabitaciones().get(2).getEstado());

        // Caso de querer aprobar una habitacion no existente:
        assertThrows(IllegalArgumentException.class, () -> hotel.aprobarLimpieza(9, "Maria"));

        // Devolvemos la habitacion a su estado inicial:
        hotel.limpiarHabitacion(4, "Pepe");
        hotel.aprobarLimpieza(4, "Juanito");
    }

    @Test
    void getHabitacionesDisp() {
        // Creamos una lista a comparar:
        ArrayList<Habitacion> prueba = new ArrayList<>(hotel.getHabitaciones());

        // Primer caso: todas las habitaciones estan disponibles
        assertEquals(5, hotel.getHabitacionesDisp().size());
        assertEquals(prueba, hotel.getHabitacionesDisp());

        // Segundo caso: hay habitaciones no disponibles
        hotel.reservarHabitacion(3, "Floriana");
        prueba.remove(2);
        assertEquals(4, hotel.getHabitacionesDisp().size());
        assertEquals(prueba, hotel.getHabitacionesDisp());

        // Devolvemos la habitacion a su estado original:
        hotel.terminarReserva(3);
    }

    @Test
    void getHabitacionesPendLimp() {
        // Creamos las listas a comparar:
        ArrayList<Habitacion> pruebaAprobadas = new ArrayList<>(hotel.getHabitaciones());
        ArrayList<Habitacion> pruebaPendLimp = new ArrayList<>();

        // Primer caso: la lista esta vacia, ninguna habitacion esta pendiente de limpieza
        assertEquals(0, hotel.getHabitacionesPendLimp().size());

        // Segundo caso: hay habitaciones pendientes de limpieza
        hotel.reservarHabitacion(3, "Juanita");
        hotel.liberarHabitacion(3);
        pruebaAprobadas.remove(2);
        pruebaPendLimp.add(hotel.getHabitaciones().get(2));
        assertEquals(1, hotel.getHabitacionesPendLimp().size());
        assertEquals(pruebaAprobadas, hotel.getHabitacionesDisp());
        assertEquals(pruebaPendLimp, hotel.getHabitacionesPendLimp());

        // Devolvemos la habitacion a su estado original:
        hotel.limpiarHabitacion(3, "Pepe");
        hotel.aprobarLimpieza(3, "Maria");

    }

    @Test
    void getHabitacionesPendAprob() {
        // Creamos las listas a comparar:
        ArrayList<Habitacion> pruebaAprobadas = new ArrayList<>(hotel.getHabitaciones());
        ArrayList<Habitacion> pruebaPendAprob = new ArrayList<>();

        // Primer caso: la lista esta vacia, ninguna habitacion esta pendiente de aprobacion:
        assertEquals(0, hotel.getHabitacionesPendAprob().size());

        // Segundo caso: hay habitaciones pendientes de aprobacion:
        hotel.reservarHabitacion(3, "Laura");
        hotel.liberarHabitacion(3);
        hotel.limpiarHabitacion(3, "Pepe");
        pruebaAprobadas.remove(2);
        pruebaPendAprob.add(hotel.getHabitaciones().get(2));
        assertEquals(1, hotel.getHabitacionesPendAprob().size());
        assertEquals(pruebaPendAprob, hotel.getHabitacionesPendAprob());
        assertEquals(pruebaAprobadas, hotel.getHabitacionesDisp());

        // Devolvemos la habitacion a su estado original:
        hotel.aprobarLimpieza(3, "Maria");
    }

    @Test
    void mostrarInformacion() {
        String salida = """
                ***********************
                Mojo Dojo Casa House
                ***********************
                Room no. 1: Free. This room was approved by Juanito.
                Room no. 2: Free. Room cleaned by Pepe, pending approval.
                Room no. 3: Booked by Fulanita. Occupied.
                Room no. 4: Free. This room was approved by Juanito.
                Room no. 5: Free. Cleaning pending.
                ***********************
                """;

        // Habitacion 2:
        hotel.reservarHabitacion(2, "Victoria");
        hotel.liberarHabitacion(2);
        hotel.limpiarHabitacion(2, "Pepe");

        // Habitacion 3:
        hotel.reservarHabitacion(3, "Fulanita");

        // Habitacion 5:
        hotel.reservarHabitacion(5, "Carlos");
        hotel.liberarHabitacion(5);

        // Comprobamos la salida:
        assertEquals(salida, hotel.mostrarInformacion());

        // Devolvemos las habitaciones a sus estados iniciales:
        hotel.aprobarLimpieza(2, "Manuel");
        hotel.terminarReserva(3);
        hotel.limpiarHabitacion(5, "Paula");
        hotel.aprobarLimpieza(5, "Maria");
    }
}