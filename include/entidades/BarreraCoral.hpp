#pragma once

#include <SFML/Graphics.hpp>
#include "componentes/Posicion.hpp"
#include "componentes/Vida.hpp"
#include "core/EstadoJuego.hpp"
#include "entidades/EstadoBarrera.hpp"
#include "proyectiles/Proyectil.hpp"

class BarreraCoral {
public:
    BarreraCoral() : vida(40), posicion(0.0f, 0.0f) {}
    explicit BarreraCoral(Posicion posicionInicial) : vida(40), posicion(posicionInicial) {}

    void RecibirImpacto(Proyectil&) {
        vida.RecibirDano(5);
        ActualizarEstado();
    }

    void RecibirImpacto(int dano = 5) {
        vida.RecibirDano(dano);
        ActualizarEstado();
    }

    bool BloquearProyectil(Proyectil& proyectil) {
        if (vida.EstaAgotada()) {
            return false;
        }
        proyectil.Impactar();
        RecibirImpacto(5);
        return true;
    }

    EstadoBarrera ActualizarEstado() {
        estadoBarrera.ActualizarPorVida(vida.ObtenerActual());
        return estadoBarrera;
    }

    EstadoJuego EstaDestruida() const {
        return vida.EstaAgotada() ? EstadoJuego(EstadoJuego::Tipo::Jugando) : EstadoJuego();
    }

    bool Destruida() const {
        return vida.EstaAgotada();
    }

    void Dibujar(sf::RenderWindow& ventana) const {
        if (vida.EstaAgotada()) {
            return;
        }

        sf::Color color = sf::Color(35, 220, 150);
        if (estadoBarrera.ObtenerTipo() == EstadoBarrera::Tipo::Agrietada) {
            color = sf::Color(80, 175, 130);
        } else if (estadoBarrera.ObtenerTipo() == EstadoBarrera::Tipo::Fracturada) {
            color = sf::Color(120, 120, 95);
        }

        sf::RectangleShape tronco({tamano.x, tamano.y * 0.55f});
        tronco.setPosition({posicion.x, posicion.y + tamano.y * 0.35f});
        tronco.setFillColor(color);
        ventana.draw(tronco);

        sf::CircleShape coral(18.0f);
        coral.setFillColor(color);
        coral.setPosition({posicion.x + 6.0f, posicion.y});
        ventana.draw(coral);
        coral.setPosition({posicion.x + 32.0f, posicion.y - 3.0f});
        ventana.draw(coral);
        coral.setPosition({posicion.x + 58.0f, posicion.y + 3.0f});
        ventana.draw(coral);
    }

    sf::FloatRect ObtenerBounds() const {
        return {posicion.ObtenerVector(), tamano};
    }

    const Vida& ObtenerVida() const {
        return vida;
    }

private:
    Vida vida;
    Posicion posicion;
    EstadoBarrera estadoBarrera;
    sf::Vector2f tamano{92.0f, 44.0f};
};
