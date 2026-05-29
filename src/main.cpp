#include <iostream>
#include <SFML/Graphics.hpp>
#include "core/Juego.hpp"

/**
 * Ocean Defenders - Space Invaders Submarino
 * Juego desarrollado en C++ con SFML 3
 * 
 * @author Team Ocean Defenders
 * @version 0.1 (FASE 0: Setup & Estructura)
 */

int main()
{
    try
    {
        Juego juego;
        juego.ejecutar();
    }
    catch (const std::exception& excepcion)
    {
        std::cerr << "Error: " << excepcion.what() << std::endl;
        return 1;
    }
    
    return 0;
}
