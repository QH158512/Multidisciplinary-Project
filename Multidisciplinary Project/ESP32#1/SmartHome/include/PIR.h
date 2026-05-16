#ifndef __PIR_H__
#define __PIR_H__
#include <Arduino.h>
#include <Servo.h>
#include "MQTT_Task.h" 

void PIR_Task (void *pvParameters);
void Servo_Task (void *pvParameters);
#endif