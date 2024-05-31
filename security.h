#ifndef SECURITY_H
#define SECURITY_H

#include "variables.h"

//Varible de seguridad
const char password [5] = {'0','1','2','3','4'};
int tries = 0;
char intentoActual [6];
int cantidadCaracteres = 0;
int sistemaBloqueado = 0;

//Tareas asincronicas

void seguridad(){
 
  Serial.println("Ha entrado a el estado Start");
  tries = 0;
  cantidadCaracteres = 0;
  sistemaBloqueado = 0;
  digitalWrite(ledR,LOW);
  digitalWrite(ledB, LOW);
  digitalWrite(ledG, LOW);
  lcd.clear();
  lcd.print("Ingrese clave");
  
  taskLoop.Start();
  for(size_t i=0; i<5;i++)
        intentoActual[i] = '*';
}
void runRightPass(){
  //Se configura led y lcd
  lcd.clear();
  lcd.print("Clave correcta");
  digitalWrite(ledG, HIGH);
  taskStopLoop.Start();
  taskCorrect.Start();
  taskTimeOut30k.Stop();
}

void runWrongPass(){
  //Se configura para contraseña incorrecta
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Clave incorrecta");
  digitalWrite(ledB, HIGH);

  for(size_t i=0; i<5;i++)
    intentoActual[i] = '*';
  taskStopLoop.Start(); 
  //Se valida si es encesario bloquear sistema
  if(tries == 2){
    taskSysBlock.Start();
  }else{
    taskAgain.Start();
    cantidadCaracteres = 0;
  }
}

void loopS(){
  lcd.setCursor(cantidadCaracteres, 1);
  char key = keypad.getKey();
  lcd.cursor();

  if(!key) return;

  taskTimeOut30k.Stop();
  taskTimeOut30k.Start();
  //Resibimos caracter ingresado
  if(cantidadCaracteres < 5)
    intentoActual[cantidadCaracteres] = key;
  lcd.print("*");

  if(strcmp(intentoActual, password) == 0){
    runRightPass();
    return;
  }else if(cantidadCaracteres+1 == 8)
    runWrongPass();
  else
    cantidadCaracteres++;
}
void again(){
  digitalWrite(ledB, LOW);
  lcd.clear();
  lcd.print("Ingresa otra vez");      
  tries++;
  taskLoop.Start(); 
}

#endif