#ifndef __LED_H__
#define __LED_H__

#include <Arduino.h>

#define LED_PIN 12
#define LEVER_PIN 35

void LED_Init();
void LED_SetState(bool state);
void LED_SetIntensity(int percentage);

#endif