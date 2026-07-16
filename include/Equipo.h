#ifndef EQUIPO_H
#define EQUIPO_H

#include "../model/Pokemin.h"

// Capacidad maxima del equipo activo
const int TAM_EQUIPO = 6;

/**
 * @brief Arreglo estatico de 6 Pokemon que representa el equipo activo.
 *        Los Pokemon se almacenan siempre desde la posicion 0, sin huecos.
 */
class Equipo {
private:
    Pokemon miembros[TAM_EQUIPO]; // Arreglo estatico
    int cantidad;                 // Cuantos Pokemon hay actualmente

public:
    Equipo();

    // Agrega un Pokemon al equipo (retorna false si esta lleno)
    bool agregar(const Pokemon& p);

    // Elimina un Pokemon por ID (retorna false si no existe)
    bool eliminar(int id);

    // Busca un Pokemon por ID (nullptr si no existe)
    Pokemon* buscar(int id) const;

    // Devuelve el Pokemon en la posicion i
    Pokemon* getPokemon(int i);

    // Devuelve cuantos Pokemon hay en el equipo
    int getCantidad() const;

    // Devuelve true si el equipo esta lleno
    bool estaLleno() const;

    // Devuelve true si el equipo esta vacio
    bool estaVacio() const;

    // Verifica si un ID ya existe en el equipo
    bool existeId(int id) const;

    // Muestra todos los Pokemon del equipo en consola
    void mostrar() const;
};

#endif
