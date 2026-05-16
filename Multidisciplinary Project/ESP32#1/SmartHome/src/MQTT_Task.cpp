#include "MQTT_Task.h"


// --- WiFi Configuration ---
const char* ssid = "    ";         // WiFi Name
const char* password = "    "; // WiFi Password

// --- Adafruit IO Configuration ---
#define IO_SERVER      "io.adafruit.com"
#define IO_PORT        1883
#define IO_USERNAME    "   "   //USER NAME ON ADAFRUIT.IO
#define IO_KEY         "   "   //AIO KEY ON ADAFRUIT.IO

const char* TEMP_TOPIC = "khangtranWoT/feeds/sensor-temp";
const char* HUM_TOPIC  = "khangtranWoT/feeds/sensor-humid";
const char* LED_TOPIC  = "khangtranWoT/feeds/button-light";
const char* DOOR_TOPIC = "khangtranWoT/feeds/button-door";
const char* MOTION_TOPIC = "khangtranWoT/feeds/sensor-motion";

// Global variables (shared across PIR.cpp, DHT_LCD.cpp, etc.)
float global_temp = 0.0;
float global_hum  = 0.0;
bool motionDetected = false;
bool remoteDoorOpen = false;

WiFiClient espClient;
PubSubClient client(espClient);

// --- 1. Callback: Route incoming dashboard commands ---
void callback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (int i = 0; i < length; i++) message += (char)payload[i];
    String top = String(topic);

    Serial.printf("MQTT Incoming [%s]: %s\n", topic, message.c_str());

    // Logic for Door (Servo)
    if (top == DOOR_TOPIC) {
        remoteDoorOpen = (message == "1" || message == "ON");
    } 
    // Logic for LED
    else if (top == LED_TOPIC) {
        // Convert the string "75" to the number 75
        int intensity = message.toInt(); 
        
        // Call your new PWM function from LED.cpp
        LED_SetIntensity(intensity);
    }
}

// --- 2. Reconnect Logic: Subscribe to all control feeds ---
void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32_Hung_Project", IO_USERNAME, IO_KEY)) {
            Serial.println("Connected!");
            
            // Subscribe to both buttons
            client.subscribe(DOOR_TOPIC);
            client.subscribe(LED_TOPIC);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }
}


// --- 3. Main MQTT Task Loop ---
void MQTT_Task(void *pvParameters) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { vTaskDelay(pdMS_TO_TICKS(500)); }
    
    client.setServer(IO_SERVER, IO_PORT);
    client.setCallback(callback);
    LED_Init(); 

    unsigned long lastDHTPublish = 0;
    unsigned long lastPIRPublish = 0;
    
    // Variables to track last published values for change detection
    float last_temp = -99.0;
    float last_hum = -99.0;
    bool last_motion = false;

    while (1) {
        if (!client.connected()) { reconnect(); }
        client.loop(); 
        unsigned long now = millis();

        // 1. KIỂM TRA DHT (Mỗi 4 giây)
        if (now - lastDHTPublish >= 4000) {
            // Only publish if there's a significant change or it's been 20s since last publish
            if (abs(global_temp - last_temp) >= 0.5 || abs(global_hum - last_hum) >= 2.0 || (now - lastDHTPublish >= 20000)) {
                lastDHTPublish = now;
                last_temp = global_temp;
                last_hum = global_hum;

                char tBuf[10], hBuf[10];
                dtostrf(global_temp, 1, 1, tBuf);
                dtostrf(global_hum, 1, 1, hBuf);

                client.publish(TEMP_TOPIC, tBuf);
                client.publish(HUM_TOPIC, hBuf);
     
                Serial.printf(">>> MQTT DHT Published: %s\n", tBuf, hBuf);
            }
        }

        // Check PIR each 6s
        if (now - lastPIRPublish >= 6000) {
            // Chỉ gửi khi trạng thái chuyển động thay đổi (Có người -> Không người hoặc ngược lại)
            if (motionDetected != last_motion) {
                lastPIRPublish = now;
                last_motion = motionDetected;

                client.publish(MOTION_TOPIC, motionDetected ? "1" : "0");
                Serial.printf(">>> MQTT PIR Changed: %d\n", motionDetected);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50)); 
    }
}

