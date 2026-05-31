#pragma once

#include <vector>
#include "entidades/Enemigo.hpp"
#include "proyectiles/ProyectilJefe.hpp"
#include "recolectables/DobleDisparo.hpp"

class Jefe : public Enemigo {
public:
    explicit Jefe(Posicion posicionInicial)
        : Enemigo("Jefe", Vida(200), DanoOxigeno(25), Velocidad(115.0f, 0.0f), CadenciaDisparo(1.2f),
                  posicionInicial, sf::Color(145, 70, 210), 2500) {
        tamano = {98.0f, 58.0f};
    }

    virtual void ActivarFase() {}

    std::unique_ptr<ProyectilEnemigo> Disparar() override {
        cadenciaDisparo.Reiniciar();
        return std::make_unique<ProyectilJefe>(ObtenerPosicionDisparo());
    }

    virtual std::unique_ptr<ProyectilJefe> DispararRafaga() {
        return std::make_unique<ProyectilJefe>(ObtenerPosicionDisparo());
    }

    virtual std::vector<std::unique_ptr<ProyectilEnemigo>> DispararRafagaTriple() {
        std::vector<std::unique_ptr<ProyectilEnemigo>> rafaga;
        rafaga.push_back(std::make_unique<ProyectilJefe>(ObtenerPosicionDisparo(), Velocidad(-80.0f, 330.0f)));
        rafaga.push_back(std::make_unique<ProyectilJefe>(ObtenerPosicionDisparo(), Velocidad(0.0f, 350.0f)));
        rafaga.push_back(std::make_unique<ProyectilJefe>(ObtenerPosicionDisparo(), Velocidad(80.0f, 330.0f)));
        cadenciaDisparo.Reiniciar();
        return rafaga;
    }

    virtual std::unique_ptr<PowerUp> LanzarPowerUp() {
        return std::make_unique<DobleDisparo>(ObtenerPosicionDisparo());
    }

    void Dibujar(sf::RenderWindow& ventana) const override {
        if (vida.EstaAgotada()) {
            return;
        }

        sf::RectangleShape cuerpo(tamano);
        cuerpo.setPosition(posicion.ObtenerVector());
        cuerpo.setFillColor(color);
        ventana.draw(cuerpo);

        for (int i = 0; i < 4; ++i) {
            sf::CircleShape tentaculo(9.0f);
            tentaculo.setPosition({posicion.x + 10.0f + i * 22.0f, posicion.y + tamano.y - 4.0f});
            tentaculo.setFillColor(sf::Color(95, 50, 170));
            ventana.draw(tentaculo);
        }
    }
};
