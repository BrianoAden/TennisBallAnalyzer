#include<LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);

}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("HELLO WORLD");
  // put your main code here, to run repeatedly:

}
