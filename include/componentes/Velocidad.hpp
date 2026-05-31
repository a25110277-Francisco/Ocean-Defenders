#pragma once

#include <SFML/System/Vector2.hpp>

class Velocidad {
public:
    float x{0.0f};
    float y{0.0f};

    Velocidad() = default;
    Velocidad(float nuevaX, float nuevaY) : x(nuevaX), y(nuevaY) {}

    sf::Vector2f ObtenerVector() const {
        return {x, y};
    }

    void Asignar(float nuevaX, float nuevaY) {
        x = nuevaX;
        y = nuevaY;
    }
};
