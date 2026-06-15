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

    void Dibujar(sf::RenderWindow& ventana) const override {
        Dibujar(ventana, nullptr);
    }

    void Dibujar(sf::RenderWindow& ventana, const sf::Texture* textura) const {
        if (!activo) {
            return;
        }

        if (textura == nullptr) {
            Proyectil::Dibujar(ventana);
            return;
        }

        const sf::Vector2u texturaTamano = textura->getSize();
        const sf::Vector2f tamanoVisual{72.0f, 72.0f};
        sf::Sprite sprite(*textura);
        sprite.setPosition({
            posicion.x + (tamano.x - tamanoVisual.x) * 0.5f,
            posicion.y + (tamano.y - tamanoVisual.y) * 0.5f
        });
        sprite.setScale({
            tamanoVisual.x / static_cast<float>(texturaTamano.x),
            tamanoVisual.y / static_cast<float>(texturaTamano.y)
        });
        ventana.draw(sprite);
    }
};
