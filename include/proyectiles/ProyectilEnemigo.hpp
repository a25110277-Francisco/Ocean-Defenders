#pragma once

#include "componentes/DanoOxigeno.hpp"
#include "proyectiles/Proyectil.hpp"

class Arrecife;
class BarreraCoral;
class Submarino;

class ProyectilEnemigo : public Proyectil {
public:
    ProyectilEnemigo(DanoVida danoArrecifeInicial, DanoOxigeno danoOxigenoInicial, Posicion posicionInicial, Velocidad velocidadInicial, sf::Color colorInicial)
        : Proyectil(danoArrecifeInicial, posicionInicial, velocidadInicial, {10.4f, 20.8f}, colorInicial), danoOxigeno(danoOxigenoInicial) {}

    int ObtenerDanoOxigeno() const {
        return danoOxigeno.ObtenerCantidad();
    }

    void ImpactarSubmarino(Submarino&) {
        Impactar();
    }

    void ImpactarArrecife(Arrecife&) {
        Impactar();
    }

    void ImpactarBarrera(BarreraCoral&) {
        Impactar();
    }

private:
    DanoOxigeno danoOxigeno;
};
