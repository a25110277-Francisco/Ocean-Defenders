#pragma once

#include "rondas/Ronda.hpp"

class RondaJefe : public Ronda {
public:
    RondaJefe() : Ronda("Ronda Jefe", 4, 0, 0, 0, true, 1.0f) {}
};
