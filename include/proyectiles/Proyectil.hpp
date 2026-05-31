#pragma once

#include <SFML/Graphics.hpp>
#include "componentes/DanoVida.hpp"
#include "componentes/Posicion.hpp"
#include "componentes/Velocidad.hpp"

class Proyectil {
public:
    Proyectil(DanoVida danoInicial, Posicion posicionInicial, Velocidad velocidadInicial, sf::Vector2f tamanoInicial, sf::Color colorInicial)
        : danoVida(danoInicial), posicion(posicionInicial), velocidad(velocidadInicial), tamano(tamanoInicial), color(colorInicial) {}

    virtual ~Proyectil() = default;

    virtual void Mover(float delta) {
        posicion.Mover(velocidad.x * delta, velocidad.y * delta);
    }

    virtual void Actualizar(float delta) {
        Mover(delta);
    }

    virtual void Impactar() {
        activo = false;
    }

    virtual void Dibujar(sf::RenderWindow& ventana) const {
        if (!activo) {
            return;
        }

        sf::RectangleShape forma(tamano);
        forma.setPosition(posicion.ObtenerVector());
        forma.setFillColor(color);
        ventana.draw(forma);
    }

    sf::FloatRect ObtenerBounds() const {
        return {posicion.ObtenerVector(), tamano};
    }

    const Posicion& ObtenerPosicion() const {
        return posicion;
    }

    int ObtenerDanoVida() const {
        return danoVida.ObtenerCantidad();
    }

    bool EstaActivo() const {
        return activo;
    }

    void Desactivar() {
        activo = false;
    }

protected:
    DanoVida danoVida;
    Posicion posicion;
    Velocidad velocidad;
    sf::Vector2f tamano;
    sf::Color color;
    bool activo{true};
};
