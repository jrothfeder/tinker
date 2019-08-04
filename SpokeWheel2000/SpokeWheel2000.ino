#include <FastLED.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN    2
#define CLOCK_PIN   4
#define LED_TYPE    DOTSTAR
#define COLOR_ORDER RGB
#define NUM_LEDS    1

#define BUTTON_PIN  6
#define NUM_STATES  3

CRGB leds[NUM_LEDS];

Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

CRGBPalette16 palette  = RainbowColors_p;
TBlendType    blending = LINEARBLEND;

void setup() {
  if(!gyro.begin(GYRO_RANGE_2000DPS))
  {
    while(1);
  }
  
  delay(1000);
  
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  fill_solid(leds, 4, CRGB::Red);
  FastLED.show();

  pinMode(BUTTON_PIN,INPUT_PULLUP);
}

void updateState() {
  int state = buttonPushCounter % NUM_STATES;

  CRGB color = CRGB::Red;
  switch(state) {
    case 0 : color = CRGB::Red;
             break;
    case 1 : color = CRGB::White;
             break;
    case 2 : color = CRGB::Cyan;
             break;
  }

  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();
}

void colorForSpeed() {
  sensors_event_t event; 
  gyro.getEvent(&event);
  int rads = (int) abs(event.gyro.z);
  int adjustedRads = 255 - map(rads, 0, 20, 0, 255);

  for(int i=0; i<NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, adjustedRads, 255, blending);
  }

  FastLED.show();
}

void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  if(buttonState != lastButtonState) {
    if(buttonState == LOW) {
      buttonPushCounter++;
      updateState();
    }
    delay(50);
  }
  lastButtonState = buttonState;

  if((buttonPushCounter % NUM_STATES) == 2) {
    colorForSpeed();
  }
}
