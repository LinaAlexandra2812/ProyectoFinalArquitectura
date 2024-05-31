/**
 * @file sketch.ino
 * @brief Se configura lo necesario en el setup,
 * tambien se ejecutan las tareas necesarias en el loop
 * y se valida si se ha presionado el boton
 * @author Miguel Calambas
 * @author Esteban Santiago
 * @author Lina Diaz
 * @version 1.0
*/

#include "variables.h"
#include "StateMachineLib.h"
#include "stateMachine.h"

//Para validar que el boton se presione una vez
int oldValueButton; /*!< Valor anterior del boton al presionarlo*/

/**
 * @brief Se configura lo necesario en el setup
 * Comenzando por la inicializacion del lcd, la maquina de estados, el sensor dht y el boton
*/
void setup() {
  Serial.begin(9600); 
  lcd.begin(16,2); //Iniciamos el lcd para el correcto uso
  setupMachine();//Creamos la maquina de estados
  dht.begin(); //Iniciamos el sensor dht
  pinMode(buttonPin, INPUT);//Configuamos el boton
  input = unknow; // Iniciamos el input como  desconocido
}

/**
 * @brief Se ejecutan las tareas necesarias en el loop
 * Comenzando por la lectura del boton, la actualizacion de la maquina de estados, la actualizacion del input y la ejecucion de tareas asincronicas
*/
void loop() {
  readInput();//Leemos si se ha presionado el boton
  stateMachine.Update(); //Se actualiza la maquina de estado
  input = unknow; // Se actualiza el input como know
  updateTask(); //Se ejecutan tareas asincronicas
}

/**
 * @brief Se valida si se ha presionado el boton y se cambia en input 
*/
void readInput(){
  Input actualInput = (input != unknow) ? input: unknow;
  int valueButton = digitalRead(buttonPin);
  if(valueButton != oldValueButton){
    oldValueButton = valueButton;
    if(valueButton == HIGH){
      actualInput = button;
      Serial.println("Se ha presionado el boton");
    }
  }
  
  input = actualInput;
}

