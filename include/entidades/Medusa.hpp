#pragma once

#include "entidades/Enemigo.hpp"

class Medusa : public Enemigo {
public:
    explicit Medusa(Posicion posicionInicial)
        : Enemigo("Medusa", Vida(10), DanoOxigeno(10), Velocidad(70.0f, 0.0f), CadenciaDisparo(3.0f),
                  posicionInicial, sf::Color(220, 90, 210), 100) {}

    std::unique_ptr<ProyectilEnemigo> Disparar() override {
        cadenciaDisparo.Reiniciar();
        return std::make_unique<ProyectilMedusa>(ObtenerPosicionDisparo());
    }
};
