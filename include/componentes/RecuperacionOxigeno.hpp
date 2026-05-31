#pragma once

class RecuperacionOxigeno {
public:
    explicit RecuperacionOxigeno(int cantidadInicial = 0) : cantidad(cantidadInicial) {}

    int ObtenerCantidad() const {
        return cantidad;
    }

private:
    int cantidad{0};
};
