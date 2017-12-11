#include "FastLED.h"
#include "Audio.h"

#include "AudioSampleLight1.h"
#include "AudioSampleLight2.h"
#include "AudioSampleLight3.h"
#include "AudioSampleLight4.h"
#include "AudioSampleLight5.h"
#include "AudioSampleLight6.h"

#define DATA_PIN (6)
#define NUM_LEDS (50)
#define MESSAGE_COUNT (5)
#define AUDIO_SAMPLE_COUNT (6)

String messages[] = {"he is comming", "merry christmas", "have you been naughty", "he is checking it twice", "deck the halls"};
byte message_index = 0;
byte character_index = 0;

byte led_mapping[26] = {49, 47, 46, 44, 42, 40, 38, 36, 19, 20, 21, 24, 26, 28, 30, 31, 33, 16, 14, 13, 11, 9, 7, 4, 2, 0};
CRGB led_data[NUM_LEDS];

const unsigned int *audiosamples[AUDIO_SAMPLE_COUNT] = {AudioSampleLight1,AudioSampleLight2,AudioSampleLight3,AudioSampleLight4,AudioSampleLight5,AudioSampleLight6};
const unsigned int audiotimes[AUDIO_SAMPLE_COUNT] = {25, 50, 70, 130, 80, 150};

AudioPlayMemory    sound0;
AudioOutputAnalog  dac;
AudioConnection patchCord1(sound0, 0, dac, 0);

void setup() {
  randomSeed(analogRead(0));
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(led_data, NUM_LEDS);
  AudioMemory(10);
}

void loop() {
  // Get Character from current message
  byte hue = random(255);
  char character = messages[message_index][character_index];
  displayCharacter(character, hue);

  // Delay Between Characters in message
  delay(random(200, 500));

  character_index++;
  if (character_index >= messages[0].length()) {
    character_index = 0;
    message_index = random(MESSAGE_COUNT-1);
    delay(random(2000, 3550));
  }
}

void displayCharacter(char character, byte hue) {
  if (character >= 'a' && character <= 'z') {
    byte led_index = led_mapping[character - 'a'];
    byte audio_index = random(AUDIO_SAMPLE_COUNT-1);
    int increment = 255 / audiotimes[audio_index];
    int value = 0;
    
    sound0.play(audiosamples[audio_index]);
    for(unsigned int i = 0; i < audiotimes[audio_index]; i++) {
      value += increment;
      if (value > 255) {
        value = 255;
      }
      led_data[led_index] = CHSV(hue, 255, value);
      FastLED.show();
    }
    for(int i = 0; i < 105; i++) {
      led_data[led_index] = CHSV(hue, 255, 255-i);
      FastLED.show();
      delayMicroseconds(200);
    }
    delay(random(200,400));
    byte fade_speed = random(5, 20);
    for(int i = 15; i >= 0; i--) {
      led_data[led_index] = CHSV(hue, 255, 10*i);
      FastLED.show();
      delay(fade_speed);
    }    
    FastLED.show();
  }
}

