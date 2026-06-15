#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <string>
#include "componentes/Posicion.hpp"
#include "componentes/Velocidad.hpp"
#include "entidades/Submarino.hpp"

class Recolectable {
public:
    Recolectable(Posicion posicionInicial, sf::Color colorInicial, sf::Vector2f tamanoInicial = {24.0f, 24.0f})
        : posicion(posicionInicial), color(colorInicial), tamano(tamanoInicial) {}

    virtual ~Recolectable() = default;

    virtual void Caer(float delta) {
        posicion.Mover(velocidad.x * delta, velocidad.y * delta);
    }

    virtual void Actualizar(float delta) {
        Caer(delta);
    }

    virtual void Recolectar(Submarino& submarino) = 0;

    virtual void Dibujar(sf::RenderWindow& ventana, const sf::Texture* textura = nullptr) const {
        if (!activo) {
            return;
        }

        if (textura != nullptr) {
            const sf::Vector2u texturaTamano = textura->getSize();
            const float escala = std::min(
                56.0f / static_cast<float>(texturaTamano.x),
                56.0f / static_cast<float>(texturaTamano.y)
            );
            const sf::Vector2f tamanoVisual{
                static_cast<float>(texturaTamano.x) * escala,
                static_cast<float>(texturaTamano.y) * escala
            };
            sf::Sprite sprite(*textura);
            sprite.setPosition({
                posicion.x + (tamano.x - tamanoVisual.x) * 0.5f,
                posicion.y + (tamano.y - tamanoVisual.y) * 0.5f
            });
            sprite.setScale({escala, escala});
            ventana.draw(sprite);
            return;
        }

        sf::CircleShape forma(tamano.x * 0.5f);
        forma.setPosition(posicion.ObtenerVector());
        forma.setFillColor(color);
        ventana.draw(forma);
    }

    sf::FloatRect ObtenerBounds() const {
        return {posicion.ObtenerVector(), tamano};
    }

    bool EstaActivo() const {
        return activo;
    }

    void Desactivar() {
        activo = false;
    }

    const Posicion& ObtenerPosicion() const {
        return posicion;
    }

    virtual std::string ObtenerNombre() const {
        return "Recolectable";
    }

protected:
    Posicion posicion;
    Velocidad velocidad{0.0f, 90.0f};
    sf::Color color;
    sf::Vector2f tamano;
    bool activo{true};
};
