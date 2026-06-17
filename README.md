# Ocean Defenders

Juego estilo Space Invaders submarino hecho en C++ con SFML 3. El jugador controla un submarino, protege el arrecife y derrota cuatro rondas de enemigos usando torpedos, barreras de coral, burbujas de oxigeno y power-ups.

Al abrir el ejecutable se muestra una portada. Al presionar Enter aparecen las indicaciones y una cuenta de 7 segundos antes de iniciar la partida.

## Objetivo del juego

El objetivo de Ocean Defenders es proteger el arrecife durante cuatro rondas de ataque enemigo. El jugador debe controlar el submarino, destruir las formaciones de cangrejos, calamares y medusas, evitar que crucen el limite de invasion y conservar suficiente oxigeno para llegar a la ronda final. La victoria se consigue al derrotar al Pez Globo, jefe de la cuarta ronda.

El jugador pierde si el oxigeno del submarino llega a 0, si el arrecife se queda sin vida o si algun enemigo alcanza el limite marcado en la pantalla.

## Caracteristicas

- Juego 2D estilo Space Invaders con tematica submarina.
- Programacion orientada a objetos con clases inline en archivos `.hpp`.
- Graficos con SFML 3.0 usando sprites, rectangulos, circulos y triangulos.
- Portada inicial, pantalla de indicaciones y contador antes de comenzar.
- Cuatro rondas progresivas con distinta cantidad de enemigos, barreras y velocidad.
- Submarino con movimiento horizontal, disparo de torpedos y sistema de oxigeno.
- Barreras de coral regeneradas por ronda, con sprites de destruccion por dano.
- Arrecife con 1000 HP persistentes durante toda la partida.
- Power-ups de doble disparo, velocidad aumentada y recuperacion de oxigeno.
- Jefe final Pez Globo con movimiento libre, barra de vida y rafagas de 3 proyectiles.
- Musica de fondo, sonidos de disparo, victoria y derrota.
- Fondo, portada, enemigos, submarino, torpedos, power-ups, barreras y arrecife con imagenes PNG.

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
- El arrecife inicia con 1000 HP y no se regenera entre rondas.
- El arrecife usa la imagen completa `reef.png`, sin etapas de destruccion. Su barra de vida se muestra solamente en el HUD superior.
- Las barreras usan las 6 etapas de destruccion incluidas en `wall.png`, conservando la proporcion original del sprite.
- Hay 4 barreras en la ronda 1, 3 en la ronda 2 y 2 en las rondas 3 y 4.
- Las rondas 1 a 3 muestran cangrejos en la primera fila, calamares en la segunda y medusas en la tercera.
- Cangrejos, calamares y medusas usan animaciones de movimiento de 4 cuadros y animaciones `Dead` de 4 cuadros al ser eliminados.
- Los enemigos normales recibieron un segundo aumento de 25% en su tamaño.
- La ronda 4 enfrenta al Pez Globo, que se mueve libremente y dispara tres triangulos rojos por rafaga.
- Entre rondas aparece una pausa de 3 segundos antes de iniciar la siguiente formacion.
- El submarino, sus torpedos, el jefe y los power-ups utilizan sus imagenes PNG correspondientes.
- El submarino reproduce su animacion `Dead` cuando se agota el oxigeno.
- La bandera roja marca el limite de invasion: pierdes cuando un enemigo la cruza.
- Los enemigos pueden soltar power-ups con colores distintos y las burbujas restauran oxigeno cada 15 segundos.
- El power-up de velocidad aumenta el movimiento del submarino en 25%.
- La vista mantiene proporcion al redimensionar o usar pantalla completa para evitar que se estire.

## Caracteristicas

- Juego 2D estilo Space Invaders con tematica submarina.
- Programacion orientada a objetos con clases inline en archivos `.hpp`.
- Graficos con SFML 3.0 usando sprites, rectangulos, circulos y triangulos.
- Portada inicial, pantalla de indicaciones y contador antes de comenzar.
- Cuatro rondas progresivas con distinta cantidad de enemigos, barreras y velocidad.
- Submarino con movimiento horizontal, disparo de torpedos y sistema de oxigeno.
- Barreras de coral regeneradas por ronda, con sprites de destruccion por dano.
- Arrecife con 1000 HP persistentes durante toda la partida.
- Power-ups de doble disparo, velocidad aumentada y recuperacion de oxigeno.
- Jefe final Pez Globo con movimiento libre, barra de vida y rafagas de 3 proyectiles.
- Musica de fondo, sonidos de disparo, victoria y derrota.
- Fondo, portada, enemigos, submarino, torpedos, power-ups, barreras y arrecife con imagenes PNG.

## Equipo

- Lider: Francisco Yael Partida Garduño (@a25110277-Francisco)

## Tecnologia

- Lenguaje: C++17.
- Libreria grafica: SFML 3.0.
- Estructura: clases inline en headers dentro de `include`.

## Creditos

- Inspiración principal: Space Invaders
- Fuente de respaldo: Noto Sans, distribuida bajo SIL Open Font License.
- Visuales: primitivas SFML e imagenes PNG para las barreras.
- Fuente principal: Pixel Digivolve, cargada desde `assets/fonts/PIXEL LETTER`.
- Musica principal: `assets/music/principal.ogg`, reproducida en bucle durante la partida.
- Fondo principal: `assets/stage/ocean.png`, recortado proporcionalmente sin deformarse.
- Efectos de audio: `shotSound.ogg`, `gameOver.ogg` y `victory.ogg`.
- Agradecimientos: A mis perritos bombom, junior y peluchin, y a mi profe