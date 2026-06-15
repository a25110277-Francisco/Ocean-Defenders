## **PROPUESTA DE VIDEOJUEGO: OCEAN DEFENDERS** 

_(Space Invaders Submarino)_ 

## **Descripción General** 

Juego basado en el clásico Space Invaders, programado en C++ con programación orientada a objetos utilizando la librería gráfica SFML 3. Los enemigos se organizan en filas en la parte superior de la pantalla, moviéndose lateralmente mientras descienden progresivamente hacia el jugador. El objetivo es eliminarlos antes de que alcancen el fondo o destruyan al jugador y su base. 

## **JUGADOR (Submarino)** 

## **Mecánicas de Movimiento y Ataque** 

- Se mueve únicamente en el eje horizontal (izquierda/derecha) 

- Dispara torpedos verticalmente hacia arriba 

- Puede disparar un torpedo cada 0.5 segundos 

- Velocidad de movimiento: normal (modificable con power-up) 

## **Sistema de Oxígeno** 

- **Oxígeno inicial:** 100 puntos 

- **Pérdida pasiva:** -2 puntos cada 5 segundos 

- **Daño por impacto enemigo:** 

   - Medusa: -10 oxígeno 

   - Calamar: -15 oxígeno 

   - Cangrejo: -20 oxígeno 

   - Pulpo Leviatán (jefe): -25 oxígeno 

- **Regeneración:** 

   - Burbujas normales: +15 oxígeno 

   - Power-up de oxígeno: +30 oxígeno 

## **Condición de Derrota del Jugador** 

El juego termina si: 

- El oxígeno llega a 0 

- Los enemigos llegan al fondo de la pantalla 

- El arrecife es destruido 

## **ARRECIFE (Base del Jugador)** 

- **Vida inicial:** 1300 puntos

- **Ubicación:** Parte inferior de la pantalla, detrás del jugador 

- **Daño recibido:** Los proyectiles enemigos que impactan el arrecife causan: 

   - Proyectil de medusa: -5 puntos 

   - Proyectil de calamar: -10 puntos 

   - Proyectil de cangrejo: -15 puntos 

- **Particularidad:** La vida del arrecife NO se regenera entre rondas 

- **Importancia:** Debe protegerse durante todo el juego 

## **ENEMIGOS** 

**Tipos y Características** 

|**Tipo**|**Vida**|**Daño al Jugador**|**Velocidad**|**Cadencia de Disparo**|
|---|---|---|---|---|
|**Medusa**(ligero)|10 HP|-10 oxígeno|Rápida|Cada 3 segundos|
|**Calamar**(medio)|20 HP|-15 oxígeno|Media|Cada 2.5 segundos|
|**Cangrejo**(tanque)|30 HP|-20 oxígeno|Lenta|Cada 2 segundos|



## **Comportamiento** 

- Se mueven en formación de lado a lado 

- Descienden una fila cada vez que tocan el borde de la pantalla 

- Disparan proyectiles hacia abajo aleatoriamente 

- Al ser destruidos, tienen 20% de probabilidad de soltar un power-up 

## **JEFE FINAL: PULPO LEVIATÁN** 

- **Vida:** 200 HP 

- **Daño por contacto/proyectil:** -25 oxígeno 

- 

   - **Patrón de ataque:** 

      - Fase 1 (200-100 HP): Dispara proyectiles en línea recta 

      - Fase 2 (100-0 HP): Dispara ráfagas de 3 proyectiles 

- 

   - **Movimiento:** Más libre que enemigos normales, con desplazamiento horizontal y vertical dentro de su zona

- 

- **Power-ups:** Lanza un power-up aleatorio cada 30 segundos 

## • **Aparición:** Ronda 4 (ronda final) 

## **POWER-UPS** 

Los power-ups caen desde donde murió el enemigo y el jugador debe recogerlos moviéndose debajo de ellos. 

|**Power-Up**|**Efecto**|**Duración**|
|---|---|---|
|**Doble Disparo**|Dispara 2 torpedos simultáneamente|10 segundos|
|**Velocidad Aumentada**|Movimiento 25% más rápido|5 segundos|
|**Suministro de Oxígeno**|Restaura 30 puntos de oxígeno|Instantáneo|



## **Burbujas de Oxígeno** 

- Aparecen cada 15 segundos en posiciones aleatorias 

- Se mueven lentamente hacia abajo 

- NO persiguen al jugador 

- Restauran +15 oxígeno al ser recolectadas 

- Desaparecen si llegan al fondo sin ser recogidas 

## **SISTEMA DE RONDAS** 

## **Ronda 1: Invasión Inicial** 

   - **Enemigos:** 15 (5 medusas, 5 calamares, 5 cangrejos)

   - **Barreras:** 4 barreras regeneradas completamente

   - **Velocidad:** Normal 

- **Oxígeno del jugador:** Se regenera completamente al terminar 

- **Ronda 2: Oleada Reforzada** 

   - **Enemigos:** 18 (6 medusas, 6 calamares, 6 cangrejos)

   - **Barreras:** 3 barreras regeneradas completamente

   - **Velocidad:** 25% más rápido que ronda 1 

   - **Oxígeno del jugador:** Se regenera completamente al terminar 

## **Ronda 3: Asalto Masivo** 

- **Enemigos:** 21 (7 medusas, 7 calamares, 7 cangrejos)

- **Barreras:** 2 barreras regeneradas completamente

- 

   - **Velocidad:** 50% más rápido que ronda 1 

- **Oxígeno del jugador:** Se regenera completamente al terminar 

## **Ronda 4: JEFE FINAL** 

- **Barreras:** 2 barreras regeneradas completamente

- **Enemigo:** Pulpo Leviatán 

- **Condición de victoria:** Derrotar al jefe 

- **Oxígeno del jugador:** Se regenera completamente al comenzar 

**Nota importante:** El arrecife mantiene el daño acumulado de todas las rondas anteriores. 

El arrecife utiliza las 6 etapas de destruccion incluidas en `reef.png`. La barra de vida se muestra unicamente en el HUD superior.

## **CONDICIONES DE VICTORIA Y DERROTA** 

## **Victoria** 

Completar las 4 rondas derrotando al Pulpo Leviatán. 

## **Derrota** 

- Oxígeno del jugador llega a 0 

- Enemigos llegan al fondo de la pantalla 

- El arrecife es destruido (vida = 0) 

Perfecto, agrego la sección de barreras con temática submarina coherente con tu juego: 

## **BARRERAS PROTECTORAS** 

## **Descripción** 

Formaciones de coral que sirven como escudos entre el jugador y los enemigos. Absorben los proyectiles enemigos. Los torpedos del jugador las atraviesan sin causar dano.

Las barreras utilizan las 6 etapas de destruccion incluidas en `wall.png` y conservan la proporcion original del sprite.

## **Características** 

|**Características**||
|---|---|
|**Atributo**|**Valor**|
|**Cantidad**|4 en ronda 1, 3 en ronda 2 y 2 en rondas 3 y 4|
|**Ubicación**|Distribuidas equitativamente en el espacio entre el jugador y los<br>enemigos|
|**Vida inicial**|40 puntos cada una|
|**Daño por**<br>**impacto**|-5 puntos por proyectil enemigo|



## **Mecánica de Degradación** 

- Cada impacto reduce la vida de la barrera en 5 puntos 

- A medida que pierden vida, cambian de apariencia visualmente: 

   - **40-30 HP:** Coral intacto (color vibrante) 

   - **29-15 HP:** Coral agrietado (color opaco, grietas visibles) 

   - **14-1 HP:** Coral fracturado (fragmentos, casi destruido) 

   - **0 HP:** Barrera destruida (desaparece) 

## **Comportamiento** 

- Bloquean solamente proyectiles enemigos

- Se regeneran completamente al comenzar cada ronda

- Proporcionan protección estratégica al jugador 

- Los torpedos del jugador atraviesan las barreras sin destruirlas

## **Consideración Estratégica** 

Las barreras obligan al jugador a: 

- Calcular trayectorias de disparo 

- Decidir cuándo sacrificar protección por ofensiva 

- Posicionarse estratégicamente para maximizar su cobertura 

**Actualización a Condiciones de Derrota:** (sin cambios, las barreras NO son obligatorias para ganar)
