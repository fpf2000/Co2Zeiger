/*
  Reading CO2, humidity and temperature from the SCD30
  By: Nathan Seidle
  SparkFun Electronics
  Date: May 22nd, 2018
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.

  Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/15112

  This example prints the current CO2 level, relative humidity, and temperature in C.

  Hardware Connections:
  Attach RedBoard to computer using a USB cable.
  Connect SCD30 to RedBoard using Qwiic cable.
  Open Serial Monitor at 115200 baud.
*/

#include <Servo.h>
#include <FastLED.h>
#include <Wire.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>

#include "SparkFun_SCD30_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_SCD30
SCD30 airSensor;

//servo Variable
int val = 0;
// FastLED PIN
#define DATA_PIN    6
//#define CLK_PIN   4
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    3
#define BRIGHTNESS  50
// Define the array of leds
CRGB leds[NUM_LEDS];

int   mapvalue = 0;

int CO2 = 0 ;

Servo myservo_2;


void setup()
{
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
 
  Serial.begin(115200);
  Serial.println("\n");
  Serial.println("CO2ZEIGER_SCD30_WMOS D1 mini");
  Wire.begin();

  // Power up wait
  delay(500);
 if (airSensor.begin() == false)
  {
    Serial.println("Air sensor not detected. Please check wiring. Freezing...");
    while (1)
      ;
  }
 //The SCD30 has data ready every two seconds
delay(6000);

  myservo_2.attach(2);
  Serial.println("\n");
  Serial.print("Servo_auf_0 ");
  Serial.println("\n");
  myservo_2.write(0);
  delay(500);
  Serial.print("Servo_auf_180 ");
  Serial.println("\n");
  myservo_2.write(180);
  delay(500);

  
  delay(2000);

  zeiGer();
  delay(500);
  ledSCD30();
  delay(2000);
 
}
void zeiGer(){
// Servo Details nach kaback

val = airSensor.getCO2();
val = map(val, 200, 2800, 0, 180);
Serial.print("\nVal ");
Serial.print(180-val);
myservo_2.write(180-val);

 }
 
void redKreis(){
    FastLED.clear();
    delay(500);
   for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(500);
  }  
}

void ledSCD30(){
if (airSensor.dataAvailable())
  {
    Serial.println("\n");
    Serial.print("co2(ppm):");
    Serial.print(airSensor.getCO2());

    Serial.print(" temp(C):");
    Serial.print(airSensor.getTemperature(), 1);

    Serial.print(" humidity(%):");
    Serial.print(airSensor.getHumidity(), 1);

    Serial.println();
  }
    else
    Serial.println("\n");
    Serial.println("Waiting for new data");

     delay(2000);

   // LED NEW 
   CO2 = airSensor.getCO2() ;

     if (( ( CO2 ) < ( 1000 ) ))
  {
    for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Green;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Green;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Green;
    FastLED.show();
  }
  }
  else
  {
    if (( ( CO2 ) > ( 2000 ) ))
    {
      for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Red;
    FastLED.show();
    }
    }
    else
    {
    for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Yellow;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Yellow;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Yellow;
    FastLED.show();
    }
  }
 }
}

 
void loop()
{
  zeiGer();
  
  delay(500);
    
  ledSCD30();

  delay(500);
  
}
