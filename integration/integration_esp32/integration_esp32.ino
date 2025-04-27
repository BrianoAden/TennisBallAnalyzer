/*
* The purpose of this file is to integrate the rotary encoder and load cell, so that they may be used simultaneously.
*/

#include "HX711_ADC.h"
#include "LiquidCrystal_I2C.h"
#include "math.h"

//https://github.com/olkal/HX711_ADC documentation for library!
//credits to DIYEngineers for helping with calibration https://www.diyengineers.com/2022/05/19/load-cell-with-hx711-how-to-use-with-examples/
//credits to https://electricdiylab.com/how-to-connect-optical-rotary-encoder-with-arduino/ for rotary encoder help

int stiff_co = 0;
int flag = 0;
float forward_deformation = 0;

//timing
unsigned long t = 0;

const int ENCODER_A = 32;
const int ENCODER_B = 33;

// HX711 circuit wiring + macros
const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 0;
float data = 0;

//calibrate load cell (value obtained from load_cell_calibration.ino)
const float calibrationValue = 201.64;
float displacement = 0;


//Rotary Encoder constants
volatile long temp, encoder_value = 0;  //This variable will increase or decrease depending on the rotation of encoder
//float PINION_DIAMETER = 19.5;        //mm
//float PINION_CIRCUMFERENCE = 61.26;  //mm
float PINION_DIAMETER = 27.3;         //mm
float PINION_CIRCUMFERENCE = 85;  //mm

const int buttonPin = 36;  //pin to read state of Button
int buttonState = 0;


HX711_ADC LoadCell(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
//LiquidCrystal lcd(12, 11, 7, 6, 5, 4); //previous 16x2 LCD display
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  //start the lcd
  //lcd.init();
  //lcd.backlight();

  //set baudrate
  Serial.begin(115200);
  //lcd.clear();
  //lcd.print("Booting...");

  //initilaize load cell
  LoadCell.begin();

  loadCellCalibration();  //calibrate the loadcell

  //lcd.clear();
  //lcd.print("Booting complete");

  pinMode(ENCODER_A, INPUT_PULLUP);  // internal pullup input pin 2

  pinMode(ENCODER_B, INPUT_PULLUP);  // internal pullup input pin 3

  pinMode(14, OUTPUT);

  pinMode(buttonPin, INPUT);  //initialize buttonPin to read inputs

  //Setting up interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoder_isr, CHANGE);
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
      int force = (data / 1000) * 9.81;  //converting grams into force newtons

      if (flag == 0){
        //LCD Print
        //lcd.clear();          // Clears the display
        //lcd.setCursor(0, 0);  // set the cursor to column 0, line 0
        //lcd.print("Force(N):" + String(int(force)));
        //lcd.setCursor(0, 1);  // set the cursor to column 0, line 1
        displacement = -((encoder_value / 1200.0) * PINION_CIRCUMFERENCE);
        //lcd.print("Position:" + String(displacement));  //print out distance traveled in mm
        if (force == 80) {
          forward_deformation = displacement;
          digitalWrite(14, HIGH);
          Serial.print(",");
          Serial.print(displacement);
          flag = 1;
        }
      }else{
        delay(1000);
        //lcd.clear();
        stiff_co = (80)/(displacement/10);
        if ((stiff_co > 108) && (stiff_co < 148)){
          //lcd.print("Competitive");
          Serial.println("Competitive");
        }else{
          //lcd.print("Not Competitive");
          Serial.println("Not Competitive");
        }
        //lcd.setCursor(0,1);
        //lcd.print("FD: " + String(forward_deformation) + "mm");
        Serial.println(stiff_co);
      }

      //Print to Serial Monitor
      // Serial.print("Mass(g):");
      // Serial.println(data);

      // // //testing yields 1200p P/R for our rotary encoder
      // if (encoder_value != temp)
      //   Serial.println(encoder_value);


      newDataReady = 0;
      t = millis();
    }
  }
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.print("Reset!");
    rotaryEncoderReset();
    loadCellCalibration();
    digitalWrite(14, LOW);
    flag = 0;
  }
}

/*
* Helper function to reset and calibrate load cell. Probably not as necessary as rotary encoder reset since the 
* Load cell zeroes itself out anyway.
*/

void loadCellCalibration() {

  //zero out load cell. VERY important.
  unsigned long stabilizingtime = 2000;
  LoadCell.start(stabilizingtime, true);

  LoadCell.setCalFactor(calibrationValue);
}

/*
* Helper function to reset the rotary encoder. This will likely only occur at a user's button press. The purpose is to
* help with measurements of multiple balls without having to manually restart Arduino. 
*/

void rotaryEncoderReset() {

  encoder_value = 0;
  temp = 0;
}

void encoder_isr() {

  // If the state of A changed, it means the encoder has been rotated
  if ((digitalRead(ENCODER_A) == HIGH) != (digitalRead(ENCODER_B) == LOW)) {
    encoder_value--;
  } else {
    encoder_value++;
  }
}
