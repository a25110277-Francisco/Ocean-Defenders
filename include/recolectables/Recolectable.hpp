#pragma once

#include <SFML/Graphics.hpp>
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

    virtual void Dibujar(sf::RenderWindow& ventana) const {
        if (!activo) {
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
