#ifndef BUZZER_H
#define BUZZER_H

//Notas necesarias
#define NOTE_B5  988
#define NOTE_F4  349
#define NOTE_G5  784
#define NOTE_C4  262

#include "variables.h"
#include "EasyBuzzer.h"
#define buzzer 13

int tempo = 60;
int currentNoteIndex = 0;
int* currentMelody;
int currentMelodyLength;
unsigned long noteStartTime = 0;
bool isPlaying = false;

// Definición de melodías en forma de arrays de notas y duraciones
int buzzerBlock[] = {
  NOTE_B5, 8, NOTE_F4, 8
};

int buzzerAlarm[] = {
  NOTE_G5, 8, NOTE_C4, 8, NOTE_G5, 8, NOTE_C4, 8
};

// Función que calcula la duración de una nota
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

// Función que se llama periódicamente para reproducir la siguiente nota
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

// Función para iniciar la reproducción de una melodía
void startMelody(int melody[], int length) {
  currentMelody = melody;
  currentMelodyLength = length;
  currentNoteIndex = 0;
  noteStartTime = millis();
  isPlaying = false;
}

#endif

