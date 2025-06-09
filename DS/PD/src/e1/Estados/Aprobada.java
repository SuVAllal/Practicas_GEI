package e1.Estados;

import e1.Habitacion;

// Aprobada = Disponible
public class Aprobada implements EstadoHabitacion {
    private static final Aprobada instancia = new Aprobada();
    private Aprobada() { }
    public static Aprobada getInstance() { return instancia; }

    @Override
    public void anadirHabitacion(Habitacion habitacion, String supervisor) {
        habitacion.setEstado(Aprobada.getInstance());
        System.out.println("Habitacion " + habitacion.getNumHabitacion() + " anadida al hotel");
    }

    @Override
    public void reservarHabitacion(Habitacion habitacion, String cliente) {
        habitacion.setEstado(Ocupada.getInstance());
        habitacion.setCliente(cliente);
        System.out.println("La habitacion " + habitacion.getNumHabitacion() + " ha sido reservada por " + cliente);
    }

    @Override
    public void terminarReserva(Habitacion habitacion) {
        // No se puede terminar la reserva sin reservarla antes
        throw new IllegalStateException("No se puede terminar la reserva, la habitacion " + habitacion.getNumHabitacion() + " no ha sido reservada inicialmente");
    }

    @Override
    public void limpiarHabitacion(Habitacion habitacion, String limpiador) {
        // La habitacion ya esta limpia
        throw new IllegalStateException("La habitacion " + habitacion.getNumHabitacion() + " ya esta limpia y disponible para reservar");
    }

    @Override
    public void liberarHabitacion(Habitacion habitacion) {
        // La habitacion ya esta limpia
        throw new IllegalStateException("La habitacion " + habitacion.getNumHabitacion() + " ya esta disponible para reservar");
    }

    @Override
    public void aprobarLimpieza(Habitacion habitacion, String supervisor) {
        habitacion.setEstado(PendLimpieza.getInstance());
        System.out.println("La limpieza ha sido revocada, el proceso de limpieza se iniciara de nuevo.");
    }

    @Override
    public String mostrarInformacion(Habitacion habitacion) {
        return "Free. This room was approved by " + habitacion.getSupervisor() + ".";
    }
}
