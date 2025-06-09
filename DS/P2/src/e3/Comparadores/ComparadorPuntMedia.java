package e3.Comparadores;

import e3.Artist;

import java.util.Comparator;

public class ComparadorPuntMedia implements Comparator<Artist> {
    @Override
    public int compare(Artist artist1, Artist artist2) {
        float media1 = artist1.puntuacionMedia();
        float media2 = artist2.puntuacionMedia();

        return Float.compare(media1, media2);
    }
}
