#include "../include/model/Pokemin.h"
#include <iostream>

// Constructor vacio, inicializa todo en cero
Pokemon::Pokemon() {
    id          = 0;
    nombre      = "";
    ataqueBase  = 0;
    nivel       = 0;
    vida        = 0;
    vidaMaxima  = 0;
    velocidad   = 0;
}

// Constructor con todos los parametros
Pokemon::Pokemon(int id, const std::string& nombre, int ataqueBase,
                 int nivel, int vida, int velocidad) {
    this->id         = id;
    this->nombre     = nombre;
    this->ataqueBase = ataqueBase;
    this->nivel      = nivel;
    this->vida       = vida;
    this->vidaMaxima = vida;
    this->velocidad  = velocidad;
}

int Pokemon::getId() const          { return id; }
std::string Pokemon::getNombre() const { return nombre; }
int Pokemon::getAtaqueBase() const  { return ataqueBase; }
int Pokemon::getNivel() const       { return nivel; }
int Pokemon::getVida() const        { return vida; }
int Pokemon::getVidaMaxima() const  { return vidaMaxima; }
int Pokemon::getVelocidad() const   { return velocidad; }

void Pokemon::setVida(int v)           { vida = v; }
void Pokemon::setVidaMaxima(int v)     { vidaMaxima = v; }

// Restaura la vida al maximo
void Pokemon::restaurarVida() {
    vida = vidaMaxima;
}

// Danio = ataque base * nivel
int Pokemon::calcularDanio() const {
    return ataqueBase * nivel;
}

// Muestra todos los datos del Pokemon
void Pokemon::mostrar() const {
    std::cout << "ID: [" << id << "]\n";
    std::cout << "Nombre: " << nombre << "\n";
    std::cout << "Nivel: " << nivel << "\n";
    std::cout << "Vida: " << vida << "\n";
    std::cout << "Velocidad: " << velocidad << "\n";
    std::cout << "Ataque base: " << ataqueBase << "\n";
}
