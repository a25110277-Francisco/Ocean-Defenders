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
        const float direccion = std::sin(tiempo * 1.7f) >= 0.0f ? 1.0f : -1.0f;
        posicion.Mover(direccion * velocidad.x * delta, std::sin(tiempo * 2.3f) * 22.0f * delta);

        if (posicion.x < 20.0f) {
            posicion.x = 20.0f;
        }
        if (posicion.x + tamano.x > anchoVentana - 20.0f) {
            posicion.x = anchoVentana - 20.0f - tamano.x;
        }
        if (posicion.y < 92.0f) {
            posicion.y = 92.0f;
        }
        if (posicion.y > 235.0f) {
            posicion.y = 235.0f;
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
    float tiempo{0.0f};
};
