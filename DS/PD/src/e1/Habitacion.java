package e1;

import e1.Estados.Aprobada;
import e1.Estados.EstadoHabitacion;

public class Habitacion {
    private final int numHabitacion;
    private EstadoHabitacion estado = Aprobada.getInstance();
    private final String supervisor;
    private String limpiador;
    private String cliente;

    public Habitacion(int numHabitacion, String supervisor) {
        this.numHabitacion = numHabitacion;
        this.supervisor = supervisor;
    }

    public int getNumHabitacion() {
        return numHabitacion;
    }

    public EstadoHabitacion getEstado() {
        return estado;
    }

    public void setEstado(EstadoHabitacion estado) {
        this.estado = estado;
    }

    public String getSupervisor() {
        return supervisor;
    }

    public String getLimpiador() {
        return limpiador;
    }

    public void setLimpiador(String limpiador) {
        this.limpiador = limpiador;
    }

    public String getCliente() {
        return cliente;
    }

    public void setCliente(String cliente) {
        this.cliente = cliente;
    }
}