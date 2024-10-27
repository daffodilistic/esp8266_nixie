#include <Arduino.h>
#include <FastLED.h>
#include "pitches.h"

#define NUM_LEDS 2
#define LED_ARRAY_PIN 12
#define TOUCH_PIN 16
#define BUZZER_PIN 4

CRGB leds[NUM_LEDS];
int melody[] = {
  NOTE_A4, NOTE_F4, NOTE_G4, NOTE_C4, 0, NOTE_C4, NOTE_G4, NOTE_A4, NOTE_F4
};
int melodyLength = *(&melody + 1) - melody;
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  2, 2, 2, 2, 1, 2, 2, 2, 2
};
bool playMelody = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Hello, World!");

  FastLED.addLeds<NEOPIXEL, LED_ARRAY_PIN>(leds, NUM_LEDS);
  leds[0] = CRGB::White; FastLED.show();
}

void loop() {
  pinMode(TOUCH_PIN, INPUT);
  int value = digitalRead(TOUCH_PIN);
  Serial.println("value is" + String(value));
  if (value == 0) {
    playMelody = !playMelody;
  }

  if (playMelody) {
    leds[0] = CRGB::Red; FastLED.show();
    for (int thisNote = 0; thisNote < melodyLength; thisNote++) {
      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(BUZZER_PIN, melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // The note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(BUZZER_PIN);
    }
    playMelody = false;
  } else {
    leds[0] = CRGB::Green; FastLED.show();
  }

  delay(500);
}