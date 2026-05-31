#pragma once

#include <algorithm>

class Vida {
public:
    explicit Vida(int maximaInicial = 1) : actual(maximaInicial), maxima(maximaInicial) {}

    int ObtenerActual() const {
        return actual;
    }

    int ObtenerMaxima() const {
        return maxima;
    }

    float ObtenerPorcentaje() const {
        if (maxima <= 0) {
            return 0.0f;
        }
        return static_cast<float>(actual) / static_cast<float>(maxima);
    }

    void RecibirDano(int cantidad) {
        actual = std::max(0, actual - std::max(0, cantidad));
    }

    void Recuperar(int cantidad) {
        actual = std::min(maxima, actual + std::max(0, cantidad));
    }

    void RestaurarCompleta() {
        actual = maxima;
    }

    bool EstaAgotada() const {
        return actual <= 0;
    }

private:
    int actual{1};
    int maxima{1};
};
