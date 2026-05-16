#include "PIR.h"

//bool motionDetected = false;

void PIR_Task (void *pvParameters)
{
    // put your setup code here, to run once:
    Serial.println("PIR Task Started");
    pinMode(GPIO_NUM_27, INPUT);

    //------------------------------
    bool prevMotionDetected = false;
    //------------------------------

    while (1) {
        int pirValue = digitalRead(GPIO_NUM_27);
        motionDetected = (pirValue == HIGH);

        //---------------------------
        if (motionDetected != prevMotionDetected) {
            Serial.print("PIR state changed: ");
            Serial.print(motionDetected ? "MOTION DETECTED" : "NO MOTION");
            Serial.print(" (pirValue=");
            Serial.print(pirValue);
            Serial.println(")");
            prevMotionDetected = motionDetected;
        }
        //---------------------------


        //Let other tasks run
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void Servo_Task (void *pvParameters)
{
    Serial.println("Servo Task Started");
    Servo myServo;
    myServo.attach(GPIO_NUM_16); 

    while (1) {
        // Open if PIR detects motion OR if Adafruit button is toggled ON
        if (remoteDoorOpen) {
            myServo.write(160); // Move to Open position
        } else {
            myServo.write(0);   // Move to Closed position
        }
        
        // Higher delay for servo task to prevent jitter
        vTaskDelay(pdMS_TO_TICKS(200)); 
    }
}