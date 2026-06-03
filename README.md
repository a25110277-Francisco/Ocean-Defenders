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
- Las barreras de coral tienen 40 HP, bloquean proyectiles enemigos y se regeneran al cambiar de ronda. Los torpedos del submarino las atraviesan sin causar dano.
- Hay 4 barreras en la ronda 1, 3 en la ronda 2 y 2 en las rondas 3 y 4.
- Las rondas 1 a 3 usan una fila de medusas, una de calamares y una de cangrejos con 5, 6 y 7 enemigos por fila respectivamente.
- La ronda 4 enfrenta al Pulpo Leviatan con dos fases de disparo.
- Los enemigos pueden soltar power-ups y las burbujas restauran oxigeno cada 15 segundos.

## Tecnologia

- Lenguaje: C++17.
- Libreria grafica: SFML 3.0.
- Estructura: clases inline en headers dentro de `include`.
- Visuales: primitivas SFML con rectangulos y circulos.

## Creditos

- Fuente HUD: Noto Sans, distribuida bajo SIL Open Font License.
