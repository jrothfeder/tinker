#include <FastLED.h>

#define LED_PIN     3
#define NUM_LEDS    3
#define BRIGHTNESS  255
#define LED_TYPE    NEOPIXEL
CRGB leds[NUM_LEDS];

void setup() 
{
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
}

void loop() 
{
     unsigned int rgbColour[3];

     // Start off with red.
     rgbColour[0] = 255;
     rgbColour[1] = 0;
     rgbColour[2] = 0;

     // Choose the colours to increment and decrement.
     for (int decColour = 0; decColour < 3; decColour += 1) 
     {
            int incColour = decColour == 2 ? 0 : decColour + 1;

            // cross-fade the two colours.
           for(int i = 0; i < 255; i += 1) 
           {
                   rgbColour[decColour] -= 1;
                   rgbColour[incColour] += 1;

                   setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
                  FastLED.show();
                  FastLED.delay(100);
           }
     }
}

void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) 
{
        leds[0].r = red;
        leds[0].g = green;
        leds[0].b = blue;

        leds[1].r = blue;
        leds[1].g = red;
        leds[1].b = green;

        leds[2].r = green;
        leds[2].g = blue;
        leds[2].b = red;
}
