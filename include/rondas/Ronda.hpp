#pragma once

#include <string>
#include <utility>
#include "componentes/Velocidad.hpp"

class Ronda {
public:
    Ronda(std::string nombreInicial, int numeroInicial, int medusasInicial, int calamaresInicial, int cangrejosInicial,
          bool jefeInicial, float multiplicadorVelocidadInicial)
        : nombre(std::move(nombreInicial)), numero(numeroInicial), medusas(medusasInicial), calamares(calamaresInicial),
          cangrejos(cangrejosInicial), jefe(jefeInicial), velocidad(multiplicadorVelocidadInicial, 0.0f) {}

    virtual ~Ronda() = default;

    int ObtenerNumero() const {
        return numero;
    }

    int ObtenerMedusas() const {
        return medusas;
    }

    int ObtenerCalamares() const {
        return calamares;
    }

    int ObtenerCangrejos() const {
        return cangrejos;
    }

    bool EsRondaJefe() const {
        return jefe;
    }

    float ObtenerMultiplicadorVelocidad() const {
        return velocidad.x;
    }

    const std::string& ObtenerNombre() const {
        return nombre;
    }

private:
    std::string nombre;
    int numero{1};
    int medusas{0};
    int calamares{0};
    int cangrejos{0};
    bool jefe{false};
    Velocidad velocidad;
};
