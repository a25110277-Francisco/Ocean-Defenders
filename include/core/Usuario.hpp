#pragma once

#include <string>
#include <utility>

class Juego;
class Submarino;

class Usuario {
public:
    explicit Usuario(std::string nombreInicial = "Jugador") : nombre(std::move(nombreInicial)) {}

    const std::string& ObtenerNombre() const {
        return nombre;
    }

private:
    std::string nombre;
};
