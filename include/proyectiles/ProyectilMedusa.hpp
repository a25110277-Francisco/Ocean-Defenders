#pragma once

#include "proyectiles/ProyectilEnemigo.hpp"

class ProyectilMedusa : public ProyectilEnemigo {
public:
    explicit ProyectilMedusa(Posicion posicionInicial)
        : ProyectilEnemigo(DanoVida(5), DanoOxigeno(10), posicionInicial, Velocidad(0.0f, 260.0f), sf::Color(255, 120, 230)) {}
};
