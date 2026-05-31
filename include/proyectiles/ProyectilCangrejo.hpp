#pragma once

#include "proyectiles/ProyectilEnemigo.hpp"

class ProyectilCangrejo : public ProyectilEnemigo {
public:
    explicit ProyectilCangrejo(Posicion posicionInicial)
        : ProyectilEnemigo(DanoVida(15), DanoOxigeno(20), posicionInicial, Velocidad(0.0f, 305.0f), sf::Color(245, 90, 75)) {}
};
