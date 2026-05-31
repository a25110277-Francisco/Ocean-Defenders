#pragma once

class DanoOxigeno {
public:
    explicit DanoOxigeno(int cantidadInicial = 0) : cantidad(cantidadInicial) {}

    int ObtenerCantidad() const {
        return cantidad;
    }

private:
    int cantidad{0};
};
