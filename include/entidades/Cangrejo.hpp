#pragma once

#include "entidades/Enemigo.hpp"

class Cangrejo : public Enemigo {
public:
    explicit Cangrejo(Posicion posicionInicial)
        : Enemigo("Cangrejo", Vida(30), DanoOxigeno(20), Velocidad(42.0f, 0.0f), CadenciaDisparo(2.0f),
                  posicionInicial, sf::Color(220, 70, 60), 200) {}

    std::unique_ptr<ProyectilEnemigo> Disparar() override {
        cadenciaDisparo.Reiniciar();
        return std::make_unique<ProyectilCangrejo>(ObtenerPosicionDisparo());
    }
};
