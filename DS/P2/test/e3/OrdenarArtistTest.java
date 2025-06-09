package e3;

import e3.Comparadores.*;
import e3.InfoArtist.*;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;

class OrdenarArtistTest {
    private static final Artist prueba = new Artist("0000 0004 7083 6655", "Maneskin", TypeArtist.GROUP, TypeGender.NON_BINARY);
    private static final Artist artist1 = new Artist("0000 0004 7083 6655", "Maneskin", TypeArtist.GROUP, null);
    private static final Artist artist2 = new Artist("0000 0001 2025 4524", "Miley Cyrus", TypeArtist.PERSON, TypeGender.NON_BINARY);
    private static final Artist artist3 = new Artist("0000 0001 1476 6419", "Rihanna", TypeArtist.PERSON, TypeGender.FEMALE);
    private static final Artist artist4 = new Artist("0000 0000 8416 0095", "Stromae", TypeArtist.PERSON, TypeGender.MALE);
    private static final Artist artist5 = new Artist("0000 0004 0219 6107", "Hatsune Miku", TypeArtist.OTHER, TypeGender.FEMALE);
    private static ArrayList<Artist> artistList;

    @BeforeAll
    static void setUp() throws ParseException {
        DateFormat format = new SimpleDateFormat("dd/MM/yyyy");

        Date fecha1 = format.parse("01/01/2016");
        Date fecha2 = format.parse("23/11/1992");
        Date fecha3 = format.parse("20/02/1988");
        Date fecha5 = format.parse("12/03/1985");
        Date fecha6 = format.parse("31/08/2007");
        artist1.setFechaNacimiento(fecha1);
        artist2.setFechaNacimiento(fecha2);
        artist3.setFechaNacimiento(fecha3);
        artist4.setFechaNacimiento(fecha5);
        artist5.setFechaNacimiento(fecha6);


        Collection<Integer> puntuaciones1 = new ArrayList<>(Arrays.asList(6, 3, 4, 7));
        Collection<Integer> puntuaciones2 = new ArrayList<>(Arrays.asList(8, 9, 5, 6));
        Collection<Integer> puntuaciones3 = new ArrayList<>(Arrays.asList(7, 4, 5, 8));
        Collection<Integer> puntuaciones4 = new ArrayList<>(Arrays.asList(5, 2, 3, 6));
        Collection<Integer> puntuaciones5 = new ArrayList<>(Arrays.asList(8, 7, 7, 10));
        artist1.setPuntuaciones(puntuaciones1);
        artist2.setPuntuaciones(puntuaciones2);
        artist3.setPuntuaciones(puntuaciones3);
        artist4.setPuntuaciones(puntuaciones4);
        artist5.setPuntuaciones(puntuaciones5);


        Collection<String> generosMusicales1 = new ArrayList<>(Arrays.asList("Rock", "Pop Rock", "Indie", "Pop", "Rock & Roll", "Indie Rock"));
        Collection<String> generosMusicales2 = new ArrayList<>(Arrays.asList("Pop", "Rock", "R&B", "Country", "Soul"));
        Collection<String> generosMusicales3 = new ArrayList<>(Arrays.asList("Pop", "R&B"));
        Collection<String> generosMusicales4 = new ArrayList<>(Arrays.asList("Electropop", "Pop", "Soul"));
        Collection<String> generosMusicales5 = new ArrayList<>(Arrays.asList("Vocaloid", "Pop", "Techno", "Punk"));
        artist1.setGenerosMusicales(generosMusicales1);
        artist2.setGenerosMusicales(generosMusicales2);
        artist3.setGenerosMusicales(generosMusicales3);
        artist4.setGenerosMusicales(generosMusicales4);
        artist5.setGenerosMusicales(generosMusicales5);


        artistList = new ArrayList<>(Arrays.asList(artist1, artist2, artist3, artist4, artist5));
    }

    @Test
    void testGetSet() throws ParseException {
        DateFormat format = new SimpleDateFormat("dd/MM/yyyy");
        Date fecha1 = format.parse("01/01/2016");
        prueba.setFechaNacimiento(fecha1);

        Collection<Integer> puntuaciones1 = new ArrayList<>(Arrays.asList(6, 3, 4, 7));
        prueba.setPuntuaciones(puntuaciones1);

        Collection<String> generosMusicales1 = new ArrayList<>(Arrays.asList("Rock", "Pop Rock", "Indie", "Pop", "Rock & Roll", "Indie Rock"));
        prueba.setGenerosMusicales(generosMusicales1);

        assertEquals("0000 0004 7083 6655", prueba.getId());
        assertEquals("Maneskin", prueba.getNombre());
        assertEquals(TypeArtist.GROUP, prueba.getTipo());
        assertEquals(generosMusicales1, prueba.getGenerosMusicales());
        assertEquals(fecha1, prueba.getFechaNacimiento());
        assertEquals(TypeGender.NON_BINARY, prueba.getGenero());
        assertEquals(puntuaciones1, prueba.getPuntuaciones());

    }

    @Test
    void ordenarOrdenNatural() {
        ArrayList<Artist> artistOrdenada = new ArrayList<>(Arrays.asList(artist4, artist3, artist2, artist5, artist1));

        assertEquals(3, artist1.compareTo(artist2));
        assertEquals(-1, artist4.compareTo(artist3));
        assertEquals(0, artist1.compareTo(prueba));

        OrdenarArtist ordenarArtist = new OrdenarArtist();
        ordenarArtist.setArtistas(artistList);
        ordenarArtist.ordenarArtistas();
        assertEquals(artistOrdenada, ordenarArtist.getArtistas());
    }

    @Test
    void ordenarEclecticismo() {
        ComparadorEclecticismo comparadorEclecticismo = new ComparadorEclecticismo();
        ArrayList<Artist> artistOrdenada = new ArrayList<>(Arrays.asList(artist1, artist2, artist5, artist4, artist3));

        assertEquals(1, comparadorEclecticismo.compare(artist4, artist1));
        Collection<String> generosMusicales1 = new ArrayList<>(Arrays.asList("Rock", "Pop Rock", "Indie", "Pop", "Rock & Roll", "Indie Rock"));
        prueba.setGenerosMusicales(generosMusicales1);
        assertEquals(0, comparadorEclecticismo.compare(artist1, prueba));
        assertEquals(-1, comparadorEclecticismo.compare(artist5, artist3));

        OrdenarArtist ordenarArtist = new OrdenarArtist();
        ordenarArtist.setArtistas(artistList);
        assertEquals(artistOrdenada, ordenarArtist.ordenarArtistas(comparadorEclecticismo));
    }

    @Test
    void ordenarFecha() {
        ComparadorFecha comparadorFecha = new ComparadorFecha();
        ArrayList<Artist> artistOrdenada = new ArrayList<>(Arrays.asList(artist1, artist5, artist2, artist3, artist4));

        assertEquals(1, comparadorFecha.compare(artist3, artist1));
        prueba.setFechaNacimiento(artist1.getFechaNacimiento());
        assertEquals(0, comparadorFecha.compare(artist1, prueba));
        assertEquals(-1, comparadorFecha.compare(artist5, artist4));

        OrdenarArtist ordenarArtist = new OrdenarArtist();
        ordenarArtist.setArtistas(artistList);
        assertEquals(artistOrdenada, ordenarArtist.ordenarArtistas(comparadorFecha));
    }

    @Test
    void ordenarNombre() {
        ComparadorNombre comparadorNombre = new ComparadorNombre();
        ArrayList<Artist> artistOrdenada = new ArrayList<>(Arrays.asList(artist5, artist1, artist2, artist3, artist4));

        assertEquals(-5, comparadorNombre.compare(artist1, artist3));
        assertEquals(0, comparadorNombre.compare(artist1, prueba));
        assertEquals(1, comparadorNombre.compare(artist4, artist3));

        OrdenarArtist ordenarArtist = new OrdenarArtist();
        ordenarArtist.setArtistas(artistList);
        assertEquals(artistOrdenada, ordenarArtist.ordenarArtistas(comparadorNombre));
    }

    @Test
    void ordenarPuntMedia() {
        ComparadorPuntMedia comparadorPuntMedia = new ComparadorPuntMedia();
        ArrayList<Artist> artistOrdenada = new ArrayList<>(Arrays.asList(artist4, artist1, artist3, artist2, artist5));

        assertEquals(-1, comparadorPuntMedia.compare(artist2, artist5));
        Collection<Integer> puntuaciones1 = new ArrayList<>(Arrays.asList(6, 3, 4, 7));
        prueba.setPuntuaciones(puntuaciones1);
        assertEquals(0, comparadorPuntMedia.compare(artist1, prueba));
        assertEquals(1, comparadorPuntMedia.compare(artist3, artist1));

        OrdenarArtist ordenarArtist = new OrdenarArtist();
        ordenarArtist.setArtistas(artistList);
        assertEquals(artistOrdenada, ordenarArtist.ordenarArtistas(comparadorPuntMedia));
    }

}