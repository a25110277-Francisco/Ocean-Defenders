#pragma once

#include "rondas/Ronda.hpp"

class AsaltoMasivo : public Ronda {
public:
    AsaltoMasivo() : Ronda("Asalto Masivo", 3, 7, 7, 7, false, 1.3f) {}
};
