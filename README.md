# IoT Smart Home System (Dual-ESP32 via Adafruit IO)

An advanced Internet of Things (IoT) Smart Home System utilizing two independent **ESP32** microcontrollers. The boards communicate and synchronize in real-time over the **MQTT** protocol via the **Adafruit IO** cloud platform. This system is heavily optimized using an **Event-Driven publishing mechanism** and **FreeRTOS** task scheduling to stay within API rate limits.

---

## System Architecture & Core Modules

This project satisfies 4 core requirements of the academic IoT curriculum:

*   **Module 1: Input Connectivity (Sensors)** -> Live telemetry acquisition from a DHT11/22 (Temperature & Humidity), a PIR Motion Sensor, and a BH1750 Ambient Light Sensor.
*   **Module 2: Threshold & Logic Analysis** -> Automated threshold verification. The system processes raw data locally and raises warnings immediately if environmental factors exceed safe boundaries.
*   **Module 3: Output Control (Actuators)** -> Automated and manual control over a Servo Motor (Smart Door), a dimmable LED (PWM dimming control), and a high-power cooling fan (L298N H-Bridge).
*   **Module 4: Data Logging & Telemetry Dashboard** -> Comprehensive data streaming to a unified Adafruit IO Dashboard utilizing bandwidth protection techniques.

---

## Hardware Pinout Configurations

### Central Controller (ESP32 Board #1)
Manages ambient environmental checking, security detection, and entryway access. This board utilizes **FreeRTOS** multi-tasking to guarantee deterministic response times.

Devices:
**DHT11/22 Sensor**: Reads room temperature & humidity 
**PIR Motion Sensor**: Detects human motion / intrusion 
**Servo Motor**: Simulates smart door lock actuation 
**LED Light**: Dimmable overhead light source 
**LCD I2C (16x2)**: Real-time on-site local display 

### Auxiliary Actuator Controller (ESP32 Board #2)
Manages independent luminosity sensing and handles heavy-load cooling actuation.

Devices
**BH1750 Sensor**: Measures ambient light level (Lux) 
**DC Fan**: Variable fan speed control (0-100%) 


## MQTT Configuration (Adafruit IO Feeds)

Ensure that you create the matching feed paths exactly as specified below within your Adafruit IO workspace:

*   `khangtranWoT/feeds/sensor-temp` (Receives the combined Temp/Humid/Alarm payload from ESP32 #1)
*   `khangtranWoT/feeds/sensor-motion` (Receives PIR binary status 0/1 from ESP32 #1)
*   `khangtranWoT/feeds/button-light` (Sends LED dimming commands down to ESP32 #1)
*   `khangtranWoT/feeds/button-door` (Sends Remote Lock/Unlock triggers down to ESP32 #1)
*   `khangtranWoT/feeds/sensor-light` (Receives raw Lux values from ESP32 #2)
*   `khangtranWoT/feeds/fan` (Sends Fan speed 0-100% commands down to ESP32 #2)

---

## Deployment & Installation

1. Install the required dependencies in **Arduino IDE / PlatformIO** library manager:
   * `PubSubClient` (For ESP32 #1)
   * `Adafruit_MQTT_Library` (For ESP32 #2)
   * `BH1750`
   * `DHT sensor library`
2. Open the source code files and update your local network configurations:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   #define IO_KEY "YOUR_ADAFRUIT_IO_ACTIVE_KEY"
