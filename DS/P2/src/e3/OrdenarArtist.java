package e3;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class OrdenarArtist {
    private ArrayList<Artist> artistas = new ArrayList<>();

    public ArrayList<Artist> getArtistas() {
        return artistas;
    }

    public void setArtistas(ArrayList<Artist> artistas) {
        this.artistas = artistas;
    }

    public ArrayList<Artist> ordenarArtistas() {
        Collections.sort(artistas);
        return artistas;
    }

    public ArrayList<Artist> ordenarArtistas(Comparator<Artist> artistComparator) {
        Collections.sort(artistas, artistComparator);
        return artistas;
    }
}

