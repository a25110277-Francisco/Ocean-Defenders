#pragma once

#include "rondas/Ronda.hpp"

class InvasionInicial : public Ronda {
public:
    InvasionInicial() : Ronda("Invasion Inicial", 1, 6, 6, 3, false, 1.0f) {}
};
