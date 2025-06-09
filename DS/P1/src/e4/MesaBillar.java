package e4;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class MesaBillar {
    private final List<BolaBillar> bolasMesa;
    private final List<BolaBillar> bolasCajetin;
    private boolean partidaIniciada;


    // METODO CONSTRUCTOR:
    public MesaBillar() {
        this.bolasMesa = new ArrayList<>();
        this.bolasCajetin = new ArrayList<>();
        this.partidaIniciada = false;

        // Guardamos todas las bolas en el cajetin menos la blanca, que durante toda la partida estara en la mesa
        for(BolaBillar bola : BolaBillar.values()) {
            if(bola != BolaBillar.BLANCA)
                bolasCajetin.add(bola);
        }
    }

    public void iniciarPartida() {
        // Si la partida ya esta iniciada lanza una excepcion
        if (partidaIniciada)
            throw new IllegalStateException("La partida ya ha sido iniciada\n");

        // Vaciamos la mesa y el cajetin para empezar la partida
        bolasMesa.clear();
        bolasCajetin.clear();
        partidaIniciada = true;

        // Ponemos todas las bolas en la mesa
        Collections.addAll(bolasMesa, BolaBillar.values());
    }

    public void meterBola(BolaBillar bola) {
        // Si la partida no esta iniciada lanza una excepcion
        if(!partidaIniciada)
            throw new IllegalStateException("Partida no iniciada\n");

        // CASO DE METER LA BOLA BLANCA (siempre se devuelve a la mesa)
        if(bola == BolaBillar.BLANCA)
            System.out.println("La bola blanca ha sido devuelta a la mesa.\n");
        else if(bola == BolaBillar.BOLA8) { // CASO DE METER LA BOLA NEGRA (se acaba la partida y se imprime el ganador)
                System.out.println("La partida ha acabado. El ganador es: " + obtenerGanador());
                partidaIniciada = false;
        } else if(bola.getNum() >= 1 && bola.getNum() <= 7 || bola.getNum() >= 9 && bola.getNum() <= 15) { // CASO DE METER UNA BOLA VALIDA
            if(bolasCajetin.contains(bola)) // Si la bola ya se metio anteriormente
                throw new IllegalStateException("La bola de numero " + bola.getNum() + ", color " + bola.getColor() + " y tipo " + bola.getTipo() + " ya esta metida en el cajetin.\n");

            System.out.println("La bola de color " + bola.getColor() + ", numero " + bola.getNum() + ", y tipo " + bola.getTipo() + " sacada de la mesa.\n");
            bolasCajetin.add(bola); // Guardamos la bola en el cajetin
            bolasMesa.remove(bola); // Quitamos la bola de la mesa
        }
    }

    public List<BolaBillar> bolasMesa() {
        return bolasMesa;
    }

    public List<BolaBillar> bolasCajetin() {
        return bolasCajetin;
    }

    public boolean esPartidaIniciada() {
        return partidaIniciada;
    }

    public String obtenerGanador() {
        // Contador de "puntos"
        int cntBolasLisas = 0;
        int cntBolasRayadas = 0;

        // Contamos cuantas bolas validas de cada tipo hay en la mesa
        for(BolaBillar bola : bolasMesa) {
            int n = bola.getNum();

            if(n >= 1 && n <= 7)
                cntBolasLisas++;
            else if(n >= 9 && n <= 15)
                cntBolasRayadas++;
        }

        // Si hay menos bolas lisas que rayadas en la mesa, ganan las lisas
        if(cntBolasLisas < cntBolasRayadas)
            return "Bolas Lisas";
        else if(cntBolasRayadas < cntBolasLisas) // Si hay menos rayadas que lisas, ganan las rayadas
            return "Bolas Rayadas";
        else // En cualquier otro caso se produce empate
            return "Empate";
    }
}
