package e3;

import e3.InfoArtist.TypeArtist;
import e3.InfoArtist.TypeGender;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;

public class Artist implements Comparable<Artist> {
    private final String id; // orden natural de los artistas
    private final String nombre;
    private final TypeArtist tipo;
    private final TypeGender genero;
    private Date fechaNacimiento;
    private Collection<Integer> puntuaciones = new ArrayList<>();
    private Collection<String> generosMusicales = new ArrayList<>();

    public Artist(String id, String nombre, TypeArtist tipo, TypeGender genero) {
        this.id = id;
        this.nombre = nombre;
        this.tipo = tipo;
        this.genero = genero;
    }

    public String getId() {
        return id;
    }

    public String getNombre() {
        return nombre;
    }

    public TypeArtist getTipo() {
        return tipo;
    }

    public TypeGender getGenero() {
        return genero;
    }

    public Date getFechaNacimiento() {
        return fechaNacimiento;
    }

    public void setFechaNacimiento(Date fechaNacimiento) {
        this.fechaNacimiento = fechaNacimiento;
    }

    public Collection<Integer> getPuntuaciones() {
        return puntuaciones;
    }

    public void setPuntuaciones(Collection<Integer> puntuaciones) {
        this.puntuaciones = puntuaciones;
    }

    public Collection<String> getGenerosMusicales() {
        return generosMusicales;
    }

    public void setGenerosMusicales(Collection<String> generosMusicales) {
        this.generosMusicales = generosMusicales;
    }

    @Override
    public int compareTo(Artist o) {
        return this.id.compareTo(o.id);
    }

    // Funcion auxiliar para calcular la puntuacion media del artista:
    public float puntuacionMedia() {
        if(puntuaciones.isEmpty())
            return 0;

        int suma = 0;
        for(int puntuacion : puntuaciones)
            suma += puntuacion;

        return (float)suma/puntuaciones.size();
    }
}

