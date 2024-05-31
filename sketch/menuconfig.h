/**
 * @file menuconfig.h
 * @brief Se configura todos los menus de la pantalla 
 * Aqui se encuentran las funciones, lineas y Menus de LiquidCrystal
 *
 * @author Miguel Calambas VivAS
 * @author Esteban Santiago Escandon Causaya
 * @author Lina Alexandra Diaz Ospona 
 * @version 1.0
 * @license MIT (u otra licencia si aplica)
 */
#ifndef MENUCONFIG_H
#define MENUCONFIG_H

#include "variables.h"
#include <LiquidMenu.h>
#define limitMaxTemp 50 /*!< Limite establecido por defecto como limite maximo de temperatura*/
#define limitMaxHall 1023 /*!< Limite establecido por defecto como limite maximo Hall*/
#define limitMaxLight 1023 /*!< Limite establecido por defecto como limite maximo de Luz*/
#define limitMinTemp 0 /*!< Limite establecido por defecto como limite minimo de temperatura*/
#define limitMinLight 0 /*!< Limite establecido por defecto como limite minimo de luz*/


LiquidLine line1(0, 1, "TEMP HIGH [", maxTemp, "]"); /*!< primera linea del menu*/
LiquidLine line2(0, 1, "TEMP LOW [", minTemp, "]"); /*!< segunda linea del menu*/
LiquidLine line3(0, 1, "LUZ HIGH [", maxLight, "]");/*!< tercera linea del menu*/
LiquidLine line4(0, 1, "LUZ LOW [", minLight, "]"); /*!< cuarta linea del menu*/
LiquidLine line5(0, 1, "HALL [", maxHall, "]");/*!< quinta linea del menu*/
LiquidLine line6(0, 1, "RESET"); /*!< sexta linea del menu*/

LiquidScreen screen; /*!< Pantalla del menu*/
LiquidMenu menu(lcd); /*!< Menu del lcd*/

/**
 * @brief Funcion que no hace nada, pero es necesaria
*/
void blankFunction() {
    return;
}

/** 
*@brief Sube los valores de las varibles

*/
void upValue(int actualLine);
/**
 * @brief Baja los valores de las variables
 * @param actualLine Linea actual en la que se encuentra el menu
*/
void downValue(int actualLine);

/**
 * @brief Inicia el menu de la pantalla
*/
void startLiquidMenu(){

  screen.add_line(line1);
  screen.add_line(line2);
  screen.add_line(line3);
  screen.add_line(line4);
  screen.add_line(line5);
  screen.add_line(line6);

  line1.attach_function(1, blankFunction);
  line2.attach_function(1, blankFunction);
  line3.attach_function(1, blankFunction);
  line4.attach_function(1, blankFunction);
  line5.attach_function(1, blankFunction);
  line6.attach_function(1, blankFunction);

  screen.set_displayLineCount(2);


  line1.set_decimalPlaces(2);

  menu.add_screen(screen);

  menu.update();
  menu.switch_focus();

  taskLoopMenu.Start();
}

/**
 * @brief Loop del menu de la pantalla para hacer los camvios respectivos
*/
void loopLiquidMenu(){
  char key = keypad.getKey();


  if(!key)
    return;

  int actualLine = menu.get_focusedLine();

  switch (key) {
  case 'A':
    upValue(actualLine);
    break;
  case 'B':
    downValue(actualLine);
    break;
  case 'D':
    menu.switch_focus();
    if(actualLine == 5)
      menu.switch_focus();
    break;
  default:
    Serial.println(actualLine);
  break;
  
  }
    menu.update();
  
}

/**
 * @brief Resetea los valores de las variables
*/
void reset(){
  maxTemp = 40;
  minTemp = 0;
  maxLight= 300;
  minLight= 50;
  maxHall = 1000;
}

/**
 * @brief Sube los valores de las varibles
 * @param actualLine Linea actual en la que se encuentra el menu
*/
void upValue(int actualLine){
  switch (actualLine) {
  case 0:
    if(maxTemp+2 <= limitMaxTemp)
      maxTemp+=2;
    break;
  case 1:
    if(minTemp+2 < maxTemp)
      minTemp+=2; 
    break;
  case 2:
    if(maxLight+20 <= limitMaxLight)
      maxLight+=20;
    break;
  case 3:
    if(minLight+20 < maxLight)
      minLight+=20;
    break;
  case 4:
    if(maxHall+50 <= limitMaxHall)
      maxHall+=50;
    break;
  case 5:
    reset();
    break;
  }

}
/**
 * @brief Baja los valores de las variables
 * @param actualLine Linea actual en la que se encuentra el menu
*/
void downValue(int actualLine){
  switch (actualLine) {
  case 0:
    if(maxTemp-2 > minTemp)
      maxTemp-=2;
    break;
  case 1:
    if(minTemp-2 >= limitMinTemp)
      minTemp-=2; 
    break;
  case 2:
    if(maxLight-20 > minLight)
      maxLight-=20;
    break;
  case 3:
    if(minLight-20 >= limitMinLight)
      minLight-=20;
    break;
  case 4:
    if(maxHall-50 >= 0)
      maxHall-=50;
    break;
  case 5:
    reset();
    break;
  }

}

#endif