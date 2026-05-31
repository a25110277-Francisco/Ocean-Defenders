#pragma once

#include "recolectables/PowerUp.hpp"

class VelocidadAumentada : public PowerUp {
public:
    explicit VelocidadAumentada(Posicion posicionInicial)
        : PowerUp(posicionInicial, 5.0f, sf::Color(80, 255, 120)) {}

    void Activar(Submarino& submarino) override {
        submarino.ActivarVelocidadAumentada(5.0f);
    }

    std::string ObtenerNombre() const override {
        return "Velocidad aumentada";
    }
};
