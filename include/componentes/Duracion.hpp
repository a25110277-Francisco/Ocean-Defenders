#pragma once

class Duracion {
public:
    explicit Duracion(float segundosIniciales = 0.0f) : segundos(segundosIniciales), restante(segundosIniciales) {}

    void Iniciar(float nuevosSegundos) {
        segundos = nuevosSegundos;
        restante = nuevosSegundos;
    }

    void Actualizar(float delta) {
        if (restante > 0.0f) {
            restante -= delta;
            if (restante < 0.0f) {
                restante = 0.0f;
            }
        }
    }

    bool Activa() const {
        return restante > 0.0f;
    }

    bool Expirada() const {
        return restante <= 0.0f;
    }

    float ObtenerRestante() const {
        return restante;
    }

    float ObtenerSegundos() const {
        return segundos;
    }

private:
    float segundos{0.0f};
    float restante{0.0f};
};
