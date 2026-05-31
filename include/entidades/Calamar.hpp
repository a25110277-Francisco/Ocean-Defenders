#pragma once

#include "entidades/Enemigo.hpp"

class Calamar : public Enemigo {
public:
    explicit Calamar(Posicion posicionInicial)
        : Enemigo("Calamar", Vida(20), DanoOxigeno(15), Velocidad(55.0f, 0.0f), CadenciaDisparo(2.5f),
                  posicionInicial, sf::Color(235, 150, 55), 150) {}

    std::unique_ptr<ProyectilEnemigo> Disparar() override {
        cadenciaDisparo.Reiniciar();
        return std::make_unique<ProyectilCalamar>(ObtenerPosicionDisparo());
    }
};
