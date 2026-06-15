#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <utility>
#include "componentes/CadenciaDisparo.hpp"
#include "componentes/DanoOxigeno.hpp"
#include "componentes/Posicion.hpp"
#include "componentes/ProbabilidadPowerUp.hpp"
#include "componentes/Velocidad.hpp"
#include "componentes/Vida.hpp"
#include "core/EstadoJuego.hpp"
#include "proyectiles/ProyectilCalamar.hpp"
#include "proyectiles/ProyectilCangrejo.hpp"
#include "proyectiles/ProyectilEnemigo.hpp"
#include "proyectiles/ProyectilMedusa.hpp"
#include "proyectiles/Torpedo.hpp"
#include "recolectables/PowerUp.hpp"

class Enemigo {
public:
    Enemigo(std::string nombreInicial, Vida vidaInicial, DanoOxigeno danoOxigenoInicial, Velocidad velocidadInicial,
            CadenciaDisparo cadenciaInicial, Posicion posicionInicial, sf::Color colorInicial, int puntajeInicial)
        : vida(vidaInicial), danoOxigeno(danoOxigenoInicial), velocidad(velocidadInicial), cadenciaDisparo(cadenciaInicial),
          posicion(posicionInicial), nombre(std::move(nombreInicial)), color(colorInicial), puntaje(puntajeInicial) {}

    virtual ~Enemigo() = default;

    virtual void MoverFormacion(float direccion, float delta, float multiplicadorVelocidad) {
        posicion.Mover(direccion * velocidad.x * multiplicadorVelocidad * delta, 0.0f);
        cadenciaDisparo.Actualizar(delta);
    }

    virtual void DescenderFila() {
        posicion.Mover(0.0f, 28.0f);
    }

    virtual std::unique_ptr<ProyectilEnemigo> Disparar() {
        cadenciaDisparo.Reiniciar();
        return std::make_unique<ProyectilEnemigo>(DanoVida(5), danoOxigeno, ObtenerPosicionDisparo(), Velocidad(0.0f, 260.0f), color);
    }

    virtual void RecibirDano(const Torpedo& torpedo) {
        vida.RecibirDano(torpedo.ObtenerDanoVida());
    }

    void RecibirDano(int dano) {
        vida.RecibirDano(dano);
    }

    virtual std::unique_ptr<PowerUp> SoltarPowerUp() {
        return nullptr;
    }

    EstadoJuego EstaDestruido() const {
        return EstadoJuego(EstadoJuego::Tipo::Jugando);
    }

    bool Destruido() const {
        return vida.EstaAgotada();
    }

    bool PuedeDisparar() const {
        return cadenciaDisparo.PuedeDisparar();
    }

    void ReiniciarDisparo() {
        cadenciaDisparo.Reiniciar();
    }

    virtual void Dibujar(sf::RenderWindow& ventana, const sf::Texture* textura = nullptr, float tiempoAnimacion = 0.0f) const {
        if (vida.EstaAgotada()) {
            return;
        }

        if (textura != nullptr) {
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
            const sf::Vector2f tamanoVisual{67.5f, 60.0f};
            sprite.setPosition({
                posicion.x + (tamano.x - tamanoVisual.x) * 0.5f,
                posicion.y + (tamano.y - tamanoVisual.y) * 0.5f
            });
            sprite.setScale({
                tamanoVisual.x / static_cast<float>(recorte.size.x),
                tamanoVisual.y / static_cast<float>(recorte.size.y)
            });
            ventana.draw(sprite);
            return;
        }

        sf::RectangleShape cuerpo(tamano);
        cuerpo.setPosition(posicion.ObtenerVector());
        cuerpo.setFillColor(color);
        ventana.draw(cuerpo);

        sf::CircleShape ojo(5.0f);
        ojo.setPosition({posicion.x + tamano.x * 0.5f - 5.0f, posicion.y + 8.0f});
        ojo.setFillColor(sf::Color(245, 250, 255));
        ventana.draw(ojo);
    }

    sf::FloatRect ObtenerBounds() const {
        return {posicion.ObtenerVector(), tamano};
    }

    bool TocaBorde(float anchoVentana) const {
        return posicion.x <= 15.0f || posicion.x + tamano.x >= anchoVentana - 15.0f;
    }

    bool LlegoAlFondo(float limiteY) const {
        return posicion.y > limiteY;
    }

    Posicion ObtenerPosicionDisparo() const {
        return {posicion.x + tamano.x * 0.5f - 4.0f, posicion.y + tamano.y};
    }

    const Posicion& ObtenerPosicion() const {
        return posicion;
    }

    int ObtenerPuntaje() const {
        return puntaje;
    }

    const Vida& ObtenerVida() const {
        return vida;
    }

    const DanoOxigeno& ObtenerDanoOxigeno() const {
        return danoOxigeno;
    }

    float ObtenerProbabilidadPowerUp() const {
        return probabilidadPowerUp.ObtenerValor();
    }

    const std::string& ObtenerNombre() const {
        return nombre;
    }

protected:
    Vida vida;
    DanoOxigeno danoOxigeno;
    Velocidad velocidad;
    CadenciaDisparo cadenciaDisparo;
    Posicion posicion;
    ProbabilidadPowerUp probabilidadPowerUp{0.2f};
    std::string nombre;
    sf::Color color;
    int puntaje{0};
    sf::Vector2f tamano{52.5f, 37.5f};
};
