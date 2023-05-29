
#include <FastLED.h>

#define MATRIX_PIN 22

#define REFRESH 40 // 50
#define BRIGHTNESS 120 // 0 -> 255

#define NUM_LEDS 256

#define LED_ARRAY_SIZE 22000

int matrix_refreh = REFRESH;
int matrix_flag_wait = 0;
int matrix_delay_betweeen_cycles = 4000; // 5000
long matrix_previous_millis = 0;
int current_frame = 1;
CRGB leds[NUM_LEDS];

////////////////////////////////////////////////////////////////////////////////
//////////FUNCTIONS/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setup_matrix(){
  FastLED.addLeds<NEOPIXEL, MATRIX_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}