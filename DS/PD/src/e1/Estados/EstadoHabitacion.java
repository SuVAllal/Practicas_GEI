package e1.Estados;

import e1.Habitacion;

public interface EstadoHabitacion {
    void anadirHabitacion(Habitacion habitacion, String supervisor);
    void reservarHabitacion(Habitacion habitacion, String cliente);
    void terminarReserva(Habitacion habitacion);
    void limpiarHabitacion(Habitacion habitacion, String limpiador);
    void liberarHabitacion(Habitacion habitacion);
    void aprobarLimpieza(Habitacion habitacion, String supervisor);
    String mostrarInformacion(Habitacion habitacion);
}
