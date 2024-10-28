#include <Arduino.h>
#include <SPI.h>
#include <FastLED.h>
#include <TFT_eSPI.h>

#include "pitches.h"

TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup.h

#define TFT_GREY 0x5AEB // New colour
#define NUM_LEDS 3
#define LED_ARRAY_PIN 12
#define TOUCH_PIN 16
#define BUZZER_PIN 4

CRGB leds[NUM_LEDS];
int melody[] = {
    // NOTE_A4, NOTE_F4, NOTE_G4, NOTE_C4, 0, NOTE_C4, NOTE_G4, NOTE_A4, NOTE_F4
    NOTE_A2, NOTE_A3, NOTE_A4};
int melodyLength = *(&melody + 1) - melody;
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
    // 2, 2, 2, 2, 1, 2, 2, 2, 2
    4, 4, 4};
bool playMelody = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello, World!");

  FastLED.addLeds<NEOPIXEL, LED_ARRAY_PIN>(leds, NUM_LEDS);
  leds[0] = CRGB::White;
  FastLED.show();

  tft.init();
  tft.setRotation(0);
}

void loop()
{
  pinMode(TOUCH_PIN, INPUT);
  int value = digitalRead(TOUCH_PIN);
  Serial.println("value is " + String(value));
  if (value == 0)
  {
    playMelody = !playMelody;
  }

  if (playMelody)
  {
    leds[0] = CRGB::Red;
    FastLED.show();
    for (int thisNote = 0; thisNote < melodyLength; thisNote++)
    {
      // to calculate the note duration, take one second divided by the note type.
      // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
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
  }
  else
  {
    leds[0] = CRGB::Green;
    FastLED.show();
  }

  tft.fillScreen(TFT_GREY);
  tft.setCursor(0, 0, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(TFT_YELLOW);
  tft.setTextFont(2);
  tft.println(1234.56);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextFont(4);
  tft.println((long)3735928559, HEX); // Should print DEADBEEF
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextFont(2);
  tft.println("Groop");
  float fnumber = 123.45;
  tft.setTextColor(TFT_BLUE);
  tft.setTextFont(2);
  tft.print("Float = ");
  tft.println(fnumber);
  tft.print("Binary = ");
  tft.println((int)fnumber, BIN);
  tft.print("Hexadecimal = ");
  tft.println((int)fnumber, HEX);

  delay(1);
}