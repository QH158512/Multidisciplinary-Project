#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <BH1750.h>
#include <Wire.h>

// --- Cấu hình WiFi ---
#define WLAN_SSID       "	"	//Wifi name
#define WLAN_PASS       "	"	//Wifi password

// --- Cấu hình Adafruit IO ---
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "	"		//User name on adafruit.io
#define AIO_KEY         "	" 		//Key on adafruit account

// --- Cấu hình chân L298N ---
const int ENA = 15; 
const int IN1 = 2;
const int IN2 = 4;

// PWM Settings cho ESP32
const int freq = 5000;
const int pwmChannel = 0;
const int resolution = 8; // 0-255

BH1750 lightMeter;
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// --- Cấu hình Feeds ---
// Feed gửi dữ liệu Lux (Publish)
Adafruit_MQTT_Publish light_feed = Adafruit_MQTT_Publish(&mqtt,"khangtranWoT/feeds/sensor-light");

// Feed nhận tốc độ quạt (Subscribe)
Adafruit_MQTT_Subscribe fan_feed = Adafruit_MQTT_Subscribe(&mqtt, "khangtranWoT/feeds/fan");

void MQTT_connect();

void setup() {
  Serial.begin(115200);

  // Cấu hình chân L298N
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, HIGH); // Quay thuận
  digitalWrite(IN2, LOW);

  // Cấu hình PWM cho chân ENA
  ledcAttachChannel(ENA, freq, resolution, pwmChannel);

  // Khởi tạo I2C và cảm biến
  Wire.begin();
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 OK"));
  }

  // Kết nối WiFi
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  // Đăng ký nhận dữ liệu từ Feed Fan
  mqtt.subscribe(&fan_feed);
}

uint32_t lastRead = 0;

void loop() {
  MQTT_connect();

  // 1. NHẬN DỮ LIỆU TỪ ADAFRUIT (Tốc độ quạt)
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(500))) {
    if (subscription == &fan_feed) {
      // Chuyển giá trị từ chuỗi sang số (0-100)
      int sliderValue = atoi((char *)fan_feed.lastread);
      Serial.print("New Fan Speed: ");
      Serial.print(sliderValue);
      Serial.println("%");

      // Chuyển đổi từ % (0-100) sang PWM (0-255)
      int pwmValue = map(sliderValue, 0, 100, 0, 255);
      ledcWrite(ENA, pwmValue);
    }
  }

  // 2. GỬI DỮ LIỆU LUX LÊN ADAFRUIT (Mỗi 15 giây)
  if (millis() - lastRead > 10000) {
    float lux = lightMeter.readLightLevel();
    Serial.print(F("Sending Lux: "));
    Serial.println(lux);
    
    if (!light_feed.publish(lux)) {
      Serial.println(F("Failed to publish Lux"));
    }
    lastRead = millis();
  }
}

// Hàm kết nối MQTT (tự động kết nối lại nếu mất mạng)
void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) return;

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) while (1);
  }
  Serial.println("MQTT Connected!");
}