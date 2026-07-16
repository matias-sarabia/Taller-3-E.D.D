cmake_minimum_required(VERSION 3.20)
project(taller3)

set(CMAKE_CXX_STANDARD 20)

add_executable(taller3 main.cpp
        model/Pokemon.cpp
        include/model/Pokemon.h
        model/SIGP.cpp
        include/model/SIGP.h
        struct/ArbolAVL.cpp
        include/struct/ArbolAVL.h
        struct/ArbolB.cpp
        include/struct/ArbolB.h
        struct/Equipo.cpp
        include/struct/Equipo.h
        src/Pokemin.cpp
        include/Pokemin.h
        src/SIGP.cpp
        include/SIGP.h
        src/Arbolavl.cpp
        include/Arbolavl.h
        src/Arbolb.cpp
        include/Arbolb.h
        src/Equipo.cpp
        include/Equipo.h
)