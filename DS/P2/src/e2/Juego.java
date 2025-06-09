package e2;

import e2.Personajes.Personaje;

public class Juego {

    public int simulacionAtaque(Personaje atacante, Personaje defensor) {
        int vidaInicial = defensor.getVida();

        System.out.println("SIMULACION DE ATAQUE UNICO: \n");
        System.out.println("Vida de " + defensor.getNombre() + " antes del ataque: " + vidaInicial);

        atacante.atacar(defensor);
        defensor.defender(atacante);

        int vidaFinal = defensor.getVida();
        int danoEfectivo = vidaInicial - vidaFinal;

        System.out.println("Vida de " + defensor.getNombre() + " despues del ataque: " + vidaFinal);
        return danoEfectivo;
    }

    public Personaje simulacionLuchaTurnos(Personaje personaje1, Personaje personaje2, int turnos) {
        System.out.println("SIMULACION DE LUCHA POR TURNOS: \n");

        for(int cntTurnos = 1; cntTurnos <= turnos; cntTurnos++) {
            // Alternar roles de atacante y defensor:
            Personaje atacante, defensor;

            if(cntTurnos % 2 == 0) {
                atacante = personaje1;
                defensor = personaje2;
            } else {
                atacante = personaje2;
                defensor = personaje1;
            }

            atacante.atacar(defensor);
            defensor.defender(atacante);

            if(!defensor.isAlive()) {
                System.out.println(defensor.getNombre() + " ha sido derrotado en el turno " + cntTurnos);
                return atacante;
            }
        }

        System.out.println("Se ha alcanzado el limite de turnos (" + turnos + ")");
        return null;
    }
}
