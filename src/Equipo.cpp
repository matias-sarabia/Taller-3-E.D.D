#include "../include/struct/Equipo.h"
#include <iostream>

Equipo::Equipo() : cantidad(0) {}

// Agrega al final si hay espacio
bool Equipo::agregar(const Pokemon& p) {
    if (cantidad >= TAM_EQUIPO) return false;
    miembros[cantidad++] = p;
    return true;
}

// Elimina por ID y compacta el arreglo (sin huecos)
bool Equipo::eliminar(int id) {
    for (int i = 0; i < cantidad; i++) {
        if (miembros[i].getId() == id) {
            // Desplaza todos los siguientes una posicion hacia atras
            for (int j = i; j < cantidad - 1; j++)
                miembros[j] = miembros[j + 1];
            cantidad--;
            return true;
        }
    }
    return false;
}

// Busca por ID y devuelve puntero al Pokemon (nullptr si no existe)
Pokemon* Equipo::buscar(int id) const {
    for (int i = 0; i < cantidad; i++)
        if (miembros[i].getId() == id)
            return const_cast<Pokemon*>(&miembros[i]);
    return nullptr;
}

Pokemon* Equipo::getPokemon(int i) {
    if (i < 0 || i >= cantidad) return nullptr;
    return &miembros[i];
}

int Equipo::getCantidad() const  { return cantidad; }
bool Equipo::estaLleno() const   { return cantidad >= TAM_EQUIPO; }
bool Equipo::estaVacio() const   { return cantidad == 0; }

bool Equipo::existeId(int id) const {
    return buscar(id) != nullptr;
}

// Muestra todos los Pokemon del equipo con separadores
void Equipo::mostrar() const {
    std::cout << "Equipo:";
    for (int i = 0; i < cantidad; i++) {
        std::cout << "-----------------------";
        miembros[i].mostrar();
    }
    std::cout << "-----------------------";
}