#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <algorithm>
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
            const sf::Vector2u texturaTamano = textura->getSize();
            const int recorteY = static_cast<int>(static_cast<float>(texturaTamano.y) * 0.20f);
            const int recorteAlto = static_cast<int>(static_cast<float>(texturaTamano.y) * 0.62f);
            constexpr int CantidadFragmentos = 8;
            constexpr std::array<int, CantidadFragmentos> ordenDestruccion{1, 6, 3, 7, 0, 5, 2, 4};
            const int fragmentosVisibles = std::clamp(
                static_cast<int>(vida.ObtenerPorcentaje() * CantidadFragmentos + 0.99f),
                1,
                CantidadFragmentos
            );
            const int anchoFuente = static_cast<int>(texturaTamano.x) / CantidadFragmentos;
            const float anchoDestino = tamano.x / static_cast<float>(CantidadFragmentos);

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
                const sf::IntRect recorte({fuenteX, recorteY}, {anchoActual, recorteAlto});
                sf::Sprite sprite(*textura, recorte);
                sprite.setPosition({
                    posicion.x + anchoDestino * static_cast<float>(fragmento),
                    posicion.y + (fragmento % 2 == 0 ? 0.0f : 2.0f)
                });
                sprite.setScale({
                    anchoDestino / static_cast<float>(anchoActual),
                    tamano.y / static_cast<float>(recorteAlto)
                });
                sprite.setColor(color);
                ventana.draw(sprite);
            }
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
    sf::Vector2f tamano{110.0f, 70.0f};
};
