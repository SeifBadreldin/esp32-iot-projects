# ESP32 IoT Projects

ESP32 projects from the Internet of Things course (Fall 2025), built as part of a 5-member team.

| # | Project | What it does | Stack |
| --- | --- | --- | --- |
| 01 | [Wi-Fi Scanner](01-wifi-scanner) | Scans nearby access points every 10 s, then sorts them by RSSI and filters open networks and channels 1/6/11 | Arduino C++, MicroPython |
| 02 | [LED Web Server](02-led-web-server) | ESP32 hosts a web page that turns 3 LEDs on/off, switches all off, or runs a traffic-light sequence | Arduino C++, WebServer |
| 03 | [Voice-Controlled Lamp](03-voice-controlled-lamp) | Python speech recognition (Egyptian Arabic) sends commands to the ESP32 over serial | Python, SpeechRecognition, PySerial |
| 04 | [Theft Alert](04-theft-alert-blynk) | Motion-triggered intrusion alert with push notifications through Blynk IoT | Arduino C++, Blynk |

## Configuration
Credentials are not committed. Before uploading a sketch, replace the placeholders:
```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"
```

**Hardware:** ESP32 DevKit, LEDs, PIR sensor
