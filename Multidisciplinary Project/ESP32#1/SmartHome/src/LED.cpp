#include "LED.h"



void LED_Init() {
    // OLD syntax (pre-2024)
    ledcSetup(12, 5000, 8);      // Channel 0, 5kHz, 8-bit
    ledcAttachPin(LED_PIN, 12);  // Attach GPIO to Channel 0
}

void LED_SetIntensity(int percentage) {
    // Basic scaling: (percentage / 100) * 255
    int dutyCycle = (percentage * 255) / 100;
    
    // Applying the PWM value to GPIO 12
    ledcWrite(12, dutyCycle); 
}