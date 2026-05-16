#ifndef __MQTT_TASK_H__
#define __MQTT_TASK_H__

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "LED.h"
#include "PIR.h" 
#include "Config.h"

// Global variables to share data between DHT task and MQTT task
extern float global_temp;
extern float global_hum;
extern bool motionDetected;
extern bool remoteDoorOpen;

void MQTT_Task(void *pvParameters);

#endif