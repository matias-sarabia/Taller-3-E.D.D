#include <iostream>
#include "include/model/SIGP.h"

int main() {
    SIGP sistema;

    // Carga el archivo y muestra la seleccion del Pokemon inicial
    if (!sistema.inicializar("especies.csv")) {
        std::cerr << "Error al inicializar el sistema. Verifique el archivo especies.csv.\n";
        return 1;
    }

    // Ejecuta el loop del menu principal hasta que el usuario elija Salir
    sistema.ejecutar();

    return 0;
}
