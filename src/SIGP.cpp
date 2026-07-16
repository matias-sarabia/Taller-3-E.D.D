#include "../include/model/SIGP.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>

// ============================================================
// CONSTRUCTOR
// ============================================================

SIGP::SIGP() {
    // Inicializa la semilla del generador de numeros aleatorios
    srand((unsigned int)time(nullptr));
}

// Genera un numero aleatorio entre min y max
int SIGP::aleatorio(int min, int max) const {
    return min + rand() % (max - min + 1);
}

// Verifica si un Pokemon ya fue capturado (esta en el equipo o en el PC)
bool SIGP::yaCapturado(int id) const {
    return equipo.existeId(id) || (pc.buscar(id) != nullptr);
}

// Lee especies.csv y las inserta en el catalogo AVL
bool SIGP::cargarArchivo(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "Error: no se pudo abrir el archivo '" << ruta << "'.\n";
        return false;
    }

    int lineasLeidas = 0;
    std::string linea;

    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        std::istringstream ss(linea);
        std::string sId, nombre, sAtaque;

        if (!std::getline(ss, sId, ',') ||
            !std::getline(ss, nombre, ',') ||
            !std::getline(ss, sAtaque, ',')) {
            std::cout << "Error de formato en linea: " << linea << "\n";
            continue;
        }

        int id      = std::stoi(sId);
        int ataque  = std::stoi(sAtaque);

        // El catalogo solo guarda la especie (nivel/vida/velocidad en 0, se asignan al capturar)
        Pokemon especie(id, nombre, ataque, 0, 0, 0);
        catalogo.insertar(especie);
        lineasLeidas++;
    }

    archivo.close();

    if (lineasLeidas < 3) {
        std::cout << "Error: el archivo debe tener al menos 3 registros.\n";
        return false;
    }

    return true;
}


// Seleccion del pokemon inicial

void SIGP::seleccionarPokemonInicial() {
    // Obtiene todos los IDs del catalogo en orden
    int ids[500];
    int cantidad = 0;
    catalogo.obtenerIds(ids, cantidad);

    std::cout << "<--------------------------------------->";
    std::cout << "Sistema Integral de Gestion Pokemon (SIGP)";
    std::cout << "Selecciona tu Pokemon inicial:";

    // Muestra los 3 primeros del catalogo
    for (int i = 0; i < 3; i++) {
        Pokemon* p = catalogo.buscar(ids[i]);
        if (p) std::cout << "  " << (i + 1) << ") " << p->getNombre();
    }

    int opcion = 0;
    while (opcion < 1 || opcion > 3) {
        std::cout << "Opcion: ";
        std::cin >> opcion;
        if (std::cin.fail() || opcion < 1 || opcion > 3) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max());
            std::cout << "Opcion invalida. Ingrese 1, 2 o 3.";
            opcion = 0;
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max());

    // Crea el Pokemon inicial con las estadisticas fijas del enunciado
    Pokemon* especie = catalogo.buscar(ids[opcion - 1]);
    Pokemon inicial(especie->getId(), especie->getNombre(),
                    50,   // ataque base fijo
                    5,    // nivel fijo
                    300,  // vida fija
                    80);  // velocidad fija

    equipo.agregar(inicial);
    std::cout << "Elegiste a " << inicial.getNombre() << "!";
}

// Menu Principal

bool SIGP::inicializar(const std::string& ruta) {
    if (!cargarArchivo(ruta)) return false;
    seleccionarPokemonInicial();
    return true;
}

void SIGP::ejecutar() {
    int opcion = 0;
    while (true) {
        std::cout << "-- Menu Principal --";
        std::cout << "1) Encuentro Pokemon";
        std::cout << "2) Mostrar Equipo";
        std::cout << "3) Dejar Pokemon en el PC";
        std::cout << "4) Buscar Pokemon";
        std::cout << "5) Salir";
        std::cout << "Opcion: ";

        std::cin >> opcion;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max());
            std::cout << "Opcion invalida.";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max());

        switch (opcion) {
            case 1: encuentroPokemon();   break;
            case 2: mostrarEquipo();      break;
            case 3: dejarPokemonEnPC();   break;
            case 4: buscarPokemon();      break;
            case 5:
                std::cout << "Hasta luego, entrenador!";
                return;
            default:
                std::cout << "Opcion invalida. Ingrese entre 1 y 5.";
        }
    }
}

// Opcion 1: Encuentro Pokemon


// Combate turno a turno. Retorna true si el salvaje fue derrotado.
bool SIGP::combatir(Pokemon& mio, Pokemon& salvaje) {
    std::cout << "-- Combate --";

    // Determina quien ataca primero segun velocidad
    bool mioAtacaPrimero;
    if (mio.getVelocidad() > salvaje.getVelocidad())
        mioAtacaPrimero = true;
    else if (salvaje.getVelocidad() > mio.getVelocidad())
        mioAtacaPrimero = false;
    else
        mioAtacaPrimero = (aleatorio(0, 1) == 1);

    std::cout << (mioAtacaPrimero ? mio.getNombre() : salvaje.getNombre())
              << " ataca primero!";

    bool turnoMio = mioAtacaPrimero;

    while (mio.getVida() > 0 && salvaje.getVida() > 0) {
        if (turnoMio) {
            int danio = mio.calcularDanio();
            int nuevaVida = salvaje.getVida() - danio;
            if (nuevaVida < 0) nuevaVida = 0;
            salvaje.setVida(nuevaVida);
            std::cout << mio.getNombre() << " ataca! Danio: " << danio
                      << " | Vida " << salvaje.getNombre() << ": " << salvaje.getVida() << ;
        } else {
            int danio = salvaje.calcularDanio();
            int nuevaVida = mio.getVida() - danio;
            if (nuevaVida < 0) nuevaVida = 0;
            mio.setVida(nuevaVida);
            std::cout << salvaje.getNombre() << " ataca! Danio: " << danio
                      << " | Vida " << mio.getNombre() << ": " << mio.getVida() << ;
        }
        turnoMio = !turnoMio;
    }

    if (mio.getVida() <= 0) {
        std::cout << mio.getNombre() << " fue derrotado. El entrenador huye...";
        mio.restaurarVida();
        return false;
    }

    std::cout << salvaje.getNombre() << " fue derrotado!";
    return true;
}

// Intenta capturar el salvaje con una pokebola
bool SIGP::lanzarPokebola(Pokemon& mio, Pokemon& salvaje) {
    // Calcula probabilidad segun vida actual vs vida total del salvaje
    double prob = (1.0 - (double)salvaje.getVida() / salvaje.getVidaMaxima()) * 100.0;
    int probInt  = (int)prob;

    int tirada = aleatorio(1, 100);
    std::cout << "Probabilidad de captura: " << probInt << "% | Tirada: " << tirada << ;

    if (tirada <= probInt) {
        std::cout << "Pokebola lanzada... iCaptura exitosa!";
        return true;
    }

    // Captura fallida: el salvaje contraataca
    std::cout << "La Pokebola fallo. " << salvaje.getNombre() << " contraataca!";
    int danio = salvaje.calcularDanio();
    int nuevaVida = mio.getVida() - danio;
    if (nuevaVida < 0) nuevaVida = 0;
    mio.setVida(nuevaVida);
    std::cout << "Danio recibido: " << danio << " | Vida " << mio.getNombre()
              << ": " << mio.getVida() << ;

    if (mio.getVida() <= 0) {
        std::cout << mio.getNombre() << " fue derrotado. El entrenador huye...";
        mio.restaurarVida();
    }
    return false;
}

void SIGP::encuentroPokemon() {
    std::cout << "Encuentro Pokemon: Buscando Pokemon...";

    // Obtiene todos los IDs del catalogo
    int ids[500];
    int cantidad = 0;
    catalogo.obtenerIds(ids, cantidad);

    // Busca un ID que no este ya capturado (hasta 20 intentos)
    int idSalvaje = -1;
    for (int intento = 0; intento < 20; intento++) {
        int idx = aleatorio(0, cantidad - 1);
        if (!yaCapturado(ids[idx])) {
            idSalvaje = ids[idx];
            break;
        }
    }

    if (idSalvaje == -1) {
        std::cout << "No se encontraron Pokemon salvajes disponibles. Volviendo al menu.";
        return;
    }

    // Crea el Pokemon salvaje con estadisticas aleatorias
    Pokemon* especie = catalogo.buscar(idSalvaje);
    int nivelSalvaje = aleatorio(5, 15);
    int velSalvaje   = aleatorio(50, 100);
    int vidaSalvaje  = aleatorio(100, 500);

    Pokemon salvaje(idSalvaje, especie->getNombre(), especie->getAtaqueBase(),
                    nivelSalvaje, vidaSalvaje, velSalvaje);

    // Muestra los datos del salvaje
    std::cout << "Id: " << salvaje.getId();
    std::cout << "Nombre: " << salvaje.getNombre();
    std::cout << "Ataque base: " << salvaje.getAtaqueBase();
    std::cout << "Nivel: " << salvaje.getNivel();
    std::cout << "Velocidad: " << salvaje.getVelocidad();
    std::cout << "Vida: " << salvaje.getVida();

    // Seleccion del Pokemon del entrenador
    Pokemon* mio = nullptr;
    if (equipo.getCantidad() == 1) {
        mio = equipo.getPokemon(0);
    } else {
        std::cout << "Seleccione el Pokemon que utilizara para el encuentro:";
        for (int i = 0; i < equipo.getCantidad(); i++)
            std::cout << "  " << (i + 1) << ". " << equipo.getPokemon(i)->getNombre() << ;

        int sel = 0;
        while (sel < 1 || sel > equipo.getCantidad()) {
            std::cout << "Opcion: ";
            std::cin >> sel;
            if (std::cin.fail() || sel < 1 || sel > equipo.getCantidad()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), );
                std::cout << "Opcion invalida.";
                sel = 0;
            }
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), );
        mio = equipo.getPokemon(sel - 1);
    }

    // Loop del encuentro
    bool encuentroActivo = true;
    while (encuentroActivo && salvaje.getVida() > 0 && mio->getVida() > 0) {
        std::cout << "Encuentro Pokemon:";
        std::cout << "1) Atacar";
        std::cout << "2) Lanzar Pokebola";
        std::cout << "3) Huir";
        std::cout << "Opcion: ";

        int accion = 0;
        std::cin >> accion;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), );
            std::cout << "Opcion invalida.";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), );

        switch (accion) {
            case 1: {
                // Atacar
                bool derroto = combatir(*mio, salvaje);
                if (derroto) {
                    encuentroActivo = false; // Salvaje derrotado, vuelve al menu
                } else if (mio->getVida() <= 0) {
                    encuentroActivo = false; // Mio fue derrotado, ya se restauro la vida
                }
                break;
            }
            case 2: {
                // Lanzar Pokebola
                bool capturo = lanzarPokebola(*mio, salvaje);
                if (capturo) {
                    // Restaura la vida del salvaje antes de guardarlo
                    salvaje.restaurarVida();

                    if (!equipo.estaLleno()) {
                        equipo.agregar(salvaje);
                        std::cout << salvaje.getNombre() << " fue agregado al equipo!\n";
                    } else {
                        pc.insertar(salvaje);
                        std::cout << salvaje.getNombre() << " fue enviado al PC!\n";
                    }
                    encuentroActivo = false;
                } else if (mio->getVida() <= 0) {
                    encuentroActivo = false;
                }
                break;
            }
            case 3:
                std::cout << "Huiste del encuentro.\n";
                encuentroActivo = false;
                break;
            default:
                std::cout << "Opcion invalida. Ingrese 1, 2 o 3.\n";
        }
    }
}

// ============================================================
// OPCION 2: MOSTRAR EQUIPO
// ============================================================

void SIGP::mostrarEquipo() {
    if (equipo.estaVacio()) {
        std::cout << "El equipo esta vacio.\n";
        return;
    }
    equipo.mostrar();
}

// ============================================================
// OPCION 3: DEJAR POKEMON EN EL PC
// ============================================================

void SIGP::dejarPokemonEnPC() {
    if (equipo.getCantidad() <= 1) {
        std::cout << "Error: necesitas al menos 2 Pokemon en el equipo para dejar uno en el PC.\n";
        return;
    }

    std::cout << "\nDejar Pokemon en el PC:\n";
    std::cout << "Ingrese el ID del Pokemon: ";
    int id;
    std::cin >> id;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ID invalido.\n";
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Pokemon* p = equipo.buscar(id);
    if (p == nullptr) {
        std::cout << "Error: no se encontro un Pokemon con ID " << id << " en el equipo.\n";
        return;
    }

    // Guarda una copia antes de eliminar del equipo
    Pokemon copia = *p;
    equipo.eliminar(id);
    pc.insertar(copia);
    std::cout << copia.getNombre() << " fue enviado al PC.\n";
}

// ============================================================
// OPCION 4: BUSCAR POKEMON
// ============================================================

void SIGP::buscarPokemon() {
    std::cout << "\nBuscar un Pokemon:\n";
    std::cout << "Ingrese su ID: ";
    int id;
    std::cin >> id;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ID invalido.\n";
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Busca primero en el equipo
    Pokemon* p = equipo.buscar(id);
    std::string ubicacion = "Equipo";

    if (p == nullptr) {
        // Busca en el PC
        p = pc.buscar(id);
        ubicacion = "PC";
    }

    if (p == nullptr) {
        std::cout << "Error: no se encontro un Pokemon con ID " << id << ".\n";
        return;
    }

    std::cout << "\nPokemon encontrado\n";
    std::cout << "ID: " << p->getId() << "\n";
    std::cout << "Nombre: " << p->getNombre() << "\n";
    std::cout << "Ubicacion actual: " << ubicacion << "\n";
    std::cout << "Nivel: " << p->getNivel() << "\n";
    std::cout << "Vida: " << p->getVida() << "\n";
    std::cout << "Velocidad: " << p->getVelocidad() << "\n";
    std::cout << "Ataque base: " << p->getAtaqueBase() << "\n";
}