#include "variables.h"
#include "StateMachineLib.h"
#include "stateMachine.h"

//Para validar que el boton se presione una vez
int oldValueButton;

void setup() {
  Serial.begin(9600); 
  lcd.begin(16,2); //Iniciamos el lcd para el correcto uso
  setupMachine();//Creamos la maquina de estados
  dht.begin(); //Iniciamos el sensor dht
  pinMode(buttonPin, INPUT);//Configuamos el boton
  input = unknow; // Iniciamos el input como  desconocido
}

void loop() {
  readInput();//Leemos si se ha presionado el boton
  stateMachine.Update(); //Se actualiza la maquina de estado
  input = unknow; // Se actualiza el input como know
  updateTask(); //Se ejecutan tareas asincronicas
}

//Funcion que valida si se ha presionado el boton
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

