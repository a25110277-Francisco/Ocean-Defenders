#pragma once

#include "proyectiles/ProyectilEnemigo.hpp"

class ProyectilJefe : public ProyectilEnemigo {
public:
    ProyectilJefe(Posicion posicionInicial, Velocidad velocidadInicial = Velocidad(0.0f, 330.0f))
        : ProyectilEnemigo(DanoVida(20), DanoOxigeno(25), posicionInicial, velocidadInicial, sf::Color::Red) {
        tamano = {14.0f, 16.0f};
    }

    void Dibujar(sf::RenderWindow& ventana) const override {
        if (!activo) {
            return;
        }

        sf::ConvexShape triangulo;
        triangulo.setPointCount(3);
        triangulo.setPoint(0, {tamano.x * 0.5f, tamano.y});
        triangulo.setPoint(1, {0.0f, 0.0f});
        triangulo.setPoint(2, {tamano.x, 0.0f});
        triangulo.setPosition(posicion.ObtenerVector());
        triangulo.setFillColor(sf::Color(235, 45, 45));
        ventana.draw(triangulo);
    }
};
