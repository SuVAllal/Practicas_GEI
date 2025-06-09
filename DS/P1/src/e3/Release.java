package e3;

import java.util.*;

public class Release {
    // ATRIBUTOS:
    private final String recID;
    private String title;
    private String mainArtist;
    private final List<Track> trackList;

    // METODOS:

    // Constructor:
    public Release(String recID) {
        if(recID == null || recID.isEmpty())
            throw new IllegalArgumentException("El ID de la grabacion no puede ser nulo o vacio.\n");

        this.recID = recID;
        this.trackList = new ArrayList<>();
    }

    public void setArtist(String mainArtist) {
        if(mainArtist == null || mainArtist.isEmpty())
            throw new IllegalArgumentException("El artista no puede ser nulo o vacio.\n");

        this.mainArtist = mainArtist;
    }

    public void setTitle(String title) {
        if(title == null || title.isEmpty())
            throw new IllegalArgumentException("El titulo no puede ser nulo o vacio.\n");

        this.title = title;
    }

    public void addTrack(Track track) {
        if(track == null)
            throw new IllegalArgumentException("El track no puede ser nulo.\n");

        trackList.add(track);
    }


    // Funcion auxiliar para comparar los IDs de las grabaciones de una pista:

    private Set<String> compareRec() {
        List<String> grabaciones = new ArrayList<>();

        for(Track track : trackList)
            grabaciones.add(track.rec());

        return new HashSet<>(grabaciones);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Release release = (Release) o;
        return Objects.equals(compareRec(), release.compareRec());
    }

    @Override
    public int hashCode() {
        return Objects.hash(compareRec());
    }

    @Override
    public String toString() {
        return "Release{" +
                "recID='" + recID + '\'' +
                ", title='" + title + '\'' +
                ", mainArtist='" + mainArtist + '\'' +
                ", trackList=" + trackList +
                '}';
    }
}
