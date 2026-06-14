# Ocean Defenders

Juego estilo Space Invaders submarino hecho en C++ con SFML 3. El jugador controla un submarino, protege el arrecife y derrota cuatro rondas de enemigos usando torpedos, barreras de coral, burbujas de oxigeno y power-ups.

Al abrir el ejecutable se muestra una pantalla de titulo con el escenario congelado. La partida inicia al presionar Enter.

## Controles

- A/D o flechas izquierda/derecha: mover el submarino.
- Space: disparar torpedos.
- Escape: cerrar el juego.
- Enter: iniciar desde la pantalla de titulo o regresar a ella despues de victoria o derrota.

## Compilar y ejecutar

```bash
make
make runOceanDefenders
```

El ejecutable principal se genera como `bin/OceanDefenders.exe`.

## Mecanicas principales

- El submarino solo se mueve horizontalmente y dispara cada 0.5 segundos.
- El oxigeno inicia en 100, pierde 2 puntos cada 5 segundos y baja al recibir proyectiles enemigos.
- El arrecife inicia con 1300 HP y no se regenera entre rondas.
- Las barreras de coral usan alternadamente las imagenes `wall1.png` y `wall2.png`, tienen 40 HP, bloquean proyectiles enemigos y se regeneran al cambiar de ronda. Los torpedos del submarino las atraviesan sin causar dano.
- Hay 4 barreras en la ronda 1, 3 en la ronda 2 y 2 en las rondas 3 y 4.
- Las rondas 1 a 3 usan una fila de medusas, una de calamares y una de cangrejos con 5, 6 y 7 enemigos por fila respectivamente.
- La ronda 4 enfrenta al Pulpo Leviatan con dos fases de disparo, movimiento libre y barra de vida visible.
- La bandera roja marca el limite de invasion: si un enemigo llega ahi, pierdes.
- Los enemigos pueden soltar power-ups con colores distintos y las burbujas restauran oxigeno cada 15 segundos.
- El power-up de velocidad aumenta el movimiento del submarino en 25%.
- La vista mantiene proporcion al redimensionar o usar pantalla completa para evitar que se estire.

## Tecnologia

- Lenguaje: C++17.
- Libreria grafica: SFML 3.0.
- Estructura: clases inline en headers dentro de `include`.
- Visuales: primitivas SFML e imagenes PNG para las barreras.
- Fuente principal: Pixel Digivolve, cargada desde `assets/fonts/PIXEL LETTER`.
- Musica principal: `assets/music/principal.ogg`, reproducida en bucle durante la partida.

## Creditos

- Fuente de respaldo: Noto Sans, distribuida bajo SIL Open Font License.
