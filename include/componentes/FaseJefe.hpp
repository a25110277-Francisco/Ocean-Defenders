#pragma once

class FaseJefe {
public:
    enum class Tipo {
        Fase1,
        Fase2
    };

    explicit FaseJefe(Tipo tipoInicial = Tipo::Fase1) : tipo(tipoInicial) {}

    Tipo ObtenerTipo() const {
        return tipo;
    }

    void ActualizarPorVida(int vidaActual) {
        tipo = vidaActual <= 100 ? Tipo::Fase2 : Tipo::Fase1;
    }

    bool EsFaseDos() const {
        return tipo == Tipo::Fase2;
    }

private:
    Tipo tipo{Tipo::Fase1};
};
