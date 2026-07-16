#ifndef POKEMIN_H
#define POKEMIN_H

#include <string>

class Pokemon {
private:
    int id;
    std::string nombre;
    int ataqueBase;
    int nivel;
    int vida;
    int vidaMaxima;
    int velocidad;

public:
    // Constructor vacio
    Pokemon();

    // Constructor completo
    Pokemon(int id, const std::string& nombre, int ataqueBase,
            int nivel, int vida, int velocidad);

    // Getters
    int getId() const;
    std::string getNombre() const;
    int getAtaqueBase() const;
    int getNivel() const;
    int getVida() const;
    int getVidaMaxima() const;
    int getVelocidad() const;

    // Setters
    void setVida(int vida);
    void setVidaMaxima(int vidaMaxima);
    void restaurarVida();

    // Calcula el danio que causa este Pokemon en un ataque
    int calcularDanio() const;

    // Muestra los datos del Pokemon en consola
    void mostrar() const;
};

#endif