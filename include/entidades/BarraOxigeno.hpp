#pragma once

#include "componentes/DanoOxigeno.hpp"
#include "componentes/RecuperacionOxigeno.hpp"
#include "componentes/Vida.hpp"

class BarraOxigeno {
public:
    BarraOxigeno() : oxigeno(100) {}

    void ConsumirOxigeno(float delta) {
        temporizadorPerdida += delta;
        while (temporizadorPerdida >= 5.0f) {
            oxigeno.RecibirDano(2);
            temporizadorPerdida -= 5.0f;
        }
    }

    void RecibirDano(const DanoOxigeno& dano) {
        oxigeno.RecibirDano(dano.ObtenerCantidad());
    }

    void Regenerar(const RecuperacionOxigeno& recuperacion) {
        oxigeno.Recuperar(recuperacion.ObtenerCantidad());
    }

    void RestaurarCompleta() {
        oxigeno.RestaurarCompleta();
    }

    int ObtenerActual() const {
        return oxigeno.ObtenerActual();
    }

    int ObtenerMaximo() const {
        return oxigeno.ObtenerMaxima();
    }

    float ObtenerPorcentaje() const {
        return oxigeno.ObtenerPorcentaje();
    }

    bool EstaAgotada() const {
        return oxigeno.EstaAgotada();
    }

private:
    Vida oxigeno;
    float temporizadorPerdida{0.0f};
};
