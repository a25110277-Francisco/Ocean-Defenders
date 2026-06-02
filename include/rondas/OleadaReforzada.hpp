#pragma once

#include "rondas/Ronda.hpp"

class OleadaReforzada : public Ronda {
public:
    OleadaReforzada() : Ronda("Oleada Reforzada", 2, 6, 6, 6, false, 1.25f) {}
};
