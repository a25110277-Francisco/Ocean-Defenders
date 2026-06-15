#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
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

    void Dibujar(sf::RenderWindow& ventana, const sf::Texture* textura = nullptr) const {
        if (vida.EstaAgotada()) {
            return;
        }

        if (textura != nullptr) {
            constexpr int CantidadEtapas = 6;
            const sf::Vector2u texturaTamano = textura->getSize();
            const int etapa = std::clamp(
                CantidadEtapas - static_cast<int>(vida.ObtenerPorcentaje() * CantidadEtapas + 0.99f),
                0,
                CantidadEtapas - 1
            );
            const int inicioY = etapa * static_cast<int>(texturaTamano.y) / CantidadEtapas;
            const int finY = (etapa + 1) * static_cast<int>(texturaTamano.y) / CantidadEtapas;
            const float altoVisual = 88.0f;
            const float posicionVisualY = posicion.y + tamano.y - altoVisual;
            const sf::IntRect recorte(
                {0, inicioY},
                {static_cast<int>(texturaTamano.x), finY - inicioY}
            );
            sf::Sprite sprite(*textura, recorte);
            sprite.setPosition({posicion.x, posicionVisualY});
            sprite.setScale({
                tamano.x / static_cast<float>(recorte.size.x),
                altoVisual / static_cast<float>(recorte.size.y)
            });
            ventana.draw(sprite);
            return;
        }

        sf::RectangleShape base(tamano);
        base.setPosition(posicion.ObtenerVector());
        base.setFillColor(sf::Color(45, 150, 110));
        ventana.draw(base);
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
