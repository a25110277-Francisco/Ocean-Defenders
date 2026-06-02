#pragma once

#include "rondas/Ronda.hpp"

class InvasionInicial : public Ronda {
public:
    InvasionInicial() : Ronda("Invasion Inicial", 1, 5, 5, 5, false, 1.0f) {}
};
