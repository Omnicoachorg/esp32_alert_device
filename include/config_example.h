
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <Arduino.h>

namespace omnicoach
{

struct Config
{
    
/****************
 * Global settings
 ************** */
    const String clientName = "clientName";
    const String orgName = "orgName";
    const bool useMqtt = true;
     
/****************
 * WIFI settings
 ************** */
    const String wifiSsid = "wifiSsid";
    const String wifiPassword = "wifiPassword!";
        
/****************
 * MQTT settings
 ************** */
    const String mqttServer = "mqttServer.example";
    const bool mqttUseAuth = true;
    const String mqttUser = "mqttUser";
    const String mqttPassword = "mqttPassword";
    const uint16_t mqttPort = 1883;
  
/****************
 * Button settings
 ************** */
    const uint16_t btnDebounceTime = 100; // the debounce time in millisecond, increase this time if it still chatters
    const uint8_t btnPinId = 23;
    
/****************
 * LED settings
 ************** */
    const uint8_t ledPinId = 5;
    const uint16_t ledBlinkTime = 500;
    const uint16_t ledTimeout = 30000;
    const bool ledTimeoutOn = true;
    bool shouldLedFlash = true;
};

} // omnicoach

#endif
