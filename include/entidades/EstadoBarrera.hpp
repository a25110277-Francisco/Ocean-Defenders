#pragma once

class EstadoBarrera {
public:
    enum class Tipo {
        Intacta,
        Agrietada,
        Fracturada,
        Destruida
    };

    EstadoBarrera() = default;
    explicit EstadoBarrera(Tipo tipoInicial) : tipo(tipoInicial) {}

    Tipo ObtenerTipo() const {
        return tipo;
    }

    void ActualizarPorVida(int vida) {
        if (vida <= 0) {
            tipo = Tipo::Destruida;
        } else if (vida <= 14) {
            tipo = Tipo::Fracturada;
        } else if (vida <= 29) {
            tipo = Tipo::Agrietada;
        } else {
            tipo = Tipo::Intacta;
        }
    }

private:
    Tipo tipo{Tipo::Intacta};
};
