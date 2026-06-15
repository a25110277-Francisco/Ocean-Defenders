#pragma once

#include <algorithm>
#include <cmath>
#include "entidades/Jefe.hpp"

class PezGlobo : public Jefe {
public:
    explicit PezGlobo(Posicion posicionInicial = Posicion(380.0f, 120.0f)) : Jefe(posicionInicial) {
        nombre = "PezGlobo";
        tamano = {130.0f, 130.0f};
    }

    void MoverErratico(float delta, float anchoVentana) {
        tiempo += delta;
        posicion.Mover(movimientoActual.x * delta, movimientoActual.y * delta);

        if (std::sin(tiempo * 1.5f) > 0.97f) {
            movimientoActual.y = -movimientoActual.y;
        }

        if (posicion.x < 20.0f) {
            posicion.x = 20.0f;
            movimientoActual.x = std::abs(movimientoActual.x);
        }
        if (posicion.x + tamano.x > anchoVentana - 20.0f) {
            posicion.x = anchoVentana - 20.0f - tamano.x;
            movimientoActual.x = -std::abs(movimientoActual.x);
        }
        if (posicion.y < 92.0f) {
            posicion.y = 92.0f;
            movimientoActual.y = std::abs(movimientoActual.y);
        }
        if (posicion.y > 350.0f) {
            posicion.y = 350.0f;
            movimientoActual.y = -std::abs(movimientoActual.y);
        }

        cadenciaDisparo.Actualizar(delta);
    }

    void MoverErratico() {
        MoverErratico(1.0f / 60.0f, 900.0f);
    }

    void Dibujar(sf::RenderWindow& ventana, const sf::Texture* textura = nullptr, float tiempoAnimacion = 0.0f) const override {
        if (vida.EstaAgotada()) {
            return;
        }

        if (textura == nullptr) {
            Jefe::Dibujar(ventana);
            return;
        }

        constexpr int CantidadCuadros = 4;
        const sf::Vector2u texturaTamano = textura->getSize();
        const int cuadro = static_cast<int>(tiempoAnimacion / 0.16f) % CantidadCuadros;
        const int inicioX = cuadro * static_cast<int>(texturaTamano.x) / CantidadCuadros;
        const int finX = (cuadro + 1) * static_cast<int>(texturaTamano.x) / CantidadCuadros;
        const sf::IntRect recorte(
            {inicioX, 0},
            {finX - inicioX, static_cast<int>(texturaTamano.y)}
        );
        sf::Sprite sprite(*textura, recorte);
        const float escala = std::min(
            tamano.x / static_cast<float>(recorte.size.x),
            tamano.y / static_cast<float>(recorte.size.y)
        );
        const sf::Vector2f tamanoVisual{
            static_cast<float>(recorte.size.x) * escala,
            static_cast<float>(recorte.size.y) * escala
        };
        sprite.setPosition({
            posicion.x + (tamano.x - tamanoVisual.x) * 0.5f,
            posicion.y + (tamano.y - tamanoVisual.y) * 0.5f
        });
        sprite.setScale({escala, escala});
        ventana.draw(sprite);
    }

private:
    Velocidad movimientoActual{155.0f, 95.0f};
    float tiempo{0.0f};
};
