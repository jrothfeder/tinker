#include "FastLED.h"

FASTLED_USING_NAMESPACE

#define DATA_PIN    4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    8
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

void setup() {
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

typedef void (*SimplePatternList[])();

// Use this array to add additional patterns
SimplePatternList gPatterns = { rainbow };

uint8_t gCurrentPatternNumber = 0;
uint8_t gHue = 0;
  
void loop()
{
  gPatterns[gCurrentPatternNumber]();
  FastLED.show();  
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
  EVERY_N_SECONDS( 10 ) { nextPattern(); }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // Built in to fastLED
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}
