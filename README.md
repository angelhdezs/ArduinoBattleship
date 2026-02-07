# Batalla Naval Electrónica con Arduino

![Foto Principal del Proyecto Terminado](ruta/a/tu/foto_principal.jpg)
*(Pon aquí una foto donde se vea tu consola terminada y encendida)*

## Descripción del Proyecto

Este proyecto es una consola de videojuegos creada desde cero utilizando un Arduino Nano. Es una versión digital del clásico juego de mesa "Batalla Naval", pero mejorada con luces, sonidos y música estilo arcade.

El sistema permite que dos jugadores se enfrenten en tiempo real. Cada uno tiene su propio control (joystick) y su propia pantalla de luces (matriz LED) para ver dónde están sus barcos y dónde ha disparado el enemigo.

Todo el circuito está protegido por una carcasa personalizada que diseñé en 3D e imprimí específicamente para acomodar los componentes de forma ergonómica y segura.

Además, logré programar el audio para que la música de fondo suene continuamente sin interrumpir el juego.

## Cómo Funciona

El cerebro del juego es un **Arduino Nano**. Este controla:
* **Visualización:** Dos pantallas de 64 luces (8x8) que muestran los barcos (azul), los aciertos (verde) y los fallos (rojo).
* **Control:** Dos joysticks que permiten mover el cursor y un botón para disparar.
* **Audio:** Dos bocinas (buzzers); una toca música de fondo (Metal Slug) y la otra hace los efectos de sonido de las explosiones.

![Foto del Modelo 3D](ruta/a/tu/captura_modelo_3d.jpg)
*(Pon aquí una captura de pantalla de tu diseño en el programa 3D)*

## Materiales Utilizados

* 1 Arduino Nano.
* 2 Matrices LED 8x8 (modelo WS2812B).
* 2 Joysticks (modelo KY-023).
* 2 Buzzers pasivos (bocinas pequeñas).
* 1 Fuente de poder externa de 5V y 3A.
* **Carcasa:** Diseño propio impreso en 3D (PLA).

## Conexiones (Pinout)

Si quieres armar este proyecto, sigue esta tabla de conexiones. Es muy importante que uses una fuente de energía externa para las pantallas, ya que consumen mucha energía.

| Componente | Pin en Arduino | Función |
| :--- | :--- | :--- |
| **Joystick 1** | A0 y A1 | Movimiento |
| **Joystick 1** | D2 | Botón |
| **Joystick 2** | A2 y A3 | Movimiento |
| **Joystick 2** | D3 | Botón |
| **Pantalla LED 1** | D6 | Datos de imagen |
| **Pantalla LED 2** | D7 | Datos de imagen |
| **Buzzer Música** | D10 | Música de fondo |
| **Buzzer Efectos** | D12 | Sonidos del juego |

*Nota: Los controles están programados de forma invertida para que coincidan con la posición cómoda de los joysticks en mi diseño de carcasa.*

![Foto del Circuito Interno](ruta/a/tu/foto_circuito.jpg)
*(Pon aquí una foto de cómo se ven los cables y conexiones por dentro de la carcasa impresa)*

## Cómo Instalar el Código

1.  Descarga este repositorio.
2.  Abre el archivo en el programa **Arduino IDE**.
3.  Instala la librería llamada **Adafruit NeoPixel** (desde el gestor de librerías).
4.  Conecta tu Arduino a la computadora.
5.  **¡OJO!** Desconecta el cargador de pared antes de conectar el USB para no quemar nada.
6.  Sube el código a la placa.

## Instrucciones de Juego

1.  **Inicio:** Al prender la consola, escucharás la música de inicio.
2.  **Colocar Barcos:**
    * Usa el joystick para moverte.
    * Presiona el botón para poner un barco.
    * Cada jugador debe colocar **20 barcos**.
3.  **La Batalla:**
    * Por turnos, elige una casilla del rival y dispara.
    * Si ves una **Luz Verde** y escuchas una explosión: ¡Le diste!
    * Si ves una **Luz Roja**: Fallaste (agua).
4.  **Ganador:** El primero en hundir los 20 barcos del rival gana el juego y verá una secuencia de victoria.

![Foto Jugando](ruta/a/tu/foto_jugando.jpg)
*(Pon aquí una foto de la consola en plena partida con las luces encendidas)*

## Créditos

Desarrollado por: Angel Emiliano Hernández Solano

<!--# Proyecto Final

Autor: Selemi

## Hola Selemi

**Este fue un proyecto que hice en la school**

1. Me tarde mucho
2. Estuvo dificil

*Este es mi perrito Chems*:

 Este es un comentario y no se verá al renderizar. 
 ![Chems](/chems.png)

<img src="chems.png" alt="Chems" width="250" height="200"> -->
