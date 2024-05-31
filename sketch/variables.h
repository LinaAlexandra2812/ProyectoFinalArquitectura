/**
 * @file variables.h
 * @brief Archivo para declarar variables generales del proyecto 
 * Aqui se encuentran la configuracion de los pines analogos y virtuales, junto con a las entradas analogas
 *
 * @author Miguel Calambas VivAS
 * @author Esteban Santiago Escandon Causaya
 * @author Lina Alexandra Diaz Ospona 
 * @version 1.0
 * @license MIT (u otra licencia si aplica)
 */

#ifndef VARIABLES_H
#define VARIABLES_H

#include <Keypad.h>
#include <LiquidMenu.h>
#include "DHT.h"
#include "AsyncTaskLib.h"
#include "StateMachineLib.h"
#include "Buzzer.h"

#define ledR 9 /*!< Numero de Pin de entrada led Rojo*/
#define ledG 8 /*!< Numero de Pin de entrada led Verde*/
#define ledB 7 /*!< Numero de Pin de entrada led Azul */
#define rs 12 /*!<Numero de Pin de entrada Rs  */
#define en 11  /*!< Numero de Pin de entrada en */
#define d4 5   /*!< Numero de Pin de entrada d4   */
#define d5 4   /*!< Numero de Pin de entrada d5*/
#define d6 3   /*!< Numero de Pin de entrada d6 */
#define d7 2   /*!< Numero de Pin de entrada d7 */

//Configuracion pines sensores
#define photocellPin A1 /*!< Entrada Analoga del photosensor */
#define hallPin A0 /*!< Entrada Analoga del sensor Hall*/
#define dhtPin 10 /*!< Entrada Virutal de dhtPin*/
#define buttonPin 6 /*!< Entrada Virtual para el pin del boton */

//Configuracion sensores
#define DHTTYPE DHT22 /*!< Tipo de sensor DHT (cambiar según corresponda).*/

DHT dht(dhtPin, DHTTYPE); /*!< Objeto de tipo dht donde se confgura (puerto digital, tipo de sensor dht)*/

//Configuracion liquidcrystal
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); /*!< Objeto lcd que guarda la configuracion de los pines del lcd 16x2*/

//COnfiguracion maquina de estados|
StateMachine stateMachine(6, 13); /*!< Objeto de la maquina de estado con 6 estados y 13 transiciones*/

//Configuracion variables keypad

#define ROWS 4 /*!< Cantidad de filas para la conexion con el keypad*/

#define COLS 4 /*!< Cantidad de columnas para la conexion con el keypad*/

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
}; /*!< Configuracion de los caracteres de entrada del keypad*/

byte rowPins[ROWS] = {22, 24, 26, 28}; /*!< Configuracion del los pines de entrada de las filas*/

byte colPins[COLS] = {30, 32, 34, 36}; /*!< Configuracion de los pines de entrada para las columnas*/

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); /*!< Objeto con configuracoin keypad*/

enum Input
{
  correctPassword = 0, /*!< Input Para la transicion correct Password*/
  button = 1,  /*!< Input Para la transicion Button*/
  timeOut7 = 2, /*!< Input Para la transicion de 7 segundos */
  timeOut10 = 3, /*!< Input Para la transicion 10 segundos */
  timeOut3 = 4, /*!< Input Para la transicion 3 segundos */
  timeOut4 = 5, /*!< Input Para la transicion 4 segundos */
  tempHigh = 6, /*!< Input Para la transicion de temperatura Alta */
  lightHigh = 7, /*!< Input Para la transicion de luz alta */
  hallHigh = 8,  /*!< Input Para la transicion de hall alta */
  sysBlock = 9,  /*!< Input Para la transicion de sistema bloqueado*/
  unknow = 10  /*!< Transicion  */
}; /*!< Posibles transcines de la maquina de estados*/

enum State {
  start = 0, /**< Estado de inicio. */
  config = 1, /**< Estado de configuración. */
  monitorAmbiental = 2, /**< Estado de monitoreo ambiental. */
  monitorEvents = 3, /**< Estado de monitoreo de eventos. */
  alarm = 4, /**< Estado de alarma. */
  block =5 /**< Estado de bloqueo. */
}; /*!< Posibles estados de la maquina de estados*/

Input input; /*!< Señal para la transicion de la maquina de estados*/

int prendido = 0; /*!< Representa si el led esta prendido*/

int maxTemp = 40; /**< Temperatura máxima permitida en grados. */
int minTemp = 0; /**< Temperatura mínima permitida en grados. */
int maxLight= 300; /**< Intensidad lumínica máxima permitida. */
int minLight= 50; /**< Intensidad limínica mínima permitida. */
int maxHall = 1000; /**< Valor máximo del sensor Hall. */

/**
 * @brief Función de bucle principal (loopS).
 *
 * Esta función se ejecuta periódicamente como parte de una tarea asincrónica.
 * Cada ciclo es necesario para la ejecucion de seguridad.
 */
void loopS();

/**
 * @brief Función de manejo de eventos (again).
 *
 * Esta función se ejecuta después de un intervalo específico como parte de una tarea asincrónica.
 * Sirve para volver intentar ingresar una contraseña.
 */
void again();

/**
 *!<Tarea asincrónica para el bucle principal. Ejecuta la función loopS cada 1 milisegundo (true indica repetición infinita).
 */
AsyncTask taskLoop(1, true, loopS);

/**
 *!< Tarea asincrónica para el manejo de eventos. Ejecuta la función again después de un intervalo de 2000 milisegundos (false indica una sola ejecución).
 */
AsyncTask taskAgain(2000, false, again);

/**
 * !<Tarea asincrónica para detener el bucle principal. Ejecuta una función lambda que detiene la tarea taskLoop después de 1 milisegundo.
 */
AsyncTask taskStopLoop(1, false, [] () { taskLoop.Stop();} );


/**
 * !<Funciones y tareas asincrónicas para el monitoreo ambiental y del sensor Hall.
 */

/**
 * !<Imprime los datos del monitoreo ambiental. Esta función se ejecuta periódicamente como parte de una tarea asincrónica. Debe implementarse para mostrar los valores ambientales (temperatura, luz, etc.).
 */
void printMonitorAmbiental();


/**
 * !<Imprime los datos del sensor Hall. Esta función se ejecuta periódicamente como parte de una tarea asincrónica. Debe implementarse para mostrar los valores del sensor Hall (campo magnético, etc.).
 */
void printMonitorHall();

/**
 * !<Tarea asincrónica para el monitoreo ambiental.
  Ejecuta la función printMonitorAmbiental cada 300 milisegundos (true indica repetición infinita).
 */
AsyncTask taskLoopMonitorAmbiental(300, true, printMonitorAmbiental);

/**
 *!<Tarea asincrónica para el sensor Hall.Ejecuta la función printMonitorHall cada 300 milisegundos (true indica repetición infinita).
 */
AsyncTask taskLoopMonitorHall(300, true, printMonitorHall);

/**
 * @brief Tareas de LiquidMenu.
 */

/**
 * @brief Función de bucle principal para LiquidMenu.
 *
 * Esta función se ejecuta periódicamente como parte de una tarea asincrónica.
 * Debe implementarse para manejar las operaciones relacionadas con LiquidMenu.
 */
void loopLiquidMenu();

/**
 * !< Tarea asincrónica para el bucle principal de LiquidMenu. Ejecuta la función loopLiquidMenu cada 100 milisegundos (true indica repetición infinita).
 */
AsyncTask taskLoopMenu(100, true, loopLiquidMenu); 

/**
 * @brief Tareas de Buzzer
*/

/**
 * !<Tarea asincrónica para reproducir la siguiente nota en la alarma Ejecuta la función playNextNote cada 1 milisegundo (true indica repetición infinita).
 */
AsyncTask taskMelodyAlarm(1, true, playNextNote);

/**
 * !<Tarea asincrónica para reproducir la siguiente nota en el bloqueo Ejecuta la función playNextNote cada 1 milisegundo (true indica repetición infinita).
 */
AsyncTask taskMelodyBlock(1, true, playNextNote);

/**
 * @brief Tareas de leds
*/

/**
 * !<Tarea asincrónica para el LED rojo. Enciende o apaga el LED rojo periódicamente cada 500 milisegundos (true indica repetición infinita).
 */
AsyncTask taskOnLedR(500, true, [](){
  if(prendido== 0)
    digitalWrite(ledR,HIGH);
   else
    digitalWrite(ledR,LOW);

  prendido = prendido == 0 ? 1:0;
  
});

/**
 * !< Tarea asincrónica para el LED azul. Enciende o apaga el LED rojo periódicamente cada 800 milisegundos (true indica repetición infinita).
 */
AsyncTask taskOnLedB(800, true, [](){
  if(prendido== 0)
    digitalWrite(ledB,HIGH);
   else
    digitalWrite(ledB,LOW);
  prendido = prendido == 0 ? 1:0;
});

/**
 * @brief Ejecuta acciones cuando se ingresa una contraseña incorrecta.
 *
 * Esta función se llama cuando se detecta una contraseña incorrecta en el sistema.
 * Funcion para ejecutar que hay una contraseña incorrecta.
 */
void runWrongPass();

/**
 * !<Tarea para manejar la transición a la entrada correcta de la contraseña. 
 Esta tarea se ejecuta una vez después de 2000 ms y actualiza el estado de entrada a Input::correctPassword.
 */
AsyncTask taskCorrect(2000, false, [](){ input = Input::correctPassword;});

/**
 *!<Tarea para manejar el bloqueo del sistema.
   Esta tarea se ejecuta una vez después de 10 ms y actualiza el estado de entrada a Input::sysBlock.
 */
AsyncTask taskSysBlock(10, false, [] () {input = Input::sysBlock;  });
/**
 *!<Tarea para manejar el tiempo de espera de 7 segundos.
  Esta tarea se ejecuta una vez después de 7000 ms y actualiza el estado de entrada a Input::timeOut7.
 */
AsyncTask taskTimeOut7(7000, false, [](){input = Input::timeOut7;});

/**
 * !< Tarea para manejar el tiempo de espera de 3 segundos.
  Esta tarea se ejecuta una vez después de 3000 ms y actualiza el estado de entrada
 a Input::timeOut3.
 */
AsyncTask taskTimeOut3(3000, false, [](){input = Input::timeOut3;});

/**
 *!<Tarea para manejar el tiempo de espera de 4 segundos.
  Esta tarea se ejecuta una vez después de 4000 ms y actualiza el estado de entrada
  a Input::timeOut4.
 */
AsyncTask taskTimeOut4(4000, false, [](){input = Input::timeOut4;});

/**
 * !<Tarea para manejar el tiempo de espera de 5 segundos por contraseñas incorrectas.
  Esta tarea se ejecuta una vez después de 5000 ms y llama a la función runWrongPass.
 */
AsyncTask taskTimeOut30k(5000, false, [](){runWrongPass();});

/**
 * !<Tarea para manejar el tiempo de espera de 10 segundos.
Esta tarea se ejecuta una vez después de 10000 ms y actualiza el estado de entrada
 a Input::timeOut10.
 */
AsyncTask taskTimeOut10(10000, false, []() { input = Input::timeOut10; });


/**
 * !< Tarea para manejar la pulsación de un botón.
  Esta tarea se ejecuta una vez después de 10 ms y actualiza el estado de entrada
  a Input::button.
 */

AsyncTask taskButton(10, false, [] () { input = Input::button;});

/**
 *!<Tarea para manejar una temperatura alta.
  Esta tarea se ejecuta una vez después de 5 ms y actualiza el estado de entrada
  a Input::tempHigh.
 */
AsyncTask taskTempHigh(5, false, [](){ input = Input::tempHigh;});

/**
 * !<Tarea para manejar una alta luminosidad.
   Esta tarea se ejecuta una vez después de 5 ms y actualiza el estado de entrada
   a Input::lightHigh.
 */
AsyncTask taskLightHigh(5, false, [](){ input = Input::lightHigh;});

/**
 * !<Tarea para manejar una alta detección en el sensor de hall.
  Esta tarea se ejecuta una vez después de 5 ms y actualiza el estado de entrada
  a Input::hallHigh.
 */
AsyncTask taskHallHigh(5, false, [](){ input = Input::hallHigh;});


/**
* @brief 
* Funcion para Actualizar todas las tareas asincronicas
*/

void updateTask(){
  taskTimeOut10.Update();
  taskLoop.Update();
  taskAgain.Update();
  taskCorrect.Update();
  taskStopLoop.Update();
  taskSysBlock.Update();
  taskLoopMonitorAmbiental.Update();
  taskTimeOut7.Update();
  taskTimeOut3.Update();
  taskButton.Update();
  taskLoopMonitorHall.Update();
  taskTimeOut4.Update();
  taskButton.Update();
  taskTempHigh.Update();
  taskLightHigh.Update();
  taskHallHigh.Update();
  taskLoopMenu.Update();
  taskMelodyAlarm.Update();
  taskOnLedR.Update();
  taskMelodyBlock.Update();
  taskTimeOut30k.Update();
  taskOnLedB.Update();
}

#endif