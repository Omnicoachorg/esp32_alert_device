#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Preferences.h>
#include <Update.h>

#include <ezButton.h>

#include "DebugPrint.h"
#include "ArduinoNVS.h"
#include "UpdateOverMqtt.h"
#include "Config.h"

/****************
 * Global settings
 ************** */
const omnicoach::Config config;
const String clientName = config.clientName;
const String orgName = config.orgName;
const bool useMqtt = config.useMqtt;

/****************
 * WIFI settings
 ************** */
const String wifiSsid = config.wifiSsid;
const String wifiPassword = config.wifiPassword;
WiFiClient client;
omnicoach::UpdateOverMqtt updater(client);

/****************
 * MQTT settings
 ************** */
const String mqttServer = config.mqttServer;
const bool mqttUseAuth = config.mqttUseAuth;
const String mqttUser = config.mqttUser;
const String mqttPassword = config.mqttPassword;
const uint16_t mqttPort = config.mqttPort;

const String mqtt_client_name = clientName;
const String mqtt_pub_topic = orgName + "/" + clientName;
const String mqtt_sub_topic = orgName;
const String mqtt_sub_topic_update = orgName + "/" + "update";
PubSubClient mqttClient(client);

/****************
 * Button settings
 ************** */
const uint16_t btnDebounceTime = config.btnDebounceTime; // the debounce time in millisecond, increase this time if it still chatters
const uint8_t btnPinId = config.btnPinId;
ezButton button(btnPinId);

/****************
 * LED settings
 ************** */
const uint8_t ledPinId = config.ledPinId;
const uint16_t ledBlinkTime = config.ledBlinkTime;
// const uint16_t ledTimeout = config.ledTimeout;
// const bool ledTimeoutOn = config.ledTimeoutOn;
bool shouldLedFlash = config.shouldLedFlash;

bool isLedOn = false;
unsigned long lastButtonFlashAction = 0;
unsigned long ledFlashingStartedAt = 0;

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  DEBUG_PRINT("Message received from: ");
  DEBUG_PRINTLN(topic);

  payload[length] = '\0';
  String _message = String((char *)payload);
  String _topic = String(topic);

  if (_topic == mqtt_sub_topic)
  {
    shouldLedFlash = true;
    ledFlashingStartedAt = millis();
  }
  if (_topic == mqtt_sub_topic_update)
  {
    updater.update(_message, 80);
  }
}

void handleButton()
{
  button.loop();

  if (button.isReleased() || button.isPressed())
  {
    DEBUG_PRINTLN("Button pressed");
    shouldLedFlash = false;
    digitalWrite(ledPinId, LOW);
  }
}

void handleLed(unsigned long currentMillis)
{
  if (!shouldLedFlash)
    return;

  if ((unsigned long)(currentMillis - lastButtonFlashAction) >= ledBlinkTime)
  {
    lastButtonFlashAction = currentMillis;
    digitalWrite(ledPinId, isLedOn ? LOW : HIGH);
    isLedOn = !isLedOn;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(ledPinId, OUTPUT);
  button.setDebounceTime(btnDebounceTime);

  if (useMqtt)
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiSsid.c_str(), wifiPassword.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      DEBUG_PRINT(".");
    }
    DEBUG_PRINTLN("");
    DEBUG_PRINT("WiFi connected to: ");
    DEBUG_PRINTLN(wifiSsid);
    DEBUG_PRINTLN("IP address: ");
    DEBUG_PRINTLN(WiFi.localIP());
    delay(2000);
    mqttClient.setBufferSize(1024);
    mqttClient.setServer(mqttServer.c_str(), mqttPort);
    mqttClient.setCallback(mqttCallback);
  }
}

void loop()
{
  if (useMqtt && !mqttClient.connected())
  {
    while (!mqttClient.connected())
    {
      bool connected = mqttUseAuth ? mqttClient.connect(mqtt_client_name.c_str(), mqttUser.c_str(), mqttPassword.c_str()) : mqttClient.connect(mqtt_client_name.c_str());
      if (connected)
      {
        DEBUG_PRINTLN("MQTT Connected!");
        mqttClient.subscribe(mqtt_sub_topic.c_str());
        mqttClient.subscribe(mqtt_sub_topic_update.c_str());
      }
      else
      {
        DEBUG_PRINT("x");
      }
    }
  }

  unsigned long currentMillis = millis();
  handleButton();
  handleLed(currentMillis);

  if (useMqtt)
  {
    mqttClient.loop();
  }
}
