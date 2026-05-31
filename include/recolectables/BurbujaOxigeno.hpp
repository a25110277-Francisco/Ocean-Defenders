#pragma once

#include "componentes/RecuperacionOxigeno.hpp"
#include "recolectables/Recolectable.hpp"

class BurbujaOxigeno : public Recolectable {
public:
    explicit BurbujaOxigeno(Posicion posicionInicial)
        : Recolectable(posicionInicial, sf::Color(150, 230, 255), {22.0f, 22.0f}), recuperacionOxigeno(15) {
        velocidad.Asignar(0.0f, 65.0f);
    }

    void Recolectar(Submarino& submarino) override {
        submarino.RegenerarOxigeno(recuperacionOxigeno);
        Desactivar();
    }

    void Desaparecer() {
        Desactivar();
    }

    std::string ObtenerNombre() const override {
        return "Burbuja oxigeno";
    }

private:
    RecuperacionOxigeno recuperacionOxigeno;
};
