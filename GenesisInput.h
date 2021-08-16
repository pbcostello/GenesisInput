#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

class GenesisButtons {
    enum {
      //D9 gamepad connector pin# to Arduino input pin#
      JOY_PIN_1 = 2,
      JOY_PIN_2 = 3,
      JOY_PIN_3 = 4,
      JOY_PIN_4 = 5,
      // JOY_PIN_5 Positive
      JOY_PIN_6 = 6,
      JOY_PIN_7 = 7, //Select
      // JOY_PIN_8 Ground
      JOY_PIN_9 = 8,
    };

    enum {
      BUTTON_UP     = JOY_PIN_1,  //blue
      BUTTON_DOWN   = JOY_PIN_2,  //green
      BUTTON_LEFT   = JOY_PIN_3,  //blue+gold
      BUTTON_RIGHT  = JOY_PIN_4,  //green+gold
      BUTTON_SELECT = JOY_PIN_7,  //yellow
      BUTTON_A      = JOY_PIN_6,  //white
      BUTTON_B      = BUTTON_A,   //+SELECT
      BUTTON_START  = JOY_PIN_9,  //white+gold
      BUTTON_C      = BUTTON_START//+SELECT
    };

    int startTime = millis();
    int buttonUp = 0;
    int buttonDown = 0;
    int buttonA = 0;
    int buttonStart = 0;
    int buttonLeft = 0;
    int buttonRight = 0;
    int buttonB = 0;
    int buttonC = 0;
    int currentButtonGroup = LOW;
    int oldButtonBits = 0;
    const int selectToggleTime = 20;  //milliseconds delay after toggling select pin and reading selected buttons

  public:
    void setup();
    bool updateGenesisButtons();
    void readButtons(int buttonGroup);
    bool buttonsChanged();
    int buttonBits();
    int readButton(int pin);
    void print();
    void prints(const char *string, ...);
};
