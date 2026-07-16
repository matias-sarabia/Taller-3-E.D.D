
#include "../include/struct/Arbolb.h"
#include <iostream>

// Nodo B
NodoB::NodoB() {
    numClaves = 0;
    esHoja    = true;
    for (int i = 0; i < M; i++) hijos[i] = nullptr;
}

// Divide el hijo lleno (hijoLleno) del padre en posicion i
void ArbolB::dividirHijo(NodoB* padre, int i, NodoB* hijoLleno) {
    int mitad = (M - 1) / 2; // Para M=4, mitad=1

    NodoB* nuevo = new NodoB();
    nuevo->esHoja = hijoLleno->esHoja;
    nuevo->numClaves = (M - 1) - mitad - 1; // claves que van al nuevo nodo

    // Copia la mitad derecha de claves al nuevo nodo
    for (int j = 0; j < nuevo->numClaves; j++)
        nuevo->claves[j] = hijoLleno->claves[j + mitad + 1];

    // Si no es hoja, copia tambien los hijos
    if (!hijoLleno->esHoja)
        for (int j = 0; j <= nuevo->numClaves; j++)
            nuevo->hijos[j] = hijoLleno->hijos[j + mitad + 1];

    hijoLleno->numClaves = mitad;

    // Hace espacio en el padre para el nuevo hijo
    for (int j = padre->numClaves; j >= i + 1; j--)
        padre->hijos[j + 1] = padre->hijos[j];
    padre->hijos[i + 1] = nuevo;

    // Sube la clave del medio al padre
    for (int j = padre->numClaves - 1; j >= i; j--)
        padre->claves[j + 1] = padre->claves[j];
    padre->claves[i] = hijoLleno->claves[mitad];
    padre->numClaves++;
}

// Inserta en nodo que tiene espacio (no lleno)
void ArbolB::insertarNoLleno(NodoB* nodo, const Pokemon& p) {
    int i = nodo->numClaves - 1;

    if (nodo->esHoja) {
        // Desplaza claves mayores a la derecha
        while (i >= 0 && nodo->claves[i].getId() > p.getId()) {
            nodo->claves[i + 1] = nodo->claves[i];
            i--;
        }
        nodo->claves[i + 1] = p;
        nodo->numClaves++;
    } else {
        // Busca el hijo correcto
        while (i >= 0 && nodo->claves[i].getId() > p.getId()) i--;
        i++;
        // Si el hijo esta lleno, lo divide primero
        if (nodo->hijos[i]->numClaves == M - 1) {
            dividirHijo(nodo, i, nodo->hijos[i]);
            if (nodo->claves[i].getId() < p.getId()) i++;
        }
        insertarNoLleno(nodo->hijos[i], p);
    }
}

// Busca por ID en el subarbol con raiz 'nodo'
Pokemon* ArbolB::buscar(NodoB* nodo, int id) const {
    int i = 0;
    while (i < nodo->numClaves && id > nodo->claves[i].getId()) i++;

    if (i < nodo->numClaves && id == nodo->claves[i].getId())
        return &(nodo->claves[i]);

    if (nodo->esHoja) return nullptr;

    return buscar(nodo->hijos[i], id);
}

// Obtiene el predecesor de la clave en posicion idx
Pokemon ArbolB::obtenerPredecesor(NodoB* nodo, int idx) {
    NodoB* actual = nodo->hijos[idx];
    while (!actual->esHoja)
        actual = actual->hijos[actual->numClaves];
    return actual->claves[actual->numClaves - 1];
}

// Toma una clave del hermano izquierdo (idx-1)
void ArbolB::tomarDelAnterior(NodoB* nodo, int idx) {
    NodoB* hijo    = nodo->hijos[idx];
    NodoB* hermano = nodo->hijos[idx - 1];

    // Desplaza claves del hijo a la derecha
    for (int i = hijo->numClaves - 1; i >= 0; i--)
        hijo->claves[i + 1] = hijo->claves[i];

    if (!hijo->esHoja)
        for (int i = hijo->numClaves; i >= 0; i--)
            hijo->hijos[i + 1] = hijo->hijos[i];

    // La clave del padre baja al hijo
    hijo->claves[0] = nodo->claves[idx - 1];

    if (!hijo->esHoja)
        hijo->hijos[0] = hermano->hijos[hermano->numClaves];

    // La ultima clave del hermano sube al padre
    nodo->claves[idx - 1] = hermano->claves[hermano->numClaves - 1];
    hijo->numClaves++;
    hermano->numClaves--;
}

// Toma una clave del hermano derecho (idx+1)
void ArbolB::tomarDelSiguiente(NodoB* nodo, int idx) {
    NodoB* hijo    = nodo->hijos[idx];
    NodoB* hermano = nodo->hijos[idx + 1];

    // La clave del padre baja al final del hijo
    hijo->claves[hijo->numClaves] = nodo->claves[idx];

    if (!hijo->esHoja)
        hijo->hijos[hijo->numClaves + 1] = hermano->hijos[0];

    // La primera clave del hermano sube al padre
    nodo->claves[idx] = hermano->claves[0];

    // Desplaza claves del hermano a la izquierda
    for (int i = 1; i < hermano->numClaves; i++)
        hermano->claves[i - 1] = hermano->claves[i];

    if (!hermano->esHoja)
        for (int i = 1; i <= hermano->numClaves; i++)
            hermano->hijos[i - 1] = hermano->hijos[i];

    hijo->numClaves++;
    hermano->numClaves--;
}

// Fusiona el hijo idx con el hijo idx+1
void ArbolB::fusionar(NodoB* nodo, int idx) {
    NodoB* hijo    = nodo->hijos[idx];
    NodoB* hermano = nodo->hijos[idx + 1];
    int minClaves  = (M - 1) / 2; // =1 para M=4

    // La clave separadora del padre baja al hijo
    hijo->claves[minClaves] = nodo->claves[idx];

    // Copia claves del hermano al hijo
    for (int i = 0; i < hermano->numClaves; i++)
        hijo->claves[i + minClaves + 1] = hermano->claves[i];

    if (!hijo->esHoja)
        for (int i = 0; i <= hermano->numClaves; i++)
            hijo->hijos[i + minClaves + 1] = hermano->hijos[i];

    // Elimina la clave separadora del padre
    for (int i = idx + 1; i < nodo->numClaves; i++)
        nodo->claves[i - 1] = nodo->claves[i];

    for (int i = idx + 2; i <= nodo->numClaves; i++)
        nodo->hijos[i - 1] = nodo->hijos[i];

    hijo->numClaves += hermano->numClaves + 1;
    nodo->numClaves--;

    delete hermano;
}

// Rellena el hijo idx si tiene menos de minClaves claves
void ArbolB::rellenar(NodoB* nodo, int idx) {
    int minClaves = (M - 1) / 2; // =1 para M=4

    if (idx != 0 && nodo->hijos[idx - 1]->numClaves > minClaves)
        tomarDelAnterior(nodo, idx);
    else if (idx != nodo->numClaves && nodo->hijos[idx + 1]->numClaves > minClaves)
        tomarDelSiguiente(nodo, idx);
    else {
        if (idx != nodo->numClaves)
            fusionar(nodo, idx);
        else
            fusionar(nodo, idx - 1);
    }
}

// Elimina la clave con id del subarbol con raiz 'nodo'
bool ArbolB::eliminar(NodoB* nodo, int id) {
    int minClaves = (M - 1) / 2;
    int i = 0;
    while (i < nodo->numClaves && nodo->claves[i].getId() < id) i++;

    if (i < nodo->numClaves && nodo->claves[i].getId() == id) {
        // La clave esta en este nodo
        if (nodo->esHoja) {
            // Caso 1: nodo hoja, simplemente la elimina
            for (int j = i + 1; j < nodo->numClaves; j++)
                nodo->claves[j - 1] = nodo->claves[j];
            nodo->numClaves--;
            return true;
        } else {
            // Caso 2: nodo interno
            if (nodo->hijos[i]->numClaves > minClaves) {
                Pokemon pred = obtenerPredecesor(nodo, i);
                nodo->claves[i] = pred;
                return eliminar(nodo->hijos[i], pred.getId());
            } else if (nodo->hijos[i + 1]->numClaves > minClaves) {
                // Usa sucesor
                NodoB* actual = nodo->hijos[i + 1];
                while (!actual->esHoja) actual = actual->hijos[0];
                Pokemon suc = actual->claves[0];
                nodo->claves[i] = suc;
                return eliminar(nodo->hijos[i + 1], suc.getId());
            } else {
                fusionar(nodo, i);
                return eliminar(nodo->hijos[i], id);
            }
        }
    } else {
        // La clave no esta en este nodo
        if (nodo->esHoja) return false;

        bool enUltimoHijo = (i == nodo->numClaves);
        if (nodo->hijos[i]->numClaves <= minClaves)
            rellenar(nodo, i);

        if (enUltimoHijo && i > nodo->numClaves)
            return eliminar(nodo->hijos[i - 1], id);
        else
            return eliminar(nodo->hijos[i], id);
    }
}

// Libera toda la memoria en postorden
void ArbolB::destruir(NodoB* nodo) {
    if (nodo == nullptr) return;
    if (!nodo->esHoja)
        for (int i = 0; i <= nodo->numClaves; i++)
            destruir(nodo->hijos[i]);
    delete nodo;
}

// Recorre en inorden y llena el arreglo
void ArbolB::inorden(NodoB* nodo, Pokemon* arr, int& cnt) const {
    if (nodo == nullptr) return;
    for (int i = 0; i < nodo->numClaves; i++) {
        if (!nodo->esHoja) inorden(nodo->hijos[i], arr, cnt);
        arr[cnt++] = nodo->claves[i];
    }
    if (!nodo->esHoja) inorden(nodo->hijos[nodo->numClaves], arr, cnt);
}

// Cuenta recursivamente
int ArbolB::contarNodos(NodoB* nodo) const {
    if (nodo == nullptr) return 0;
    int total = nodo->numClaves;
    if (!nodo->esHoja)
        for (int i = 0; i <= nodo->numClaves; i++)
            total += contarNodos(nodo->hijos[i]);
    return total;
}

ArbolB::ArbolB() : raiz(nullptr) {}

ArbolB::~ArbolB() {
    destruir(raiz);
}

void ArbolB::insertar(const Pokemon& p) {
    if (raiz == nullptr) {
        raiz = new NodoB();
        raiz->claves[0] = p;
        raiz->numClaves = 1;
        return;
    }

    // Si la raiz esta llena, crece el arbol hacia arriba
    if (raiz->numClaves == M - 1) {
        NodoB* nueva = new NodoB();
        nueva->esHoja   = false;
        nueva->hijos[0] = raiz;
        dividirHijo(nueva, 0, raiz);
        // Decide en cual de los dos hijos insertar
        int i = (nueva->claves[0].getId() < p.getId()) ? 1 : 0;
        insertarNoLleno(nueva->hijos[i], p);
        raiz = nueva;
    } else {
        insertarNoLleno(raiz, p);
    }
}

Pokemon* ArbolB::buscar(int id) const {
    if (raiz == nullptr) return nullptr;
    return buscar(raiz, id);
}

bool ArbolB::eliminar(int id) {
    if (raiz == nullptr) return false;
    bool resultado = eliminar(raiz, id);
    // Si la raiz quedo vacia y tiene un hijo, ese hijo pasa a ser la nueva raiz
    if (raiz->numClaves == 0) {
        NodoB* vieja = raiz;
        raiz = raiz->esHoja ? nullptr : raiz->hijos[0];
        vieja->esHoja = true; // evitar que destruir elimine hijos
        delete vieja;
    }
    return resultado;
}

int ArbolB::getCantidad() const {
    return contarNodos(raiz);
}

void ArbolB::obtenerTodos(Pokemon* arr, int& cantidad) const {
    cantidad = 0;
    if (raiz != nullptr) inorden(raiz, arr, cantidad);
}