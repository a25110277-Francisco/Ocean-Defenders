#pragma once

#include <SFML/Graphics.hpp>
#include "componentes/Posicion.hpp"
#include "componentes/Vida.hpp"
#include "core/EstadoJuego.hpp"
#include "proyectiles/ProyectilEnemigo.hpp"

class Arrecife {
public:
    Arrecife() : vida(1300), posicion(80.0f, 662.0f) {}

    void RecibirImpacto(const ProyectilEnemigo& proyectil) {
        vida.RecibirDano(proyectil.ObtenerDanoVida());
    }

    void RecibirDano(int dano) {
        vida.RecibirDano(dano);
    }

    EstadoJuego EstaDestruido() const {
        return vida.EstaAgotada() ? EstadoJuego(EstadoJuego::Tipo::DerrotaArrecife) : EstadoJuego();
    }

    bool Destruido() const {
        return vida.EstaAgotada();
    }

    void Dibujar(sf::RenderWindow& ventana) const {
        if (vida.EstaAgotada()) {
            return;
        }

        sf::RectangleShape base(tamano);
        base.setPosition(posicion.ObtenerVector());
        base.setFillColor(sf::Color(45, 150, 110));
        ventana.draw(base);

        const float proporcion = vida.ObtenerPorcentaje();
        sf::RectangleShape vidaVisual({tamano.x * proporcion, 5.0f});
        vidaVisual.setPosition({posicion.x, posicion.y - 8.0f});
        vidaVisual.setFillColor(sf::Color(90, 230, 160));
        ventana.draw(vidaVisual);
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
    sf::Vector2f tamano{740.0f, 26.0f};
};
