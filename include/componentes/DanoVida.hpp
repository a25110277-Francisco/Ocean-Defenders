#pragma once

class DanoVida {
public:
    explicit DanoVida(int cantidadInicial = 0) : cantidad(cantidadInicial) {}

    int ObtenerCantidad() const {
        return cantidad;
    }

private:
    int cantidad{0};
};
