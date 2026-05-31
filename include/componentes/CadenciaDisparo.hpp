#pragma once

class CadenciaDisparo {
public:
    explicit CadenciaDisparo(float intervaloInicial = 1.0f) : intervalo(intervaloInicial), acumulado(intervaloInicial) {}

    void Actualizar(float delta) {
        acumulado += delta;
    }

    bool PuedeDisparar() const {
        return acumulado >= intervalo;
    }

    void Reiniciar() {
        acumulado = 0.0f;
    }

    float ObtenerIntervalo() const {
        return intervalo;
    }

    void AsignarIntervalo(float nuevoIntervalo) {
        intervalo = nuevoIntervalo;
    }

private:
    float intervalo{1.0f};
    float acumulado{0.0f};
};
