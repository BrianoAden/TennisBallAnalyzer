/*
* The purpose of this file is to integrate the rotary encoder and load cell, so that they may be used simultaneously.
*/

#include "HX711_ADC.h"
#include "LiquidCrystal.h"

//https://github.com/olkal/HX711_ADC documentation for library!
//credits to DIYEngineers for helping with calibration https://www.diyengineers.com/2022/05/19/load-cell-with-hx711-how-to-use-with-examples/
//credits to https://electricdiylab.com/how-to-connect-optical-rotary-encoder-with-arduino/ for rotary encoder help

//timing
unsigned long t = 0;

// HX711 circuit wiring + macros
const int LOADCELL_DOUT_PIN = 14;
const int LOADCELL_SCK_PIN = 15;
float data = 0;

//calibrate load cell (value obtained from load_cell_calibration.ino)
const float calibrationValue = 213.32;


//Rotary Encoder constants
volatile long temp, counter = 0;     //This variable will increase or decrease depending on the rotation of encoder
//float PINION_DIAMETER = 19.5;        //mm
//float PINION_CIRCUMFERENCE = 61.26;  //mm
float PINION_DIAMETER = 27.3;        //mm
float PINION_CIRCUMFERENCE = 85.765;  //mm

const int buttonPin = 8; //pin to read state of Button
int buttonState; 


HX711_ADC LoadCell(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

void setup() {
  //start the lcd
  lcd.begin(16, 2);

  //set baudrate
  Serial.begin(115200);
  lcd.clear();
  lcd.print("Booting...");

  //initilaize load cell
  LoadCell.begin();

  loadCellCalibration(); //calibrate the loadcell

  lcd.clear();
  lcd.print("Booting complete");

  pinMode(2, INPUT_PULLUP);  // internal pullup input pin 2

  pinMode(3, INPUT_PULLUP);  // internal pullup input pin 3

  pinMode(buttonPin, INPUT); //initialize buttonPin to read inputs

  //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(0, ai0, RISING);

  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(1, ai1, RISING);
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0;  //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {

      data = LoadCell.getData();
      float force = (data / 1000) * 9.81;  //converting grams into force newtons

      //Print to Serial Monitor
      Serial.print("Mass(g):");
      Serial.println(data);

      //testing yields 1200p P/R for our rotary encoder
      if (counter != temp) {
        Serial.println(counter);
        temp = counter;
      }

      //LCD Print
      lcd.clear();          // Clears the display
      lcd.setCursor(0, 0);  // set the cursor to column 0, line 0
      lcd.print("Force(N):" + String(int(force)));
      lcd.setCursor(0, 1);  // set the cursor to column 0, line 1
      lcd.print("Position:" + String((counter/1200.0)*PINION_CIRCUMFERENCE));  //print out distance traveled in mm

      newDataReady = 0;
      t = millis();
    }
  }
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH)
    rotaryEncoderReset();
}

/*
* Helper function to reset and calibrate load cell. Probably not as necessary as rotary encoder reset since the 
* Load cell zeroes itself out anyway.
*/

void loadCellCalibration(){

  //zero out load cell. VERY important.
  unsigned long stabilizingtime = 2000;
  LoadCell.start(stabilizingtime, true);

  LoadCell.setCalFactor(calibrationValue);
}

/*
* Helper functoin to reset the rotary encoder. This will likely only occur at a user's button press. The purpose is to
* help with measurements of multiple balls without having to manually restart Arduino. 
*/

void rotaryEncoderReset(){

  counter = 0;
  temp = 0; 
}

void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if (digitalRead(3) == LOW) {
    counter++;
  } else {
    counter--;
  }
}

void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if (digitalRead(2) == LOW) {
    counter--;
  } else {
    counter++;
  }
}
