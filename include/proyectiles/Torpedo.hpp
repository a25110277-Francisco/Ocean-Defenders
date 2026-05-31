#pragma once

#include "proyectiles/Proyectil.hpp"

class BarreraCoral;
class Enemigo;

class Torpedo : public Proyectil {
public:
    explicit Torpedo(Posicion posicionInicial)
        : Proyectil(DanoVida(10), posicionInicial, Velocidad(0.0f, -470.0f), {6.0f, 18.0f}, sf::Color(100, 240, 255)) {}

    void ImpactarEnemigo(Enemigo&) {
        Impactar();
    }

    void ImpactarBarrera(BarreraCoral&) {
        Impactar();
    }
};
