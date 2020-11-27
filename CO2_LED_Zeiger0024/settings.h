//#define PIN_CLK  D2
//#define PIN_DATA D1


/**
 * If you're using a cheap D1 clone, these might be the correct pins for you
 * 
 */
//#define PIN_CLK  4 //D2 on PCB
//#define PIN_DATA 5 //D1 on PCB

const float EXP_SMOOTH_ALPHA = 0.125;

const unsigned long PUBLISH_INTERVAL_FAST_MS = (10 * 1000);
const unsigned long PUBLISH_INTERVAL_SLOW_MS = (5 * 60 * 1000);

const char* MQTT_HOST = "my.mqtt.server";
const char* MQTT_USERNAME = "test";
const char* MQTT_PASSWORD = "testpwd";

const uint8_t MQTT_MAX_CONNECT_RETRY = 42;

//If you don't want to use home-assistant autodiscovery comment this out
//#define USE_HA_AUTODISCOVERY


#ifdef USE_HA_AUTODISCOVERY
  #define HA_DISCOVERY_PREFIX "CO2_SDC30_002"
  const char* MQTT_LAST_WILL_PAYLOAD_CONNECTED = "online";
  const char* MQTT_LAST_WILL_PAYLOAD_DISCONNECTED = "offline";
#else
  //If you're not using HA Autodiscovery, you can specify your topics here

  const char* MQTT_TOPIC_CO2_MEASUREMENT = "sensor/co2";
  const char* MQTT_TOPIC_TEMPERATURE_MEASUREMENT = "sensor/temperature/";
  const char* MQTT_TOPIC_LAST_WILL = "state/sensor/co2Zeiger";
  const char* MQTT_LAST_WILL_PAYLOAD_CONNECTED = "connected";
  const char* MQTT_LAST_WILL_PAYLOAD_DISCONNECTED = "disconnected";
#endif


//These are optional
//const char* HOSTNAME = "ESP-co2Zeiger";
//const char* CONF_WIFI_PASSWORD = "drowssap";

//#define OTA_PASSWORD ""
