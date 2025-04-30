
//https://github.com/olkal/HX711_ADC documentation for library!
//credits to DIYEngineers for helping with calibration https://www.diyengineers.com/2022/05/19/load-cell-with-hx711-how-to-use-with-examples/
//credits to https://electricdiylab.com/how-to-connect-optical-rotary-encoder-with-arduino/ for rotary encoder help

//timing
unsigned long t = 0;

const int ENCODER_A = 32;
const int ENCODER_B = 33;

//Rotary Encoder constants
volatile long temp, encoder_value = 0;  //This variable will increase or decrease depending on the rotation of encoder
float PINION_DIAMETER = 27.3;         //mm
float PINION_CIRCUMFERENCE = 85;  //mm



void setup() {

  //set baudrate
  Serial.begin(115200);

  pinMode(ENCODER_A, INPUT_PULLUP);  // internal pullup input pin 2

  pinMode(ENCODER_B, INPUT_PULLUP);  // internal pullup input pin 3

  //Setting up interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoder_isr, CHANGE);
}

void loop() {
  const int serialPrintInterval = 0;  //increase value to slow down serial print activity

  // get smoothed value from the dataset:
    if (millis() > t + serialPrintInterval) {

      // // //testing yields S1200p P/R for our rotary encoder
      if (encoder_value != temp)
        Serial.println(encoder_value);
        
      t = millis();
    }
}


void encoder_isr() {

  // If the state of A changed, it means the encoder has been rotated
  if ((digitalRead(ENCODER_A) == HIGH) != (digitalRead(ENCODER_B) == LOW)) {
    encoder_value--;
  } else {
    encoder_value++;
  }
}
