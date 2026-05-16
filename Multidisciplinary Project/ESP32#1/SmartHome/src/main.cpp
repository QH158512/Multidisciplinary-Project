#include "DHT_LCD.h"
#include "PIR.h"
#include "MQTT_Task.h"
#include "LED.h"




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  xTaskCreate(DHT_LCD_Task, "Task DHT_LCD", 2048, NULL, 2, NULL);
  xTaskCreate(PIR_Task, "Task PIR", 2048, NULL, 1, NULL);
  xTaskCreate(Servo_Task, "Task Servo", 2048, NULL, 1, NULL);
  xTaskCreate(MQTT_Task, "MQTT_Task", 4096, NULL, 1, NULL);


  
}

void loop() {
  
}


