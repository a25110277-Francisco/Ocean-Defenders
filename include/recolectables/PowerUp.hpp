#pragma once

#include "componentes/Duracion.hpp"
#include "recolectables/Recolectable.hpp"

class PowerUp : public Recolectable {
public:
    PowerUp(Posicion posicionInicial, float segundosDuracion, sf::Color colorInicial)
        : Recolectable(posicionInicial, colorInicial), duracion(segundosDuracion) {}

    virtual void Activar(Submarino& submarino) = 0;

    virtual void Expirar() {
        duracion.Iniciar(0.0f);
    }

    void Recolectar(Submarino& submarino) override {
        Activar(submarino);
        Desactivar();
    }

protected:
    Duracion duracion;
};
