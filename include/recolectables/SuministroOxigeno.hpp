#pragma once

#include "componentes/RecuperacionOxigeno.hpp"
#include "recolectables/PowerUp.hpp"

class SuministroOxigeno : public PowerUp {
public:
    explicit SuministroOxigeno(Posicion posicionInicial)
        : PowerUp(posicionInicial, 0.0f, sf::Color(60, 170, 255)), recuperacionOxigeno(30) {}

    void Activar(Submarino& submarino) override {
        submarino.RegenerarOxigeno(recuperacionOxigeno);
    }

    std::string ObtenerNombre() const override {
        return "Suministro oxigeno";
    }

private:
    RecuperacionOxigeno recuperacionOxigeno;
};
