#pragma once

#include "proyectiles/ProyectilEnemigo.hpp"

class ProyectilJefe : public ProyectilEnemigo {
public:
    ProyectilJefe(Posicion posicionInicial, Velocidad velocidadInicial = Velocidad(0.0f, 330.0f))
        : ProyectilEnemigo(DanoVida(20), DanoOxigeno(25), posicionInicial, velocidadInicial, sf::Color(180, 80, 255)) {}
};
