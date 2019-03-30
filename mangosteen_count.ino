#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;

#define DEC_POINT  2
float get_units_kg();
float offset=-0.13;
float calibration_factor = 169885.00;


void setup()
{
  int error;

  Serial.begin(115200);
  Serial.println("LCD...");

  while (! Serial);

  Serial.println("Dose: check for LCD");

  // See http://playground.arduino.cc/Main/I2cScanner
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");

  } else {
    Serial.println(": LCD not found.");
  } // if

  lcd.begin(16, 2); // initialize the lcd
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

} // setup()

float weight = 0;
float pre_weight = 0;
float total_weight = 0;
int count = 0;
int prepare_count = 0;
void loop()
{

    // พิมพ์น้ำหนักและจำนวนมังคุดออกจอ LCD
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.setCursor(0, 0);
    scale.set_scale(calibration_factor); //Adjust to this calibration factor
    String data = String(get_units_kg()+offset, DEC_POINT);
    Serial.print(data);
    Serial.println(" kg");
    lcd.print("Weight:");
    lcd.print(data);
    lcd.setCursor(0, 1);
    weight = data.toFloat();
    if (weight >= 0.02) {
      delay(200);
      prepare_count = 1;   
      pre_weight = weight;  
      
    }

    if (prepare_count == 1) {
      if (weight < 0.02) {
        count++;
        total_weight += pre_weight;
        prepare_count = 0;
      }
    }

    //พิมพ์จำนวนมังคุด   
    lcd.print("C:");
    lcd.print(count);

    //พิมพ์น้ำหนักรวม
    lcd.setCursor(8, 1);
    lcd.print("T:");
    lcd.print(total_weight);
    
    

 
  delay(100);
} // loop()

float get_units_kg()
{
  return(scale.get_units()*0.453592);
}
