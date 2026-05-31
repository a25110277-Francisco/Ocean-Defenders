#pragma once

#include <SFML/System/Vector2.hpp>

class Posicion {
public:
    float x{0.0f};
    float y{0.0f};

    Posicion() = default;
    Posicion(float nuevaX, float nuevaY) : x(nuevaX), y(nuevaY) {}

    sf::Vector2f ObtenerVector() const {
        return {x, y};
    }

    void Asignar(float nuevaX, float nuevaY) {
        x = nuevaX;
        y = nuevaY;
    }

    void Mover(float deltaX, float deltaY) {
        x += deltaX;
        y += deltaY;
    }
};
