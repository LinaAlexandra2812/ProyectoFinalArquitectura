
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "security.h"
#include "monitores.h"
#include "menuconfig.h"
#include "Buzzer.h"

void runSysBlock();
void runConfig();
void runMonitorAmbiental();
void runMonitorEvents();
void runAlarm();

void leavingMonitorAmbiental();
void leavingMonitorEvents();
void leavingConfig();
void leavingAlarm();
void leavingBlock();
void leavingStart();
void setupMachine(){
  stateMachine.AddTransition(start, config, []() {return input ==correctPassword;});
  stateMachine.AddTransition(start, block, []() {return input == sysBlock;});
  
  stateMachine.AddTransition(config, monitorAmbiental, []() {return input == button;});
  
  stateMachine.AddTransition(monitorAmbiental, alarm, []() {return input == tempHigh;});
  stateMachine.AddTransition(monitorAmbiental, alarm, []() {return input == lightHigh;});
  stateMachine.AddTransition(monitorAmbiental, monitorEvents, []() {return input == timeOut7;});
  stateMachine.AddTransition(monitorAmbiental, config, []() {return input == button;});

  stateMachine.AddTransition(monitorEvents, alarm, []() {return input == hallHigh;});
  stateMachine.AddTransition(monitorEvents, config, []() {return input == button;});
  stateMachine.AddTransition(monitorEvents, monitorAmbiental, []() {return input == timeOut3;});

  stateMachine.AddTransition(alarm, start, []() {return input == button;});
  stateMachine.AddTransition(alarm, monitorAmbiental, []() {return input == timeOut4;});

  stateMachine.AddTransition(block, start, []() {return input == timeOut10;});

  stateMachine.SetOnEntering(start, seguridad);
  stateMachine.SetOnEntering(block,runSysBlock );
  stateMachine.SetOnEntering(config, startLiquidMenu);
  stateMachine.SetOnEntering(monitorAmbiental, runMonitorAmbiental);
  stateMachine.SetOnEntering(monitorEvents, runMonitorEvents);
  stateMachine.SetOnEntering(alarm, runAlarm);

  stateMachine.SetOnLeaving(start, leavingStart);
  stateMachine.SetOnLeaving(monitorAmbiental, leavingMonitorAmbiental);
  stateMachine.SetOnLeaving(monitorEvents, leavingMonitorEvents);
  stateMachine.SetOnLeaving(config, leavingConfig);
  stateMachine.SetOnLeaving(alarm, leavingAlarm);
  stateMachine.SetOnLeaving(block, leavingBlock );
  //Se configura estado inicial
  stateMachine.SetState(start, false, true);
} 

void startMelody();
void runSysBlock(){
  taskLoop.Stop();
  lcd.setCursor(0,0);
  lcd.print("Sistema bloqueado");
  digitalWrite(ledR, HIGH);
  lcd.noCursor();
  taskTimeOut10.Start();
  taskMelodyBlock.Start();
  taskOnLedR.Start();
  startMelody(buzzerBlock, sizeof(buzzerBlock) / sizeof(buzzerBlock[0]) / 2);
}

void runConfig(){
  lcd.clear();
  lcd.print("> Config");
}

void runMonitorAmbiental(){
  lcd.clear();
  taskLoopMonitorAmbiental.Start();
  taskTimeOut7.Start();
}

void runMonitorEvents(){
  lcd.clear();
  taskLoopMonitorHall.Start();
  taskTimeOut3.Start();
}
void runAlarm(){
  lcd.clear();
  lcd.print("> Alarm");
  taskTimeOut4.Start();
  startMelody(buzzerAlarm, sizeof(buzzerAlarm) / sizeof(buzzerAlarm[0]) / 2);
  taskMelodyAlarm.Start();
  taskOnLedB.Start();
}

void leavingStart(){
  digitalWrite(ledR,LOW);
  digitalWrite(ledB, LOW);
  digitalWrite(ledG, LOW);
}

void leavingMonitorAmbiental(){
  taskLoopMonitorAmbiental.Stop();
  Serial.println("Saliendo del estado Monitor Ambiental");
}
void leavingMonitorEvents(){
  taskLoopMonitorHall.Stop();
}
void leavingConfig(){
  taskLoopMenu.Stop();
}
void leavingAlarm(){
  taskMelodyAlarm.Stop();
  taskOnLedB.Stop();
}
void  leavingBlock (){
  taskMelodyBlock.Stop();
  taskOnLedR.Stop();  
}

#endif