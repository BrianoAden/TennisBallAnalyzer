/*
* The purpose of this file is to integrate the rotary encoder and load cell, so that they may be used simultaneously.
*/

#include "HX711_ADC.h"
#include "LiquidCrystal.h"

//https://github.com/bogde/HX711 documentation for library!
//credits to DIYEngineers for helping with calibration https://www.diyengineers.com/2022/05/19/load-cell-with-hx711-how-to-use-with-examples/

//timing
unsigned long t = 0;

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 14;
const int LOADCELL_SCK_PIN = 15;

//Rotary Encoder constants
volatile byte val;
byte prevVal;
long clicks = 0;
const float DEGREES_PER_CLICK = 0.15;
float degrees;
const float PINION_RADIUS = 12.5; // Pinion radius in mm
const float GEAR_RADIUS = 10.35; // Gear radius in mm
const float GEAR_CIRCUMFERENCE = 65; // Gear circumference in mm

HX711_ADC LoadCell(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

void setup() {
  //start the lcd
  lcd.begin(16,2);

  //set baudrate
  Serial.begin(115200);
  Serial.print("Booting...");

  //initilaize load cell
  LoadCell.begin();

  //zero out load cell. VERY important.
  unsigned long stabilizingtime = 2000;
  LoadCell.start(stabilizingtime, true);

  //calibrate load cell (value obtained from load_cell_calibration.ino)
  float calibrationValue = 213.32;
  LoadCell.setCalFactor(calibrationValue);

  Serial.println("Booting complete");
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(2), angleISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), angleISR, CHANGE);
  val=0;
  prevVal = -1;
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

    // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      float force = (i/1000) * 9.81; //converting grams into force newtons 
      Serial.print("Load_cell output val: ");
      Serial.println(i);
      if(val != prevVal){
        degrees = angleCalc(val, prevVal);
        prevVal = val;
      }
      //LCD Print
      lcd.clear(); // Clears the display 
      lcd.setCursor(0,0);  // set the cursor to column 0, line 0
      lcd.print("Force(N): " + String(int(force)));
      lcd.setCursor(0,1);  // set the cursor to column 0, line 1
      lcd.print("Position: " + String(degrees, 2));
      newDataReady = 0;
      t = millis();
    }
  }
  // currentMillis = millis();
  // mass;
  // //this is NECESSARY! only calculate reading every 500ms so it isn't continuously taking processing power from the arduino and messing up rotary encoder
  // if (currentMillis - lastTime >= interval) {
  //   lastTime = currentMillis;
  //   reading = scale.read();
  //   mass=reading/207.3516-2942.43;
  // }
  //if (scale.is_ready()) {
  // if(val != prevVal){
  //   degrees = angleCalc(val, prevVal);
  //   prevVal = val;
  // }
  // lcd.setCursor(0, 0);
  // lcd.print("Force: ");
  //lcd.print(reading);
  //lcd.print(": ");
  // lcd.print(mass);
  // lcd.setCursor(0, 1);
  // lcd.print("Position: ");
  // lcd.println(degrees, 2); 
  //} else {
    //Serial.print("HX711 not found.");
  //}
}

void angleISR(){
  val = digitalRead(2) + 2*digitalRead(3);
}

float angleCalc(byte value, byte prevValue){
  switch(value){
    case 0:
      if (prevValue ==2)
        clicks++;
      else if(prevValue == 1)
        clicks--;
      break;
    case 1:
      if (prevValue == 0)
        clicks++;
      else if(prevValue == 3)
        clicks--;
      break;
    case 2:
      if (prevValue == 3)
        clicks++;
      else if(prevValue == 0)
        clicks--;
      break;
    case 3:
      if (prevValue == 1)
        clicks++;
      else if(prevValue == 2)
        clicks--;
      break;
  }
  return (clicks * DEGREES_PER_CLICK * PINION_RADIUS * PI/180.0);
}
