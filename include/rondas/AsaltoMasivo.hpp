#pragma once

#include "rondas/Ronda.hpp"

class AsaltoMasivo : public Ronda {
public:
    AsaltoMasivo() : Ronda("Asalto Masivo", 3, 4, 8, 9, false, 1.5f) {}
};
