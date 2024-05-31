/**
* @file Buzzer.h
* @brief Se guarda toda logica y informacion necesaria para el sonido del buzzer
* @author Miguel Calambas
* @author Esteban Escandon
* @author Lina Diaz
*/
#ifndef BUZZER_H
#define BUZZER_H

#define NOTE_B5  988 /*!<  valor de la nota B5*/
#define NOTE_F4  349  /*!<  valor de la nota F4*/
#define NOTE_G5  784  /*!<  valor de la nota G5*/
#define NOTE_C4  262  /*!<  valor de la nota C4*/

#include "variables.h"
#include "EasyBuzzer.h"
#define buzzer 13  /*!< Pin de salida digital para el buzzer*/

int tempo = 60;  /*!<  temp para el buzzer*/
int currentNoteIndex = 0;  /*!<  index de la nota actual*/
int* currentMelody;  /*!<  melodia que se ha configurado*/
int currentMelodyLength;  /*!<  Lonigutd de la melodia*/
unsigned long noteStartTime = 0;  /*!<  valor del tiempo de la nota start*/
bool isPlaying = false;  /*!<  boleano que representa si esta sonando*/

int buzzerBlock[] = {
  NOTE_B5, 8, NOTE_F4, 8
};  /*!<  melodia para el estado block*/

int buzzerAlarm[] = {
  NOTE_G5, 8, NOTE_C4, 8, NOTE_G5, 8, NOTE_C4, 8
};  /*!<  molodia para el alarm*/

/**
* @brief calcula la duracion de la nota que se va a tocar
*/
int calculateNoteDuration(int note, int tempo) {
  int wholenote = (60000 * 4) / tempo;
  int divider = note;
  if (divider > 0) {
    return (wholenote) / divider;
  } else if (divider < 0) {
    return (wholenote) / abs(divider) * 1.5; // notas con puntillo
  }
  return 0;
}

/**
* @brief Hace sonar la siguiente nota y valida casos importantes
*/
void playNextNote() {
  if (currentNoteIndex < currentMelodyLength * 2) {
    if (!isPlaying) {
      int note = currentMelody[currentNoteIndex];
      int duration = calculateNoteDuration(currentMelody[currentNoteIndex + 1], tempo);
      tone(buzzer, note, duration * 0.9);
      noteStartTime = millis();
      isPlaying = true;
    } else {
      unsigned long currentTime = millis();
      int duration = calculateNoteDuration(currentMelody[currentNoteIndex + 1], tempo);
      if (currentTime - noteStartTime >= duration) {
        noTone(buzzer);
        currentNoteIndex += 2;
        isPlaying = false;
      }
    }
  } else {
    // Resetea el índice cuando se acaba la melodía
    currentNoteIndex = 0;
  }
}

/**
* @brief Configura la meolodia para cuando se ejecute el playNextNote
* @param melody parametro que resibe la meolodia que se va a configurar
* @param length parametro que resibe la longitud de la meolodia que se va a configurar
*/
void startMelody(int melody[], int length) {
  currentMelody = melody;
  currentMelodyLength = length;
  currentNoteIndex = 0;
  noteStartTime = millis();
  isPlaying = false;
}

#endif

