#pragma once

#include <string>

class EstadoJuego {
public:
    enum class Tipo {
        Jugando,
        Victoria,
        DerrotaOxigeno,
        DerrotaArrecife,
        DerrotaInvasion
    };

    EstadoJuego() = default;
    explicit EstadoJuego(Tipo tipoInicial) : tipo(tipoInicial) {}

    Tipo ObtenerTipo() const {
        return tipo;
    }

    void Asignar(Tipo nuevoTipo) {
        tipo = nuevoTipo;
    }

    bool EstaJugando() const {
        return tipo == Tipo::Jugando;
    }

    bool Termino() const {
        return tipo != Tipo::Jugando;
    }

    std::string ObtenerMensaje() const {
        switch (tipo) {
            case Tipo::Victoria:
                return "Victoria: derrotaste al Pulpo Leviatan";
            case Tipo::DerrotaOxigeno:
                return "Derrota: oxigeno agotado";
            case Tipo::DerrotaArrecife:
                return "Derrota: arrecife destruido";
            case Tipo::DerrotaInvasion:
                return "Derrota: los enemigos llegaron al fondo";
            case Tipo::Jugando:
            default:
                return "Jugando";
        }
    }

private:
    Tipo tipo{Tipo::Jugando};
};
