#pragma once

class Puntaje {
public:
    void Agregar(int puntos) {
        valor += puntos;
    }

    int ObtenerValor() const {
        return valor;
    }

    void Reiniciar() {
        valor = 0;
    }

private:
    int valor{0};
};
