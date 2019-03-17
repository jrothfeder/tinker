#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <WidgetRTC.h>

#include <FastLED.h>

FASTLED_USING_NAMESPACE

char auth[] = "6ef5a5f7ce8e454384f604327776e9d9";
char ssid[] = "[wifi]";
char pass[] = "[pass]";

// LED Settings
#define DATA_PIN    13
#define LED_TYPE    NEOPIXEL
#define NUM_LEDS    3

CRGB leds[NUM_LEDS];

BlynkTimer timer;
WidgetRTC rtc;

volatile int wakeHour;
volatile int wakeMinute;

volatile int sleepHour;
volatile int sleepMinute;

volatile int brightness;

volatile int r=255,g=0,b=0;

volatile bool isSleep = false;

BLYNK_CONNECTED() {
  rtc.begin();
  Blynk.syncAll();
}

BLYNK_WRITE(V1) {
  TimeInputParam t(param);
  wakeHour = t.getStartHour();
  wakeMinute = t.getStartMinute();
}

BLYNK_WRITE(V2) {
  int brightness = param.asInt(); 
  FastLED.setBrightness(brightness);
}

BLYNK_WRITE(V3) {
  isSleep = param.asInt();
}

BLYNK_WRITE(V4) {
  TimeInputParam t(param);
  sleepHour = t.getStartHour();
  sleepMinute = t.getStartMinute();
}

BLYNK_WRITE(V5) {
  r = param[0].asInt();
  g = param[1].asInt();
  b = param[2].asInt();
}

void setup() {
  Serial.begin(9600);
  
  Blynk.begin(auth, ssid, pass);
  setSyncInterval(10 * 60);

  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}

void loop() {
  Blynk.run();
  timer.run();

  EVERY_N_SECONDS(10) {
    writeTimeToSerial();
  }
  
  maintain();
    
  if(isSleep) {
    setLeds();
  } else {
    fadeToBlackBy(leds, NUM_LEDS, 25);
  }
  
  FastLED.show();  
}

void setLeds() {
  fill_solid(leds, NUM_LEDS, CRGB(g,r,b));
}

void writeTimeToSerial() {
  int hours = hour();
  int minutes = minute();

  Serial.print("hour: ");
  Serial.println(hours);

  Serial.print("minute: ");
  Serial.println(minutes);

  if (isSleep) {
    Serial.println("state is: sleep");
  } else {
    Serial.println("state is: wake");
  }
}

void maintain() {
  int hours = hour();
  int minutes = minute();

  // Assume sleep time is always later in the day than wake time
  if (hours == sleepHour && minutes == sleepMinute) {
    isSleep = true;
    Blynk.virtualWrite(V3, isSleep);
  } 

  if (hours == wakeHour && minutes == wakeMinute) {
    isSleep = false;
    Blynk.virtualWrite(V3, isSleep);
  }
}
