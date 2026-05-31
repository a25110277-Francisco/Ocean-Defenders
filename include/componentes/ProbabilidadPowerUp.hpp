#pragma once

class ProbabilidadPowerUp {
public:
    explicit ProbabilidadPowerUp(float valorInicial = 0.2f) : valor(valorInicial) {}

    float ObtenerValor() const {
        return valor;
    }

private:
    float valor{0.2f};
};
