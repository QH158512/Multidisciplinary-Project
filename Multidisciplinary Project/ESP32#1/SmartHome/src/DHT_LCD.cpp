#include "DHT_LCD.h"
#include "MQTT_Task.h"

DHTesp dht;

LiquidCrystal_I2C lcd(0x27,20,4);

void DHT_LCD_Task (void *pvParameters)
{
  // put your setup code here, to run once:
  Serial.println("Peripheral Task Started");

  dht.setup(GPIO_NUM_26, DHTesp::DHT22);
  Serial.println("TESTING PRINT OUT TERMINAL");
  

  // Init LCD
  lcd.init();          // initialize the lcd 
  lcd.backlight();     // turn on backlight

  // // Optional welcome screen
  lcd.setCursor(2, 0);
  lcd.print("Hello World");
  delay(2000);
  lcd.clear();

  while (1) {
    TempAndHumidity  data = dht.getTempAndHumidity();
    // Update global variables for the MQTT task to pick up
    global_temp = data.temperature;
    global_hum = data.humidity;

    Serial.println("Temp: " + String(data.temperature, 2) + "°C");
    Serial.println("Humidity: " + String(data.humidity, 2) + "%");
    Serial.println("---");


    lcd.clear();
    //Row 0: Temperature 
    lcd.setCursor(0, 0);    
    lcd.print("Temp: "); 
    lcd.print(data.temperature, 1); // 1 decimal 
    lcd.print((char)223); // degree symbol 
    lcd.print("C "); // spaces to clear old data 

    // Row 1: Humidity 
    lcd.setCursor(0, 1); 
    lcd.print("Humidity: "); 
    lcd.print(data.humidity, 1); 
    lcd.print("% "); 


    //vTaskDelay(2000);
    vTaskDelay(5000);
  }

}