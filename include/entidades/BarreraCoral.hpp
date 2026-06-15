#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
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

    void Dibujar(sf::RenderWindow& ventana, const sf::Texture* textura = nullptr) const {
        if (vida.EstaAgotada()) {
            return;
        }

        sf::Color color = sf::Color::White;
        if (estadoBarrera.ObtenerTipo() == EstadoBarrera::Tipo::Agrietada) {
            color = sf::Color(190, 190, 190);
        } else if (estadoBarrera.ObtenerTipo() == EstadoBarrera::Tipo::Fracturada) {
            color = sf::Color(125, 125, 125);
        }

        if (textura != nullptr) {
            constexpr int CantidadEtapas = 6;
            constexpr std::array<sf::IntRect, CantidadEtapas> Recortes{{
                sf::IntRect({23, 259}, {384, 204}),
                sf::IntRect({434, 254}, {309, 212}),
                sf::IntRect({776, 264}, {308, 209}),
                sf::IntRect({1144, 282}, {290, 193}),
                sf::IntRect({1497, 363}, {277, 112}),
                sf::IntRect({1888, 407}, {213, 68})
            }};
            const int impactosRecibidos = (vida.ObtenerMaxima() - vida.ObtenerActual()) / 5;
            const int etapa = std::clamp(impactosRecibidos, 0, CantidadEtapas - 1);
            const sf::IntRect& recorte = Recortes[etapa];
            const float escala = tamano.x / static_cast<float>(Recortes[0].size.x);
            const sf::Vector2f tamanoSprite{
                static_cast<float>(recorte.size.x) * escala,
                static_cast<float>(recorte.size.y) * escala
            };

            sf::Sprite sprite(*textura, recorte);
            sprite.setPosition({
                posicion.x + (tamano.x - tamanoSprite.x) * 0.5f,
                posicion.y + tamano.y - tamanoSprite.y
            });
            sprite.setScale({escala, escala});
            ventana.draw(sprite);
            return;
        }

        color = sf::Color(35, 220, 150);
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
    sf::Vector2f tamano{120.0f, 70.0f};
};
