package e3.Comparadores;

import e3.Artist;

import java.util.Comparator;

public class ComparadorEclecticismo implements Comparator<Artist> {
    @Override
    public int compare(Artist artist1, Artist artist2) {
        int generos1 = artist1.getGenerosMusicales().size();
        int generos2 = artist2.getGenerosMusicales().size();

        return Integer.compare(generos2, generos1);
    }
}
