/**
* @file security.h
* @brief Componente de control de seguridad
* Este archivo tiene toda la logica acerca del inicio de secion
* Tambien tiene las variables para su control como la contraseña
* @author Miguel Calambas
* @author Esteban Escandon
* @author Lina Diaz
*/

#ifndef SECURITY_H
#define SECURITY_H

#include "variables.h"

const char password [5] = {'0','1','2','3','4'}; /*!< contraseña que para acceder*/
int tries = 0; /*!< Intentos que se han hecho (minimo cero, maximo 3)*/
char intentoActual [6]; /*!< vector que me guarda lo que ha escrito hasta el momento*/
int cantidadCaracteres = 0; /*!< cantidad de caracteres escrita hasta el momento*/
int sistemaBloqueado = 0; /*!< Guarda si el sistema esta bloqueado*/


/**
* @brief Inicializa las variables para el correcto funcionamiento.
* Tambien inicia la tarea para el loop 
*/
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

/**
* @brief Hace las acciones necesarias para cuando la contraseña es correcta
*/
void runRightPass(){
  lcd.clear();
  lcd.print("Clave correcta");
  digitalWrite(ledG, HIGH);
  taskStopLoop.Start();
  taskCorrect.Start();
  taskTimeOut30k.Stop();
}

/**
* @brief Hace las acciones necesarias para un intento fallido, incluyendo validar si es necesario bloquear el sistema
*/
void runWrongPass(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Clave incorrecta");
  digitalWrite(ledB, HIGH);

  for(size_t i=0; i<5;i++)
    intentoActual[i] = '*';
  taskStopLoop.Start(); 
  if(tries == 2){
    taskSysBlock.Start();
  }else{
    taskAgain.Start();
    cantidadCaracteres = 0;
  }
}

/**
* @brief Funcion que se va a ejecutar cada vez para validar si se presiono un key y resibir la contraseña
*/
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
/**
* @brief Hace las acciones para volver a intentar ingresar la contraseña 
*/
void again(){
  digitalWrite(ledB, LOW);
  lcd.clear();
  lcd.print("Ingresa otra vez");      
  tries++;
  taskLoop.Start(); 
}

#endif