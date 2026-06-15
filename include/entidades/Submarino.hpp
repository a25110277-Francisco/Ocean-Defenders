#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include "componentes/CadenciaDisparo.hpp"
#include "componentes/DanoOxigeno.hpp"
#include "componentes/Duracion.hpp"
#include "componentes/Posicion.hpp"
#include "componentes/RecuperacionOxigeno.hpp"
#include "componentes/Tiempo.hpp"
#include "componentes/Velocidad.hpp"
#include "entidades/BarraOxigeno.hpp"
#include "proyectiles/Torpedo.hpp"

class Enemigo;
class PowerUp;
class Recolectable;

class Submarino {
public:
    Submarino() : posicion(430.0f, 612.0f), velocidad(260.0f, 0.0f), cadenciaDisparo(0.5f) {}

    void Actualizar(float delta, float anchoVentana) {
        cadenciaDisparo.Actualizar(delta);
        barraOxigeno.ConsumirOxigeno(delta);
        dobleDisparo.Actualizar(delta);
        velocidadAumentada.Actualizar(delta);
        posicion.x = std::clamp(posicion.x, 10.0f, anchoVentana - tamano.x - 10.0f);
    }

    void MoverIzquierda(float delta) {
        posicion.Mover(-ObtenerVelocidadActual() * delta, 0.0f);
    }

    void MoverDerecha(float delta) {
        posicion.Mover(ObtenerVelocidadActual() * delta, 0.0f);
    }

    void MoverIzquierda() {
        MoverIzquierda(1.0f / 60.0f);
    }

    void MoverDerecha() {
        MoverDerecha(1.0f / 60.0f);
    }

    Torpedo Disparar() {
        cadenciaDisparo.Reiniciar();
        return Torpedo(Posicion(posicion.x + tamano.x * 0.5f - 3.0f, posicion.y - 18.0f));
    }

    std::vector<Torpedo> DispararTorpedos() {
        std::vector<Torpedo> torpedos;
        if (!cadenciaDisparo.PuedeDisparar()) {
            return torpedos;
        }

        cadenciaDisparo.Reiniciar();
        if (dobleDisparo.Activa()) {
            torpedos.emplace_back(Posicion(posicion.x + 8.0f, posicion.y - 18.0f));
            torpedos.emplace_back(Posicion(posicion.x + tamano.x - 14.0f, posicion.y - 18.0f));
        } else {
            torpedos.emplace_back(Posicion(posicion.x + tamano.x * 0.5f - 3.0f, posicion.y - 18.0f));
        }
        return torpedos;
    }

    bool PuedeDisparar() const {
        return cadenciaDisparo.PuedeDisparar();
    }

    void RecibirDano(const DanoOxigeno& dano) {
        barraOxigeno.RecibirDano(dano);
    }

    void RecibirDano(Enemigo&) {
        barraOxigeno.RecibirDano(DanoOxigeno(10));
    }

    void ConsumirOxigeno(Tiempo tiempo) {
        barraOxigeno.ConsumirOxigeno(tiempo.ObtenerSegundos());
    }

    void RegenerarOxigeno(const RecuperacionOxigeno& recuperacion) {
        barraOxigeno.Regenerar(recuperacion);
    }

    void RegenerarOxigeno(Recolectable&) {
        barraOxigeno.Regenerar(RecuperacionOxigeno(15));
    }

    void AplicarPowerUp(PowerUp&) {}

    void RestaurarOxigeno() {
        barraOxigeno.RestaurarCompleta();
    }

    void ActivarDobleDisparo(float segundos) {
        dobleDisparo.Iniciar(segundos);
    }

    void ActivarVelocidadAumentada(float segundos) {
        velocidadAumentada.Iniciar(segundos);
    }

    void Dibujar(sf::RenderWindow& ventana, const sf::Texture* textura = nullptr) const {
        if (textura != nullptr) {
            const sf::Vector2u texturaTamano = textura->getSize();
            const sf::Vector2f tamanoVisual{66.0f, 80.0f};
            sf::Sprite sprite(*textura);
            sprite.setPosition({
                posicion.x + (tamano.x - tamanoVisual.x) * 0.5f,
                posicion.y + tamano.y - tamanoVisual.y
            });
            sprite.setScale({
                tamanoVisual.x / static_cast<float>(texturaTamano.x),
                tamanoVisual.y / static_cast<float>(texturaTamano.y)
            });
            ventana.draw(sprite);
            return;
        }

        sf::RectangleShape cuerpo(tamano);
        cuerpo.setPosition(posicion.ObtenerVector());
        cuerpo.setFillColor(sf::Color(35, 140, 220));
        ventana.draw(cuerpo);

        sf::CircleShape ventanaSubmarino(10.0f);
        ventanaSubmarino.setPosition({posicion.x + 20.0f, posicion.y + 5.0f});
        ventanaSubmarino.setFillColor(sf::Color(170, 245, 255));
        ventana.draw(ventanaSubmarino);

        sf::RectangleShape canon({8.0f, 16.0f});
        canon.setPosition({posicion.x + tamano.x * 0.5f - 4.0f, posicion.y - 12.0f});
        canon.setFillColor(sf::Color(90, 210, 235));
        ventana.draw(canon);
    }

    sf::FloatRect ObtenerBounds() const {
        return {posicion.ObtenerVector(), tamano};
    }

    const BarraOxigeno& ObtenerBarraOxigeno() const {
        return barraOxigeno;
    }

    BarraOxigeno& ObtenerBarraOxigeno() {
        return barraOxigeno;
    }

    const Posicion& ObtenerPosicion() const {
        return posicion;
    }

private:
    float ObtenerVelocidadActual() const {
        return velocidad.x * (velocidadAumentada.Activa() ? 1.25f : 1.0f);
    }

    BarraOxigeno barraOxigeno;
    Posicion posicion;
    Velocidad velocidad;
    CadenciaDisparo cadenciaDisparo;
    Duracion dobleDisparo{0.0f};
    Duracion velocidadAumentada{0.0f};
    sf::Vector2f tamano{54.0f, 30.0f};
};
