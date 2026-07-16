#include "../include/struct/Arbolavl.h"
#include <algorithm>

// ============================================================
// PRIVADOS
// ============================================================

// Devuelve la altura almacenada en el nodo (0 si es null)
int ArbolAVL::altura(NodoAVL* n) const {
    return (n == nullptr) ? 0 : n->altura;
}

// Factor de balance: altura izq - altura der
int ArbolAVL::balance(NodoAVL* n) const {
    return (n == nullptr) ? 0 : altura(n->izq) - altura(n->der);
}

// Rotacion simple a la derecha
NodoAVL* ArbolAVL::rotarDerecha(NodoAVL* y) {
    NodoAVL* x  = y->izq;
    NodoAVL* T2 = x->der;

    x->der = y;
    y->izq = T2;

    // Actualiza alturas (primero el hijo, luego el padre)
    y->altura = 1 + std::max(altura(y->izq), altura(y->der));
    x->altura = 1 + std::max(altura(x->izq), altura(x->der));

    return x;
}

// Rotacion simple a la izquierda
NodoAVL* ArbolAVL::rotarIzquierda(NodoAVL* x) {
    NodoAVL* y  = x->der;
    NodoAVL* T2 = y->izq;

    y->izq = x;
    x->der = T2;

    x->altura = 1 + std::max(altura(x->izq), altura(x->der));
    y->altura = 1 + std::max(altura(y->izq), altura(y->der));

    return y;
}

// Insercion recursiva con rebalanceo AVL
NodoAVL* ArbolAVL::insertar(NodoAVL* nodo, const Pokemon& p) {
    // Insercion BST normal
    if (nodo == nullptr) return new NodoAVL(p);

    if (p.getId() < nodo->dato.getId())
        nodo->izq = insertar(nodo->izq, p);
    else if (p.getId() > nodo->dato.getId())
        nodo->der = insertar(nodo->der, p);
    else
        return nodo; // ID duplicado, no se inserta

    // Actualiza altura del nodo actual
    nodo->altura = 1 + std::max(altura(nodo->izq), altura(nodo->der));

    // Calcula el factor de balance
    int b = balance(nodo);

    // Caso Izquierda-Izquierda
    if (b > 1 && p.getId() < nodo->izq->dato.getId())
        return rotarDerecha(nodo);

    // Caso Derecha-Derecha
    if (b < -1 && p.getId() > nodo->der->dato.getId())
        return rotarIzquierda(nodo);

    // Caso Izquierda-Derecha
    if (b > 1 && p.getId() > nodo->izq->dato.getId()) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }

    // Caso Derecha-Izquierda
    if (b < -1 && p.getId() < nodo->der->dato.getId()) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

// Busqueda recursiva por ID
NodoAVL* ArbolAVL::buscar(NodoAVL* nodo, int id) const {
    if (nodo == nullptr || nodo->dato.getId() == id)
        return nodo;
    if (id < nodo->dato.getId())
        return buscar(nodo->izq, id);
    return buscar(nodo->der, id);
}

// Libera memoria en postorden
void ArbolAVL::destruir(NodoAVL* nodo) {
    if (nodo == nullptr) return;
    destruir(nodo->izq);
    destruir(nodo->der);
    delete nodo;
}

// Recorrido inorden para obtener IDs ordenados
void ArbolAVL::inorden(NodoAVL* nodo, int* ids, int& contador) const {
    if (nodo == nullptr) return;
    inorden(nodo->izq, ids, contador);
    ids[contador++] = nodo->dato.getId();
    inorden(nodo->der, ids, contador);
}

// Cuenta nodos recursivamente
int ArbolAVL::contarNodos(NodoAVL* nodo) const {
    if (nodo == nullptr) return 0;
    return 1 + contarNodos(nodo->izq) + contarNodos(nodo->der);
}

ArbolAVL::ArbolAVL() : raiz(nullptr) {}

ArbolAVL::~ArbolAVL() {
    destruir(raiz);
}

void ArbolAVL::insertar(const Pokemon& p) {
    raiz = insertar(raiz, p);
}

Pokemon* ArbolAVL::buscar(int id) const {
    NodoAVL* nodo = buscar(raiz, id);
    return (nodo == nullptr) ? nullptr : &(nodo->dato);
}

int ArbolAVL::getCantidad() const {
    return contarNodos(raiz);
}

void ArbolAVL::obtenerIds(int* ids, int& cantidad) const {
    cantidad = 0;
    inorden(raiz, ids, cantidad);
}
