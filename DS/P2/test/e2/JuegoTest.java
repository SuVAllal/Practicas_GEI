package e2;

import org.junit.jupiter.api.Test;

import e2.Objetos.*;
import e2.Personajes.*;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.*;

class JuegoTest {

    public Juego simulacion = new Juego();

    ObjetoAtaque sword1 = new Sword("Espada", 6, 2, 3, 3, 3);
    ObjetoAtaque sword2 = new Sword("Espada", 6, 2, 3, 3, 3);
    ObjetoDefensa sword3 = new Sword("Espada", 6, 2, 3, 3, 3);

    ObjetoAtaque wand1 = new Wand("Varita", 5, 5, 3, 3, 2);
    ObjetoAtaque wand2 = new Wand("Varita", 5, 5, 3, 3, 2);
    ObjetoDefensa wand3 = new Wand("Varita", 5, 5, 3, 3, 2);
    ObjetoDefensa wand4 = new Wand("Varita", 5, 5, 3, 3, 2);

    ObjetoAtaque fireBall1 = new FireBallSpell("FireBall", 8, 3, 5);
    ObjetoAtaque fireBall2 = new FireBallSpell("FireBall", 8, 3, 5);
    ObjetoAtaque fireBall3 = new FireBallSpell("FireBall", 8, 3, 5);

    ObjetoDefensa armor1 = new Armor("Armadura", 2, 3);

    Warrior warrior1 = new Warrior("Guerrero", 15, 8, 4);
    Wizard wizard1 = new Wizard("Mago", 12, 2, 8);

    ArrayList<ObjetoAtaque> objetosAtaques1 = new ArrayList<>(Arrays.asList(sword1, fireBall1, wand1)); // Sí para wizard, no para warrior
    ArrayList<ObjetoAtaque> objetosAtaques2 = new ArrayList<>(Arrays.asList(fireBall2, fireBall3, sword2)); // Sí para ambos

    ArrayList<ObjetoDefensa> objetosDefensas1 = new ArrayList<>(Arrays.asList(sword3, armor1)); // Si warrior no wizard
    ArrayList<ObjetoDefensa> objetosDefensas2 = new ArrayList<>(Collections.singletonList(wand4)); // Si para ambos

    @Test
    void listaObjetos() {
        // Caso base:
        warrior1.setObjetosAtaque(objetosAtaques2);
        assertEquals(objetosAtaques2, warrior1.getObjetosAtaque());
        wizard1.setObjetosAtaque(objetosAtaques1);
        assertEquals(objetosAtaques1, wizard1.getObjetosAtaque());

        warrior1.setObjetosDefensa(objetosDefensas1);
        assertEquals(objetosDefensas1, warrior1.getObjetosDefensa());
        wizard1.setObjetosDefensa(objetosDefensas2);
        assertEquals(objetosDefensas2, wizard1.getObjetosDefensa());

        // Caso en el que uno o varios de los objetos no son validos:
        assertThrows(IllegalArgumentException.class, () -> warrior1.setObjetosAtaque(objetosAtaques1));
        assertThrows(IllegalArgumentException.class, () -> wizard1.setObjetosDefensa(objetosDefensas1));

        // Exceso de objetos:
        ArrayList<ObjetoAtaque> objetosAtaques = new ArrayList<>(Arrays.asList(sword1, fireBall1, wand1, sword2, fireBall2, wand2));
        assertThrows(IllegalArgumentException.class, () -> warrior1.setObjetosAtaque(objetosAtaques));
        assertThrows(IllegalArgumentException.class, () -> wizard1.setObjetosAtaque(objetosAtaques));

        ArrayList<ObjetoDefensa> objetosDefensas = new ArrayList<>(Arrays.asList(sword3, wand3, armor1, wand4, armor1, sword3));
        assertThrows(IllegalArgumentException.class, () -> warrior1.setObjetosDefensa(objetosDefensas));
        assertThrows(IllegalArgumentException.class, () -> wizard1.setObjetosDefensa(objetosDefensas));

    }

    @Test
    void atributosPersonaje() {
        // Getters:
        assertEquals("Guerrero", warrior1.getNombre());
        assertEquals("Mago", wizard1.getNombre());

        assertEquals(15, warrior1.getVida());
        assertEquals(12, wizard1.getVida());

        assertEquals(8, warrior1.getFuerza());
        assertEquals(2, wizard1.getFuerza());

        assertEquals(4, warrior1.getMana());
        assertEquals(8, wizard1.getMana());

        // Crear un personaje con atributos no validos:

        // Atributo vida:
        assertThrows(IllegalArgumentException.class, () -> new Warrior("Guerrero", -4, 5, 5));
        assertThrows(IllegalArgumentException.class, () -> new Warrior("Guerrero", 24, 5, 5));
        // Atributo fuerza:
        assertThrows(IllegalArgumentException.class, () -> new Warrior("Guerrero", 14, -5, 5));
        assertThrows(IllegalArgumentException.class, () -> new Warrior("Guerrero", 14, 15, 5));
        // Atributo mana:
        assertThrows(IllegalArgumentException.class, () -> new Warrior("Guerrero", 14, 5, -5));
        assertThrows(IllegalArgumentException.class, () -> new Warrior("Guerrero", 14, 5, 15));

    }

    @Test
    void testAtaque() {
        // Intentar atacar sin objetos equipados:
        assertThrows(IllegalStateException.class, () -> warrior1.atacar(wizard1));
    }

    @Test
    void simulacionAtaque() {
        warrior1.setObjetosAtaque(objetosAtaques2);
        wizard1.setObjetosDefensa(objetosDefensas2);

        assertEquals(5, simulacion.simulacionAtaque(warrior1, wizard1));
    }

    @Test
    void simulacionLuchaTurnos() {
        // Se alcanza el maximo de turnos:
        warrior1.setObjetosAtaque(objetosAtaques2);
        warrior1.setObjetosDefensa(objetosDefensas1);
        wizard1.setObjetosAtaque(objetosAtaques1);
        wizard1.setObjetosDefensa(objetosDefensas2);
        assertNull(simulacion.simulacionLuchaTurnos(warrior1, wizard1, 3));

        // Caso base:
        Warrior warrior1 = new Warrior("Guerrero", 15, 8, 4);
        Wizard wizard1 = new Wizard("Mago", 12, 2, 8);
        warrior1.setObjetosAtaque(objetosAtaques2);
        warrior1.setObjetosDefensa(objetosDefensas2);
        wizard1.setObjetosAtaque(objetosAtaques1);
        wizard1.setObjetosDefensa(objetosDefensas2);
        assertEquals(warrior1, simulacion.simulacionLuchaTurnos(warrior1, wizard1, 20));


    }
}