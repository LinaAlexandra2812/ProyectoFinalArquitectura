
#ifndef VARIABLES_H
#define VARIABLES_H

#include <Keypad.h>
//#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include "DHT.h"
#include "AsyncTaskLib.h"
#include "StateMachineLib.h"
#include "Buzzer.h"
//#include "menuconfig.h"

//configuracion salidas
#define ledR 9
#define ledG 8
#define ledB 7
#define rs 12
#define en 11
#define d4 5
#define d5 4
#define d6 3
#define d7 2


//Configuracion pines sensores
#define photocellPin A1
#define hallPin A0
#define dhtPin 10
#define buttonPin 6

//Configuracion sensores
#define DHTTYPE DHT22 // << CAMBIAR
DHT dht(dhtPin, DHTTYPE);

//Configuracion liquidcrystal
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//COnfiguracion maquina de estados
StateMachine stateMachine(6, 13);

//Configuracion variables keypad
#define ROWS 4
#define COLS 4

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


byte rowPins[ROWS] = {22, 24, 26, 28}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {30, 32, 34, 36}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//Configuracion transiciones y estados stateMachine
enum Input
{
  correctPassword = 0,
  button = 1,
  timeOut7 = 2,
  timeOut10 = 3,
  timeOut3 = 4,
  timeOut4 = 5,
  tempHigh = 6,
  lightHigh = 7,
  hallHigh = 8,
  sysBlock = 9,
  unknow = 10
};

enum State {
  start=0,
  config=1,
  monitorAmbiental=2,
  monitorEvents=3,
  alarm=4,
  block=5
};

//Variables de acceso global 
Input input;
int prendido= 0;

//Variables limites de lectura
int maxTemp = 40;
int minTemp = 0;
int maxLight= 300;
int minLight= 50;
int maxHall = 1000;

//Tareas Asincronicassecurity
void loopS();
void again();
AsyncTask taskLoop(1, true, loopS);
AsyncTask taskAgain(2000, false, again);


AsyncTask taskStopLoop(1, false, [] () { taskLoop.Stop();} );

//Tareas asincronicas monitores
void printMonitorAmbiental();
void printMonitorHall();
AsyncTask taskLoopMonitorAmbiental(300, true, printMonitorAmbiental);
AsyncTask taskLoopMonitorHall(300, true, printMonitorHall);

//Tareas de liquidMenu
void loopLiquidMenu();
AsyncTask taskLoopMenu(100, true, loopLiquidMenu); 
//Tareas buzzer
AsyncTask taskMelodyAlarm(1, true, playNextNote);
AsyncTask taskMelodyBlock(1, true, playNextNote);
//Tareas leds
AsyncTask taskOnLedR(500, true, [](){
  if(prendido== 0)
    digitalWrite(ledR,HIGH);
   else
    digitalWrite(ledR,LOW);

  prendido = prendido == 0 ? 1:0;
  
});
AsyncTask taskOnLedB(800, true, [](){
  if(prendido== 0)
    digitalWrite(ledB,HIGH);
   else
    digitalWrite(ledB,LOW);
  prendido = prendido == 0 ? 1:0;
});

void runWrongPass();
//Configuracion Tareas asincronicas  de transicion y su respectiva actualizacion
AsyncTask taskCorrect(2000, false, [](){ input = Input::correctPassword;});
AsyncTask taskSysBlock(10, false, [] () {input = Input::sysBlock;  });
AsyncTask taskTimeOut7(7000, false, [](){input = Input::timeOut7;});
AsyncTask taskTimeOut3(3000, false, [](){input = Input::timeOut3;});
AsyncTask taskTimeOut4(4000, false, [](){input = Input::timeOut4;});
AsyncTask taskTimeOut30k(5000, false, [](){runWrongPass();});
AsyncTask taskTimeOut10(10000, false, []() { input = Input::timeOut10; });//>> Pendiente del tiempo que etsa pasando
AsyncTask taskButton(10, false, [] () { input = Input::button;});
AsyncTask taskTempHigh(5, false, [](){ input = Input::tempHigh;});
AsyncTask taskLightHigh(5, false, [](){ input = Input::lightHigh;});
AsyncTask taskHallHigh(5, false, [](){ input = Input::hallHigh;});


//Tareas leds preguntar como deberian de comportarse

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