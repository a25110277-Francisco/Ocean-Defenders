#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
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
            constexpr int CantidadFragmentos = 12;
            constexpr std::array<int, CantidadFragmentos> ordenDestruccion{5, 1, 9, 3, 11, 7, 0, 10, 4, 8, 2, 6};
            const int fragmentosVisibles = std::clamp(
                static_cast<int>(vida.ObtenerPorcentaje() * CantidadFragmentos + 0.99f),
                1,
                CantidadFragmentos
            );
            const sf::Vector2u texturaTamano = textura->getSize();
            const int anchoFuente = static_cast<int>(texturaTamano.x) / CantidadFragmentos;
            const float anchoDestino = tamano.x / static_cast<float>(CantidadFragmentos);
            const float altoVisual = 88.0f;
            const float posicionVisualY = posicion.y + tamano.y - altoVisual;

            sf::Color color = sf::Color::White;
            if (vida.ObtenerPorcentaje() <= 0.35f) {
                color = sf::Color(125, 125, 125);
            } else if (vida.ObtenerPorcentaje() <= 0.70f) {
                color = sf::Color(190, 190, 190);
            }

            for (int fragmento = 0; fragmento < CantidadFragmentos; ++fragmento) {
                const auto posicionOrden = std::find(ordenDestruccion.begin(), ordenDestruccion.end(), fragmento);
                const int etapaDestruccion = static_cast<int>(std::distance(ordenDestruccion.begin(), posicionOrden));
                if (etapaDestruccion >= fragmentosVisibles) {
                    continue;
                }

                const int fuenteX = fragmento * anchoFuente;
                const int anchoActual = fragmento == CantidadFragmentos - 1
                    ? static_cast<int>(texturaTamano.x) - fuenteX
                    : anchoFuente;
                const sf::IntRect recorte(
                    {fuenteX, 0},
                    {anchoActual, static_cast<int>(texturaTamano.y)}
                );
                sf::Sprite sprite(*textura, recorte);
                sprite.setPosition({
                    posicion.x + anchoDestino * static_cast<float>(fragmento),
                    posicionVisualY + (fragmento % 3 == 0 ? 3.0f : 0.0f)
                });
                sprite.setScale({
                    anchoDestino / static_cast<float>(anchoActual),
                    altoVisual / static_cast<float>(texturaTamano.y)
                });
                sprite.setColor(color);
                ventana.draw(sprite);
            }
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
