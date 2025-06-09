package e2.Personajes;

import e2.Objetos.ObjetoAtaque;
import e2.Objetos.ObjetoDefensa;

import java.util.ArrayList;
import java.util.List;

public class Personaje {
    private final String nombre;
    private int vida;
    private final int fuerza;
    private final int mana;

    private List<ObjetoDefensa> objetosDefensa;
    private List<ObjetoAtaque> objetosAtaque;


    public Personaje(String nombre, int vida, int fuerza, int mana) {
        this.nombre = nombre;

        if(vida < 0 || vida > 20)
            throw new IllegalArgumentException("La vida debe estar entre 0 y 20\n");
        else
            this.vida = vida;

        if(fuerza < 1 || fuerza > 10)
            throw new IllegalArgumentException("La fuerza debe estar entre 1 y 10\n");
        else
            this.fuerza = fuerza;

        if(mana < 0 || mana > 10)
            throw new IllegalArgumentException("El mana debe estar entre 0 y 10\n");
        else
            this.mana = mana;

        this.objetosDefensa = new ArrayList<>();
        this.objetosAtaque = new ArrayList<>();
    }

    public String getNombre() {
        return nombre;
    }

    public int getVida() {
        return vida;
    }

    public int getFuerza() {
        return fuerza;
    }

    public int getMana() {
        return mana;
    }

    public List<ObjetoDefensa> getObjetosDefensa() {
        return objetosDefensa;
    }

    // Funcion auxiliar para saber si un objeto es valido para que el personaje lo equipe:
    private boolean equiparDefensa(ObjetoDefensa objetoDefensa) {
        return fuerza >= objetoDefensa.getFuerzaMin();
    }

    public void setObjetosDefensa(ArrayList<ObjetoDefensa> objDefensa) {
        if (objDefensa.size() > 5)
            throw new IllegalArgumentException("La lista de objetos de defensa no puede tener mas de 5 elementos\n");
        else {
            for (ObjetoDefensa objeto : objDefensa) {
                if (!equiparDefensa(objeto))
                    throw new IllegalArgumentException("Uno o varios de los objetos de defensa de la lista no son validos");
            }

            this.objetosDefensa = objDefensa;
        }
    }

    public List<ObjetoAtaque> getObjetosAtaque() {
        return objetosAtaque;
    }

    // Funcion auxiliar para saber si un objeto es valido para que el personaje lo equipe:
    private boolean equiparAtaque(ObjetoAtaque objetoAtaque) {
        return mana >= objetoAtaque.getManaMin();
    }

    public void setObjetosAtaque(ArrayList<ObjetoAtaque> objAtaque) {
        if (objAtaque.size() > 5)
            throw new IllegalArgumentException("La lista de objetos de ataque no puede tener mas de 5 elementos\n");
        else {
            for (ObjetoAtaque objeto : objAtaque) {
                if (!equiparAtaque(objeto))
                    throw new IllegalArgumentException("Uno o varios de los objetos de defensa de la lista no son validos");
            }

            this.objetosAtaque = objAtaque;
        }
    }

    // Funcion auxiliar para calcular la defensa total del personaje:
    private int getDefensaTotal() {
        int defensa = 0;
        for(ObjetoDefensa objetoDefensa : objetosDefensa)
            defensa += objetoDefensa.getValorDefensa();

        return defensa;
    }

    public void defender(Personaje enemigo) {
        ObjetoAtaque objetoAtaque = enemigo.objetosAtaque.get(0); // Obtenemos el objeto con el que van a atacar al personaje
        int ataqueRecibido = Math.max(0, objetoAtaque.getValorAtaque() - getDefensaTotal());
        vida = Math.max(0, vida - ataqueRecibido);

        System.out.println("El daño recibido por " + nombre + " ha sido de " + ataqueRecibido + ", queda " + vida + " de vida");
    }

    public void atacar(Personaje enemigo) {
        if(!objetosAtaque.isEmpty()) {
            ObjetoAtaque objetoAtaqueActual = objetosAtaque.get(0);
            System.out.println(nombre + " ha atacado.");

            if(objetoAtaqueActual.getUsos() > 0) {
                objetoAtaqueActual.reducirUsos();
            } else
                objetosAtaque.remove(objetoAtaqueActual);
        } else
            throw new IllegalStateException("El personaje no tiene objetos de ataque equipados");

    }

    public boolean isAlive() {
        return vida > 0;
    }

}
