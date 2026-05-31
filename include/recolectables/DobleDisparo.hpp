#pragma once

#include "recolectables/PowerUp.hpp"

class DobleDisparo : public PowerUp {
public:
    explicit DobleDisparo(Posicion posicionInicial)
        : PowerUp(posicionInicial, 10.0f, sf::Color(110, 240, 255)) {}

    void Activar(Submarino& submarino) override {
        submarino.ActivarDobleDisparo(10.0f);
    }

    std::string ObtenerNombre() const override {
        return "Doble disparo";
    }
};
