# ESP32 WiFi REST call

This project uses the **Heltec ESP32** for connecting to my WLAN and sending HTTP GET and HTTP POST requests against a REST service hosted on Raspberry PI.

## Configuration

Create the file `platformio.ini`:

```ini
[env:heltec_wifi_lora_32_V2]
platform = espressif32
board = heltec_wifi_lora_32_V2
build_flags = 
	-DWIFI_SSID='"change-me"'
	-DWIFI_PASSWORD='"change-me"'
framework = arduino
lib_deps = 
	heltecautomation/Heltec ESP32 Dev-Boards@^1.1.0
	bblanchon/ArduinoJson@^6.19.2
monitor_speed = 115200
```
