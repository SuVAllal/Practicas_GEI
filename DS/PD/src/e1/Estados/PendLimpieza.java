package e1.Estados;

import e1.Habitacion;

public class PendLimpieza implements EstadoHabitacion {
    private static final PendLimpieza instancia = new PendLimpieza();
    private PendLimpieza() { }
    public static PendLimpieza getInstance() { return  instancia; }

    @Override
    public void anadirHabitacion(Habitacion habitacion, String supervisor) {
        // La habitacion ya existe, no se realiza ninguna accion
    }

    @Override
    public void reservarHabitacion(Habitacion habitacion, String cliente) {
        // La habitacion no se puede reservar, no esta limpia
        throw new IllegalStateException("La habitacion " + habitacion.getNumHabitacion() + " no se puede reservar, esta pendiente de limpieza");
    }

    @Override
    public void terminarReserva(Habitacion habitacion) {
        // Como no se pudo reservar, no se puede terminar la reserva
        throw new IllegalStateException("La habitacion " + habitacion.getNumHabitacion() + " esta pendiente de limpieza");
    }

    @Override
    public void limpiarHabitacion(Habitacion habitacion, String limpiador) {
        habitacion.setEstado(PendAprob.getInstance());
        habitacion.setLimpiador(limpiador);
        System.out.println("La habitacion " + habitacion.getNumHabitacion() + " ha sido limpiada por " + limpiador);
    }

    @Override
    public void liberarHabitacion(Habitacion habitacion) {
        // Como no se puede reservar, no se puede liberar
        throw new IllegalStateException("La habitacion " + habitacion.getNumHabitacion() + " esta pendiente de limpieza");
    }

    @Override
    public void aprobarLimpieza(Habitacion habitacion, String supervisor) {
        // La habitacion aun no se ha limpiado
        throw new IllegalStateException("La habitacion " + habitacion.getNumHabitacion() + " esta pendiente de limpieza");
    }

    @Override
    public String mostrarInformacion(Habitacion habitacion) {
        return "Free. Cleaning pending.";
    }
}
