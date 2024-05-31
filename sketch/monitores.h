/**
 * @file monitores.h
 * @brief Archivo donde se definen las funciones para Imprimir en el Li
 * Aqui se encuentran la configuracion de los pines analogos y virtuales, junto con a las entradas analogas
 *
 * @author Miguel Calambas VivAS
 * @author Esteban Santiago Escandon Causaya
 * @author Lina Alexandra Diaz Ospona 
 * @version 1.0
 * @license MIT (u otra licencia si aplica)
 */

/**
 * @brief Función para mostrar el monitor ambiental.
 * Lee los valores de temperatura, humedad e intensidad lumínica para luego mostrarlos.
 * Si la temperatura, humedad o intensidad lumínica superan el umbral, se lanza una tarea.
*/
void printMonitorAmbiental() {
  lcd.clear();
  int h = dht.readHumidity();

  int t =dht.readTemperature();

  int photocell = analogRead(photocellPin);


  lcd.setCursor(0, 0);

  lcd.print("TEMP:");
  lcd.print(t);

  lcd.setCursor(8, 0);
  lcd.print("HUM:");
  lcd.print(h);
  
  lcd.setCursor(4,1);
  lcd.print("LUZ:");
  lcd.print(photocell);

  if(t > maxTemp){
    Serial.print("La temperatura supera los ");
    Serial.println(maxTemp);
    taskTempHigh.Start();
  }
  else if(photocell > maxLight){
    Serial.print("La lectura del photocell es mayor a ");
    Serial.println(maxLight);
    taskLightHigh.Start();
  }

}

/**
 * @brief Función para mostrar el monitor magnético.
 * Lee los valores del sensor hall para luego mostrarlos en pantalla.
 * Si el valor supera el umbral, se lanza una tarea.
*/
void printMonitorHall(){
  
  int mag = analogRead(hallPin);

  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("MAG:");
  lcd.print(mag);

  if(mag > maxHall)
    taskHallHigh.Start();
}