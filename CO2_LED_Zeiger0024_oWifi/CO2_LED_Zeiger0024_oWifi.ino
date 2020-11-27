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
//#include <Adafruit_NeoPixel.h>
#include <Wire.h>
//#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
//#include <ESP8266WebServer.h>
//#include "dep/pubsubclient-2.7/src/PubSubClient.cpp"
//#include "dep/WiFiManager-0.15.0/WiFiManager.cpp"

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

//#include "settings.h"

/*#ifdef USE_HA_AUTODISCOVERY
  #define FIRMWARE_PREFIX "esp8266-co2Zeiger"
  char MQTT_TOPIC_LAST_WILL[128];
  char MQTT_TOPIC_CO2_MEASUREMENT[128];
  char MQTT_TOPIC_TEMPERATURE_MEASUREMENT[128];
#endif

WiFiClient wifiClient;
PubSubClient mqttClient;

uint8_t mqttRetryCounter = 0;

char hostname[16];
*/
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

 /*WiFiManager wifiManager;
  int32_t chipid = ESP.getChipId();

  Serial.print("MQTT_MAX_PACKET_SIZE: ");
  Serial.println(MQTT_MAX_PACKET_SIZE);

#ifdef HOSTNAME
  hostname = HOSTNAME;
#else
  snprintf(hostname, 24, "CO2ZEIGER-%X", chipid);
#endif
*/
  
/*
#ifdef USE_HA_AUTODISCOVERY
  snprintf(MQTT_TOPIC_LAST_WILL, 127, "%s/%s/presence", FIRMWARE_PREFIX, hostname);
  snprintf(MQTT_TOPIC_CO2_MEASUREMENT, 127, "%s/%s/%s_%s/state"(airSensor.getCO2(), FIRMWARE_PREFIX, hostname, hostname, "co2");
  snprintf(MQTT_TOPIC_TEMPERATURE_MEASUREMENT, 127, "%s/%s/%s_%s/state", FIRMWARE_PREFIX, hostname, hostname, "temp");
#endif

wifiManager.setTimeout(120);

#ifdef CONF_WIFI_PASSWORD
  wifiManager.autoConnect(hostname, CONF_WIFI_PASSWORD);
#else
  wifiManager.autoConnect(hostname);
#endif

if(!wifiManager.autoConnect(hostname)) {
    Serial.println("failed to connect and hit timeout");
    redKreis();
    delay(100);
    redKreis();
    delay(100);
    redKreis();
    delay(100);
    //Starte Zeiger
    zeiGer();
    //Starte Anzeige
    ledSCD30();
  }
  */
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
  /*
  WiFi.hostname(hostname);
  mqttClient.setClient(wifiClient);
  mqttClient.setServer(MQTT_HOST, 1883);

  Serial.print("Hostname: ");
  Serial.println(hostname);
  */
  
  zeiGer();
  delay(500);
  ledSCD30();
  delay(2000);
  //mqttConnect();
  //delay(2000);
  

 
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
/*
void mqttConnect() {
  while (!mqttClient.connected()) {

    bool mqttConnected = false;
    if (MQTT_USERNAME && MQTT_PASSWORD) {
      mqttConnected = mqttClient.connect(hostname, MQTT_USERNAME, MQTT_PASSWORD, MQTT_TOPIC_LAST_WILL, 1, true, MQTT_LAST_WILL_PAYLOAD_DISCONNECTED);
    } else {
      mqttConnected = mqttClient.connect(hostname, MQTT_TOPIC_LAST_WILL, 1, true, MQTT_LAST_WILL_PAYLOAD_DISCONNECTED);
    }

    if (mqttConnected) {
      Serial.println("Connected to MQTT Broker");
      mqttClient.publish(MQTT_TOPIC_LAST_WILL, MQTT_LAST_WILL_PAYLOAD_CONNECTED, true);
      mqttRetryCounter = 0;
    } else {
      Serial.println("Failed to connect to MQTT Broker");
      ledSCD30();
      if (mqttRetryCounter++ > MQTT_MAX_CONNECT_RETRY) {
        Serial.println("Restarting uC");
        ESP.restart();
      }

      delay(2000);
    }
  }
}


void mqttPublish(){
    String cO2=String("co2: "+String(String(airSensor.getCO2())));
    mqttClient.publish(hostname,cO2.c_str());
    delay(1000);
    String tMp=String("temp: "+String(String(airSensor.getTemperature())));
    mqttClient.publish(hostname,tMp.c_str());
    delay(1000);
    String hUm=String("hum(%): "+String(String(airSensor.getHumidity())));
    mqttClient.publish(hostname,hUm.c_str());
    delay(1000);
    Serial.print("\nmqtt publish: "); 
    Serial.print(cO2);
    Serial.print("\nmqtt publish: ");
    Serial.print(tMp);
    Serial.print("\nmqtt publish: ");
    Serial.print(hUm);
  }
*/
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
  
  //mqttPublish();
  
  //delay(6000);
}
