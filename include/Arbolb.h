#ifndef ARBOLB_H
#define ARBOLB_H

#include "../model/Pokemin.h"

// Orden del arbol B (M=4 segun enunciado)
const int M = 4;

struct NodoB {
    Pokemon claves[M - 1];   // Hasta 3 Pokemon por nodo
    int numClaves;            // Cuantos Pokemon hay actualmente
    NodoB* hijos[M];         // Hasta 4 hijos
    bool esHoja;             // true si no tiene hijos

    NodoB();
};

class ArbolB {
private:
    NodoB* raiz;

    // Divide el hijo lleno del nodo padre en la posicion i
    void dividirHijo(NodoB* padre, int i, NodoB* hijoLleno);

    // Inserta en un nodo que no esta lleno
    void insertarNoLleno(NodoB* nodo, const Pokemon& p);

    // Busca un Pokemon por ID de forma recursiva
    Pokemon* buscar(NodoB* nodo, int id) const;

    // Elimina un Pokemon por ID de forma recursiva
    bool eliminar(NodoB* nodo, int id);

    // Obtiene el predecesor (mayor clave del subarbol izquierdo)
    Pokemon obtenerPredecesor(NodoB* nodo, int idx);

    // Rellena el hijo en posicion idx si tiene menos de ceil(M/2)-1 claves
    void rellenar(NodoB* nodo, int idx);

    // Toma una clave del hijo izquierdo (idx-1)
    void tomarDelAnterior(NodoB* nodo, int idx);

    // Toma una clave del hijo derecho (idx+1)
    void tomarDelSiguiente(NodoB* nodo, int idx);

    // Fusiona el hijo idx con el hijo idx+1
    void fusionar(NodoB* nodo, int idx);

    // Libera toda la memoria del arbol
    void destruir(NodoB* nodo);

    // Recorre el arbol y llena un arreglo con los Pokemon (inorden)
    void inorden(NodoB* nodo, Pokemon* arr, int& cnt) const;

    // Cuenta cuantos Pokemon hay en el arbol
    int contarNodos(NodoB* nodo) const;

public:
    ArbolB();
    ~ArbolB();

    // Inserta un Pokemon en el PC
    void insertar(const Pokemon& p);

    // Busca un Pokemon por ID (nullptr si no existe)
    Pokemon* buscar(int id) const;

    // Elimina un Pokemon del PC por ID
    bool eliminar(int id);

    // Devuelve cuantos Pokemon hay en el PC
    int getCantidad() const;

    // Llena arr[] con todos los Pokemon del PC en orden de ID
    void obtenerTodos(Pokemon* arr, int& cantidad) const;
};

#endif
