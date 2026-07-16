#ifndef SIGP_H
#define SIGP_H

#include "Pokemon.h"
#include "../struct/ArbolAVL.h"
#include "../struct/Equipo.h"
#include "../struct/ArbolB.h"
#include <string>

class SIGP {
private:
    ArbolAVL catalogo;  // Catalogo de todas las especies (arbol AVL)
    Equipo   equipo;    // Equipo activo del entrenador (arreglo estatico de 6)
    ArbolB   pc;        // PC del entrenador (arbol B M=4)

    // Lee el archivo CSV y carga las especies en el catalogo
    bool cargarArchivo(const std::string& ruta);

    // Muestra los 3 primeros Pokemon del CSV y pide elegir uno
    void seleccionarPokemonInicial();

    // Genera un numero aleatorio entre min y max (ambos inclusive)
    int aleatorio(int min, int max) const;

    // Verifica si un ID ya esta capturado (equipo o PC)
    bool yaCapturado(int id) const;

    // Opciones del menu
    void encuentroPokemon();
    void mostrarEquipo();
    void dejarPokemonEnPC();
    void buscarPokemon();


    // Ejecuta el combate turno a turno entre el Pokemon del entrenador y el salvaje
    // Retorna true si el salvaje fue derrotado
    bool combatir(Pokemon& mio, Pokemon& salvaje);

    // Intenta capturar el Pokemon salvaje con una pokebola
    // Retorna true si la captura fue exitosa
    bool lanzarPokebola(Pokemon& mio, Pokemon& salvaje);

public:
    SIGP();

    // Inicializa el sistema: carga el archivo y muestra seleccion inicial
    bool inicializar(const std::string& ruta);

    // Ejecuta el loop del menu principal
    void ejecutar();
};

#endif