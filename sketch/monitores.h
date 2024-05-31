
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
void printMonitorHall(){
  
  int mag = analogRead(hallPin);

  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("MAG:");
  lcd.print(mag);

  if(mag > maxHall)
    taskHallHigh.Start();
}