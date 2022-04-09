/**
 * https://github.com/jnsdbr/esp32-ota-update-mqtt
 */

#ifndef __UPDATE_OVER_MQTT_H__
#define __UPDATE_OVER_MQTT_H__

#include <Arduino.h>
#include <Update.h>
#include <WiFi.h>

#include "DebugPrint.h"

namespace omnicoach
{

class UpdateOverMqtt
{
public:
    UpdateOverMqtt(WiFiClient& connectionClient) : client(connectionClient) {}
    void update(String url, int port);
private:
    String getHeaderValue(String header, String headerName);
    String getBinName(String url);
    String getHostName(String url);
private:
    int contentLength = 0;
    bool isValidContentType = false;
    WiFiClient& client;
};

} // omnicoach
#endif