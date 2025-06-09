package e4;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class MesaBillarTest {
    private static MesaBillar mesa;

    @BeforeEach
    void setUp() {
        mesa = new MesaBillar();
    }

    @Test
    void iniciarPartida() {
        // Aseguramos que la partida no esta iniciada y que todas las bolas estan en el cajetin menos la blanca:
        assertFalse(mesa.esPartidaIniciada());
        List<BolaBillar> bolasCorrecto = new ArrayList<>(List.of(BolaBillar.values()));
        bolasCorrecto.remove(BolaBillar.BLANCA);

        assertEquals(bolasCorrecto, mesa.bolasCajetin());

        // Inicia la partida:
        mesa.iniciarPartida();

        // Aseguramos que la partida esta iniciada:
        assertTrue(mesa.esPartidaIniciada());

        // Aseguramos que todas las bolas estan en la mesa despues de iniciar la partida:
        assertEquals(BolaBillar.values().length, mesa.bolasMesa().size());

        for(BolaBillar bola : BolaBillar.values())
            assertTrue(mesa.bolasMesa().contains(bola));
    }

    @Test
    void meterBola() {
        // Intentamos meter bolas con la partida no iniciada:
        assertFalse(mesa.esPartidaIniciada());
        assertThrows(IllegalStateException.class, () -> mesa.meterBola(BolaBillar.BOLA2));

        // Inicia la partida:
        mesa.iniciarPartida();

        // Comprobar que al intentar meter la bola blanca, esta permanece en la mesa
        mesa.meterBola(BolaBillar.BLANCA);
        assertTrue(mesa.bolasMesa().contains(BolaBillar.BLANCA));

        // Comprobar que pasa si metemos bola cualquiera (la bola se añade al cajetin y se quita de la mesa)
        BolaBillar bolaValida = BolaBillar.BOLA3;
        mesa.meterBola(bolaValida);
        assertFalse(mesa.bolasMesa().contains(bolaValida));
        assertTrue(mesa.bolasCajetin().contains(bolaValida));

        // Comprobar que no se permite meter mas de una vez la misma bola:
        assertThrows(IllegalStateException.class, () -> mesa.meterBola(bolaValida));

        // Comprobar que pasa si metemos bola negra (la partida acaba y se imprime al ganador)
        mesa.meterBola(BolaBillar.BOLA8);
        assertFalse(mesa.esPartidaIniciada());
        assertTrue(mesa.obtenerGanador().equals("Empate") || mesa.obtenerGanador().equals("Bolas Lisas") || mesa.obtenerGanador().equals("Bolas Rayadas"));
    }

    @Test
    void bolasMesa() {
        // CASO 1: Partida no iniciada, la mesa debe estar vacia:
        assertFalse(mesa.esPartidaIniciada());
        assertTrue(mesa.bolasMesa().isEmpty());

        // Inicia la partida:
        mesa.iniciarPartida();
        assertTrue(mesa.esPartidaIniciada());

        // CASO 2: Al inicio de la partida, todas las bolas deben estar en la mesa:
        assertEquals(BolaBillar.values().length, mesa.bolasMesa().size());

        // CASO 3: Meter algunas bolas al cajetin y verificar que bolasMesa refleja correctamente la situacion:
        mesa.meterBola(BolaBillar.BOLA2);
        mesa.meterBola(BolaBillar.BOLA10);
        mesa.meterBola(BolaBillar.BOLA5);

        assertEquals(BolaBillar.values().length - 3, mesa.bolasMesa().size());

        // Ademas, las bolas salen incluidas en el cajetin:
        assertTrue(mesa.bolasCajetin().contains(BolaBillar.BOLA2));
        assertTrue(mesa.bolasCajetin().contains(BolaBillar.BOLA10));
        assertTrue(mesa.bolasCajetin().contains(BolaBillar.BOLA5));

        // CASO 4: Finalizar la partida:
        mesa.meterBola(BolaBillar.BOLA8);
        assertFalse(mesa.esPartidaIniciada());
    }

    @Test
    void bolasCajetin() {
        // CASO 1: Partida no iniciada, el cajetin debe tener todas las bolas menos la blanca:
        assertFalse(mesa.esPartidaIniciada());

        List<BolaBillar> bolasCorrecto = new ArrayList<>(List.of(BolaBillar.values()));
        bolasCorrecto.remove(BolaBillar.BLANCA);

        assertEquals(bolasCorrecto, mesa.bolasCajetin());

        // Inicia la partida:
        mesa.iniciarPartida();
        assertTrue(mesa.esPartidaIniciada());

        // CASO 2: Al inicio de la partida, el cajetin esta vacio:
        assertTrue(mesa.bolasCajetin().isEmpty());

        // CASO 3: Meter algunas bolas al cajetin y comprobar que bolasCajetin refleja la situacion:
        mesa.meterBola(BolaBillar.BOLA6);
        mesa.meterBola(BolaBillar.BOLA4);
        mesa.meterBola(BolaBillar.BOLA11);

        assertEquals(3, mesa.bolasCajetin().size());
        assertTrue(mesa.bolasCajetin().contains(BolaBillar.BOLA6));
        assertTrue(mesa.bolasCajetin().contains(BolaBillar.BOLA4));
        assertTrue(mesa.bolasCajetin().contains(BolaBillar.BOLA11));

        // Ademas, comprobamos que no estan en la mesa:
        assertFalse(mesa.bolasMesa().contains(BolaBillar.BOLA6));
        assertFalse(mesa.bolasMesa().contains(BolaBillar.BOLA4));
        assertFalse(mesa.bolasMesa().contains(BolaBillar.BOLA11));

        // CASO 4: Intentar meter la bola blanca, esta debe permanecer en la mesa:
        mesa.meterBola(BolaBillar.BLANCA);
        assertFalse(mesa.bolasCajetin().contains(BolaBillar.BLANCA));

        // CASO 5: Finalizar la partida:
        mesa.meterBola(BolaBillar.BOLA8);
        assertFalse(mesa.esPartidaIniciada());
    }

    @Test
    void esPartidaIniciada() {
        //CASO 1: La partida no esta iniciada y devuelve falso:
        assertFalse(mesa.esPartidaIniciada());

        //CASO 2: La partida esta iniciada y devuelve true:
        mesa.iniciarPartida();
        assertTrue(mesa.esPartidaIniciada());
    }

    @Test
    void obtenerGanador() {
        mesa.iniciarPartida();

        // Acaba de empezar la partida: empate
        assertEquals("Empate", mesa.obtenerGanador());

        // Hay algunas bolas metidas: caso empate
        mesa.meterBola(BolaBillar.BOLA1);
        mesa.meterBola(BolaBillar.BOLA9);
        assertEquals("Empate", mesa.obtenerGanador());

        // Ganan las rayadas
        mesa.meterBola(BolaBillar.BOLA10);
        mesa.meterBola(BolaBillar.BOLA11);
        assertEquals("Bolas Rayadas", mesa.obtenerGanador());

        // Ganan las lisas
        mesa.meterBola(BolaBillar.BOLA2);
        mesa.meterBola(BolaBillar.BOLA3);
        mesa.meterBola(BolaBillar.BOLA4);
        assertEquals("Bolas Lisas", mesa.obtenerGanador());
    }
}