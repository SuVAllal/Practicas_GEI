package e3.Comparadores;

import e3.Artist;

import java.util.Comparator;

public class ComparadorNombre implements Comparator<Artist> {
    @Override
    public int compare(Artist artist1, Artist artist2) {
        return artist1.getNombre().compareTo(artist2.getNombre());
    }
}
