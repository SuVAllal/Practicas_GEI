package e1.Estados;

import e1.Habitacion;

import java.util.Objects;

public class PendAprob implements EstadoHabitacion {
    private static final PendAprob instancia = new PendAprob();
    private PendAprob() { }
    public static PendAprob getInstance() { return  instancia; }

    @Override
    public void anadirHabitacion(Habitacion habitacion, String supervisor) {
        // La habitacion ya existe, no se realiza ninguna accion
    }

    @Override
    public void reservarHabitacion(Habitacion habitacion, String cliente) {
        // No se puede reservar, esta pendiente de aprobacion
        throw new IllegalStateException("La habitacion " + habitacion.getNumHabitacion() + " no se puede reservar, esta pendiente de aprobacion");
    }

    @Override
    public void terminarReserva(Habitacion habitacion) {
        // Como no se puede reservar, tampoco se puede terminar la reserva
        throw new IllegalStateException("La habitacion " + habitacion.getNumHabitacion() + " no esta aprobada, no se puede reservar");
    }

    @Override
    public void limpiarHabitacion(Habitacion habitacion, String limpiador) {
        // La habitacion ya esta limpia, solamente falta aprobarla
        throw new IllegalStateException("La habitacion " + habitacion.getNumHabitacion() + " ya esta limpia, es necesario aprobarla");
    }

    @Override
    public void liberarHabitacion(Habitacion habitacion) {
        // Como no esta aprobada, no puede estar ocupada
        throw new IllegalStateException("La habitacion " + habitacion.getNumHabitacion() + " ya esta liberada, es necesario aprobarla");
    }

    @Override
    public void aprobarLimpieza(Habitacion habitacion, String supervisor) {
        if(Objects.equals(habitacion.getSupervisor(), supervisor)) {
            habitacion.setEstado(Aprobada.getInstance());
            habitacion.setLimpiador(null);
            System.out.println("La habitacion " + habitacion.getNumHabitacion() + " ha sido aprobada por " + habitacion.getSupervisor());
        } else
            throw new IllegalArgumentException("El supervisor dado no esta asignado a la habitacion");
    }

    @Override
    public String mostrarInformacion(Habitacion habitacion) {
        return "Free. Room cleaned by " + habitacion.getLimpiador() + ", pending approval.";
    }
}
