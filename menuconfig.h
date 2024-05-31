
#ifndef MENUCONFIG_H
#define MENUCONFIG_H

#include "variables.h"
#include <LiquidMenu.h>
#define limitMaxTemp 50
#define limitMaxHall 1023
#define limitMaxLight 1023
#define limitMinTemp 0
#define limitMinLight 0



LiquidLine line1(0, 1, "TEMP HIGH [", maxTemp, "]");
LiquidLine line2(0, 1, "TEMP LOW [", minTemp, "]");
LiquidLine line3(0, 1, "LUZ HIGH [", maxLight, "]");
LiquidLine line4(0, 1, "LUZ LOW [", minLight, "]");
LiquidLine line5(0, 1, "HALL [", maxHall, "]");
LiquidLine line6(0, 1, "RESET");

LiquidScreen screen;
LiquidMenu menu(lcd);

// Used for attaching something to the lines, to make them focusable.
void blankFunction() {
    return;
}

//Sube los valores de las varibles
void upValue(int actualLine);
//Baja los valores de las vairables
void downValue(int actualLine);

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

void reset(){
  maxTemp = 40;
  minTemp = 0;
  maxLight= 300;
  minLight= 50;
  maxHall = 1000;
}

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