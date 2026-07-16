#ifndef ARBOLAVL_H
#define ARBOLAVL_H

#include "../model/Pokemin.h"

struct NodoAVL {
    Pokemon dato;
    int altura;
    NodoAVL* izq;
    NodoAVL* der;

    NodoAVL(const Pokemon& p) : dato(p), altura(1), izq(nullptr), der(nullptr) {}
};

class ArbolAVL {
private:
    NodoAVL* raiz;

    // Altura de un nodo (0 si es null)
    int altura(NodoAVL* n) const;

    // Factor de balance de un nodo
    int balance(NodoAVL* n) const;

    // Rotaciones para mantener el balance
    NodoAVL* rotarDerecha(NodoAVL* y);
    NodoAVL* rotarIzquierda(NodoAVL* x);

    // Inserta recursivamente y rebalancea
    NodoAVL* insertar(NodoAVL* nodo, const Pokemon& p);

    // Busca un nodo por ID de forma recursiva
    NodoAVL* buscar(NodoAVL* nodo, int id) const;

    // Libera toda la memoria del arbol
    void destruir(NodoAVL* nodo);

    // Recorre en inorden y llena un arreglo con los IDs
    void inorden(NodoAVL* nodo, int* ids, int& contador) const;

    // Cuenta los nodos del arbol
    int contarNodos(NodoAVL* nodo) const;

public:
    ArbolAVL();
    ~ArbolAVL();

    // Inserta una especie en el catalogo
    void insertar(const Pokemon& p);

    // Busca una especie por ID y la devuelve (nullptr si no existe)
    Pokemon* buscar(int id) const;

    // Devuelve la cantidad de especies en el catalogo
    int getCantidad() const;

    // Llena el arreglo ids[] con todos los IDs del catalogo en orden
    void obtenerIds(int* ids, int& cantidad) const;
};

#endif
