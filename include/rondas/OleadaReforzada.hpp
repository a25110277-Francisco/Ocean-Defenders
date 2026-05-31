#pragma once

#include "rondas/Ronda.hpp"

class OleadaReforzada : public Ronda {
public:
    OleadaReforzada() : Ronda("Oleada Reforzada", 2, 5, 7, 6, false, 1.25f) {}
};
