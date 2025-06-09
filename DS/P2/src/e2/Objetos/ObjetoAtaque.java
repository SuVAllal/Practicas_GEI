package e2.Objetos;

public interface ObjetoAtaque {
    int getValorAtaque(); // Daño que pueden inflingir
    int getManaMin(); // Minimo avlor de mana para utilizarlo
    int getUsos(); // Numero de usos restantes
    void reducirUsos();
}