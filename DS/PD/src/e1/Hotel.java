package e1;

import e1.Estados.Aprobada;
import e1.Estados.PendAprob;
import e1.Estados.PendLimpieza;

import java.util.ArrayList;

public class Hotel {
    private final String nombreHotel;

    private final ArrayList<Habitacion> habitaciones = new ArrayList<>();

    public Hotel(String nombreHotel) {
        this.nombreHotel = nombreHotel;
    }

    public ArrayList<Habitacion> getHabitaciones() {
        return habitaciones;
    }

    public void anadirHabitacion(String supervisor) {
        Habitacion habitacion = new Habitacion(habitaciones.size() + 1, supervisor);
        habitacion.getEstado().anadirHabitacion(habitacion, supervisor);
        habitaciones.add(habitacion);
    }

    public void reservarHabitacion(int numHabitacion, String cliente) {
        if(habitaciones.size() < numHabitacion - 1)
            throw new IllegalArgumentException("El numero de habitacion no es valido\n");
        else {
            Habitacion habitacion = habitaciones.get(numHabitacion-1);
            habitacion.getEstado().reservarHabitacion(habitacion, cliente);
        }
    }

    public void terminarReserva(int numHabitacion) {
        if(habitaciones.size() < numHabitacion - 1)
            throw new IllegalArgumentException("El numero de habitacion no es valido\n");
        else {
            Habitacion habitacion = habitaciones.get(numHabitacion-1);
            habitacion.getEstado().terminarReserva(habitacion);
        }
    }

    public void limpiarHabitacion(int numHabitacion, String limpiador) {
        if(habitaciones.size() < numHabitacion - 1)
            throw new IllegalArgumentException("El numero de habitacion no es valido\n");
        else {
            Habitacion habitacion = habitaciones.get(numHabitacion-1);
            habitacion.getEstado().limpiarHabitacion(habitacion, limpiador);
        }
    }

    public void liberarHabitacion(int numHabitacion) {
        if(habitaciones.size() < numHabitacion - 1)
            throw new IllegalArgumentException("El numero de habitacion no es valido\n");
        else {
            Habitacion habitacion = habitaciones.get(numHabitacion - 1);
            habitacion.getEstado().liberarHabitacion(habitacion);
        }
    }

    public void aprobarLimpieza(int numHabitacion, String supervisor) {
        if(habitaciones.size() < numHabitacion - 1)
            throw new IllegalArgumentException("El numero de habitacion no es valido\n");
        else {
            Habitacion habitacion = habitaciones.get(numHabitacion - 1);
            habitacion.getEstado().aprobarLimpieza(habitacion, supervisor);
        }
    }

    public ArrayList<Habitacion> getHabitacionesDisp() {
        ArrayList<Habitacion> habitacionesDisp = new ArrayList<>();

        for(Habitacion h : habitaciones) {
            if(h.getEstado() instanceof Aprobada)
                habitacionesDisp.add(h);
        }

        return habitacionesDisp;
    }

    public ArrayList<Habitacion> getHabitacionesPendLimp() {
        ArrayList<Habitacion> habitacionesPendLimp = new ArrayList<>();

        for(Habitacion h : habitaciones) {
            if(h.getEstado() instanceof PendLimpieza)
                habitacionesPendLimp.add(h);
        }

        return habitacionesPendLimp;
    }

    public ArrayList<Habitacion> getHabitacionesPendAprob() {
        ArrayList<Habitacion> habitacionesPendAprob = new ArrayList<>();

        for(Habitacion h : habitaciones) {
            if(h.getEstado() instanceof PendAprob)
                habitacionesPendAprob.add(h);
        }

        return habitacionesPendAprob;
    }

    public String mostrarInformacion() {
        StringBuilder salida = new StringBuilder();

        salida.append("***********************\n");
        salida.append(nombreHotel);
        salida.append("\n***********************\n");

        for(Habitacion h : habitaciones)
            salida.append("Room no. ").append(h.getNumHabitacion()).append(": ").
                    append(h.getEstado().mostrarInformacion(h)).append("\n");

        salida.append("***********************\n");

        return salida.toString();
    }
}
