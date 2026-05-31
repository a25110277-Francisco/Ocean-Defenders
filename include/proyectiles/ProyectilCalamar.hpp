#pragma once

#include "proyectiles/ProyectilEnemigo.hpp"

class ProyectilCalamar : public ProyectilEnemigo {
public:
    explicit ProyectilCalamar(Posicion posicionInicial)
        : ProyectilEnemigo(DanoVida(10), DanoOxigeno(15), posicionInicial, Velocidad(0.0f, 280.0f), sf::Color(255, 190, 80)) {}
};
