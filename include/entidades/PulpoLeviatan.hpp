#pragma once

#include <cmath>
#include "componentes/FaseJefe.hpp"
#include "entidades/Jefe.hpp"

class PulpoLeviatan : public Jefe {
public:
    explicit PulpoLeviatan(Posicion posicionInicial = Posicion(400.0f, 120.0f)) : Jefe(posicionInicial) {}

    void ActivarFase() override {
        faseJefe.ActualizarPorVida(vida.ObtenerActual());
    }

    void MoverErratico(float delta, float anchoVentana) {
        tiempo += delta;
        posicion.Mover(movimientoActual.x * delta, movimientoActual.y * delta);

        if (std::sin(tiempo * 1.3f) > 0.96f) {
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
        ActivarFase();
    }

    void MoverErratico() {
        MoverErratico(1.0f / 60.0f, 900.0f);
    }

    bool EstaEnFaseDos() const {
        return faseJefe.EsFaseDos();
    }

    const FaseJefe& ObtenerFaseJefe() const {
        return faseJefe;
    }

private:
    FaseJefe faseJefe;
    Velocidad movimientoActual{145.0f, 85.0f};
    float tiempo{0.0f};
};
