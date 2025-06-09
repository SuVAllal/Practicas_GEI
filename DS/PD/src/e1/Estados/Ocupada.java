package e1.Estados;

import e1.Habitacion;

public class Ocupada implements EstadoHabitacion {
    private static final Ocupada instancia = new Ocupada();
    private Ocupada() { }
    public static Ocupada getInstance() { return  instancia; }

    @Override
    public void anadirHabitacion(Habitacion habitacion, String supervisor) {
        // La habitacion ya existe, no se realiza no se realiza ninguna accion
    }

    @Override
    public void reservarHabitacion(Habitacion habitacion, String cliente) {
        // La habitacion ya esta ocupada
        throw new IllegalStateException("La habitacion " + habitacion.getNumHabitacion() + " esta ocupada");
    }

    @Override
    public void terminarReserva(Habitacion habitacion) {
        habitacion.setEstado(Aprobada.getInstance());
        habitacion.setCliente(null);
        System.out.println("La reserva de la habitacion " + habitacion.getNumHabitacion() + " ha terminado");
    }

    @Override
    public void limpiarHabitacion(Habitacion habitacion, String limpiador) {
        // No se puede limpiar una habitacion ocupada
        throw new IllegalStateException("La habitacion " + habitacion.getNumHabitacion() + " esta ocupada, no se puede limpiar");
    }

    @Override
    public void liberarHabitacion(Habitacion habitacion) {
        habitacion.setEstado(PendLimpieza.getInstance());
        habitacion.setCliente(null);
        System.out.println("La habitacion " + habitacion.getNumHabitacion() + " ha sido liberada");
    }

    @Override
    public void aprobarLimpieza(Habitacion habitacion, String supervisor) {
        // Esta ocupada, no se puede limpiar, por tanto no se puede aprobar su limpieza
        throw new IllegalStateException("La habitacion " + habitacion.getNumHabitacion() + " esta ocupada, no se ha podido limpiar");
    }

    @Override
    public String mostrarInformacion(Habitacion habitacion) {
        return "Booked by " + habitacion.getCliente() + ". Occupied.";
    }
}
