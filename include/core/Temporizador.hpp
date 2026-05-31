#pragma once

#include "componentes/Tiempo.hpp"

class Temporizador {
public:
    void Actualizar(float delta) {
        tiempo.Avanzar(delta);
    }

    void Reiniciar() {
        tiempo.Reiniciar();
    }

    float ObtenerSegundos() const {
        return tiempo.ObtenerSegundos();
    }

private:
    Tiempo tiempo;
};
