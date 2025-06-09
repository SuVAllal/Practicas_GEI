package e2;

import e2.Nodos.Bifurcacion.NodoAvistamiento;
import e2.Nodos.Bifurcacion.NodoBatalla;
import e2.Nodos.Nodo;
import e2.Nodos.NodoFin;
import e2.Nodos.RutaFija.NodoAtaqueAereo;
import e2.Nodos.RutaFija.NodoTormentaMarina;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class AlmiranteTest {
    Almirante almirante;
    Nodo mapa;
    Flota flotaA, flotaB, flotaEnemiga1, flotaEnemiga2;

    @BeforeEach
    void setUp() {
        flotaA = new Flota(11, 42, 47, 0, 0);
        flotaB = new Flota(1, 25, 0, 46, 28);
        flotaEnemiga1 = new Flota(20, 30, 25, 0, 0);
        flotaEnemiga2 = new Flota(30, 17, 41, 0, 0);

        mapa = new NodoAvistamiento("A", 28, new NodoBatalla("B", flotaEnemiga1,
                new NodoFin("D"), new NodoAtaqueAereo("E", 151, new NodoFin("K"))),
                new NodoTormentaMarina("C", 20, new NodoBatalla("F", flotaEnemiga2, new NodoFin("H"),
                        new NodoTormentaMarina("G", 67, new NodoFin("I")))));

        almirante = new Almirante();
    }

    @Test
    void simularIncursion() {
        String salida1 = "Sortie Result:\nSUCCESS\nLast Visited Node: H\nFinal HP: 1";
        assertEquals(salida1, almirante.simularIncursion(mapa, flotaA));

        String salida2 = "Sortie Result:\nFAIL\nLast Visited Node: E\nFinal HP: -33";
        assertEquals(salida2, almirante.simularIncursion(mapa, flotaB));
    }

    @Test
    void minimoNodos() {
        String salida = "Smallest Node Count to End: 3";
        assertEquals(salida, almirante.minimoNodos(mapa));
    }

    @Test
    void mapaTexto() {
        String salida = "(A WaypointSpotting, (B Battle, (D End), " +
                "(E AirRaid, (K End))), (C Maelstrom, (F Battle, (H End), " +
                "(G Maelstrom, (I End)))))";

        assertEquals(salida, almirante.mapaTexto(mapa));
    }
}