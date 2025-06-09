package e3.Comparadores;

import e3.Artist;

import java.util.Comparator;

public class ComparadorFecha implements Comparator<Artist> {

    @Override
    public int compare(Artist artist1, Artist artist2) {
        return artist2.getFechaNacimiento().compareTo(artist1.getFechaNacimiento());
    }
}
