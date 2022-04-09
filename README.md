# Omnicoach ESP32 alert device

## What is this
This is an ESP32 project created for Omnicoach. The main purpose of this firmware is to use MQTT to listen to remote messages and start blinking a LED when a message arrives.

We use this to broadcast "coffe break invites" to all the colleagues. Omnicoach works in a full remote way, and we were trying to find something that can simulate the "random coffee break" feeling of a physical environment.

A different service is monitoring our virtual office (we use Gather), and when someone enters the coffee lounge there, it broadcasts a message to the MQTT broker. This is received by all the ESP32 devices at our colleagues, notifying them about the "happening" by blinking a LED. This can be turned off via a push-button.

## Capabilities
- Non-blocking blinking of an external LED
- Monitoring of a button for press events
- MQTT connection and message handling on multiple topics
  - OTA over MQTT (special message can be sent to make the ESP32 device update it's firmware)

### Planned capabilities, TODOs
- Automatic turn off of the LED after a given delay
- NVS based on-site configuration for all devices (so we can send the device with the firmware, and let our colleagues update their preferences secretly)

## How to use
- Clone this repository
- Install PlatformIO plugin to VisualStudio Code
- Change settings in `.platformio.ini` as needed
- Install dependencies
- Connect your ESP32 device via USB
- Copy the `include/config_example.h` file to `include/config.h` and modify the parameters to your need
- Build and upload code to the ESP32 device

### MQTT messages
Start blinking the LED:
- Broadcast a raw text message with any content to the `orgName` channel (orgName set in `config.h`)
Update the firmware:
- Broadcast a raw text message to the `orgName/update` channel (orgName set in `config.h`)
  - Message content should be the publicly accessible URL to the new firmware binary file.
    - Example message content: `omnicoach.gg/omnicoach_alert_v_1_0_0.bin`
    - Protocol must be omitted (no http://)
    - Referenced file must be served over HTTP (no HTTPS support).
    - Referenced file must return a 200 HTTP message (no redirect support, eg: 301)

### Wiring
Default settings can be changed in `include/config.h`

LED circuit: `D5 -> resistor -> LED -> GND`
- Alternatively the on-board led can be used with `D2`

Button circuit: `D23 -> button (leg1) + button (leg2) -> GND `

Wiring TODO