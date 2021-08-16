#include "GenesisInput.h"

//Reads Sega Genesis gamepad buttons and prints state to serial monitor
/*
  Pin      Name (Select=GND)      Name (Select=+5V)
  1       Up       Up
  2       Down       Down
  3       Gnd       Left
  4       Gnd       Right
  5       +5VDC       +5VDC
  6       Button A       Button B
  7       Select       Select
  8       Ground       Ground
  9       Start       Button C
*/

void GenesisButtons::setup() {
  pinMode(JOY_PIN_1, INPUT);
  pinMode(JOY_PIN_2, INPUT);
  pinMode(JOY_PIN_3, INPUT);
  pinMode(JOY_PIN_4, INPUT);
  pinMode(JOY_PIN_6, INPUT);
  pinMode(JOY_PIN_7, INPUT);
  pinMode(JOY_PIN_7, OUTPUT);
  pinMode(JOY_PIN_9, INPUT);
  digitalWrite(BUTTON_SELECT, LOW);
  delay(selectToggleTime);
}

bool GenesisButtons::updateGenesisButtons() {
  int tickTime = millis() - startTime;
  if (tickTime < 0) {
    //counter rolled over
    tickTime = 0 - tickTime;
    startTime = selectToggleTime - tickTime;
  }

  if (tickTime > selectToggleTime) {
    readButtons( currentButtonGroup);
    currentButtonGroup ^= HIGH;
    digitalWrite(BUTTON_SELECT, currentButtonGroup);
    startTime = millis();
  }
  return buttonsChanged();
}

int GenesisButtons::readButton(int pin) {
  return digitalRead(pin) ^ 1;
}

void GenesisButtons::readButtons(int buttonGroup) {
  switch (buttonGroup) {
    case 0:
      //read these buttons after setting the Select pin low
      buttonUp    = readButton(BUTTON_UP);
      buttonDown  = readButton(BUTTON_DOWN);
      buttonA     = readButton(BUTTON_A);
      buttonStart = readButton(BUTTON_START);
      break;
    case 1:
      //read these buttons after setting the Select pin high
      buttonLeft  = readButton(BUTTON_LEFT);
      buttonRight = readButton(BUTTON_RIGHT);
      buttonB     = readButton(BUTTON_B);
      buttonC     = readButton(BUTTON_C);
      break;
  }
}

bool GenesisButtons::buttonsChanged() {
  int previousBits = oldButtonBits;
  oldButtonBits = buttonBits();
  return previousBits != oldButtonBits;
}

int GenesisButtons::buttonBits() {
  return buttonUp | (buttonDown << 1) | (buttonLeft << 2) | (buttonRight << 3) | (buttonStart << 4) | (buttonA << 5) | (buttonB << 6) | (buttonC << 7);
}

void GenesisButtons::print() {
  prints("%d,%d,%d,%d,%d,%d,%d,%d", buttonUp, buttonDown, buttonLeft, buttonRight, buttonStart, buttonA, buttonB, buttonC);
}

void GenesisButtons::prints(const char *string, ...) {
  va_list ArgPtr;
  char temp[512];
  va_start(ArgPtr, string);
  vsprintf(temp, string, ArgPtr);
  va_end(ArgPtr);
  Serial.println (temp);
}
