#pragma once

class Tiempo {
public:
    explicit Tiempo(float segundosIniciales = 0.0f) : segundos(segundosIniciales) {}

    float ObtenerSegundos() const {
        return segundos;
    }

    void Asignar(float nuevosSegundos) {
        segundos = nuevosSegundos;
    }

    void Avanzar(float delta) {
        segundos += delta;
    }

    void Reiniciar() {
        segundos = 0.0f;
    }

private:
    float segundos{0.0f};
};
