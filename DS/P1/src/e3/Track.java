package e3;

import java.util.Objects;

public record Track(String side, String rec, String artist, String title, int duration) {

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Track track = (Track) o;
        return Objects.equals(rec, track.rec); // Dos pistas son iguales si lo son sus IDs de grabaciones
    }

    @Override
    public int hashCode() {
        return Objects.hash(rec);
    }

    @Override
    public String toString() {
        return "Track{" +
                "side='" + side + '\'' +
                ", rec='" + rec + '\'' +
                ", artist='" + artist + '\'' +
                ", title='" + title + '\'' +
                ", duration=" + duration +
                '}';
    }
}