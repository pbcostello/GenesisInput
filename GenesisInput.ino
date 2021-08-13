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

struct GenesisButtons {
  int buttonBits = 0;
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
};

#define SELECT_TOGGLE_TIME 20 //milliseconds delay after toggling select pin and reading selected buttons

GenesisButtons genesisButtons;

void readButtons(GenesisButtons &buttons, int buttonGroup) {
  switch (buttonGroup) {
    case 0:
      //read these buttons after setting the Select pin low
      buttons.buttonUp    = digitalRead(BUTTON_UP) ^ 1;
      buttons.buttonDown  = digitalRead(BUTTON_DOWN) ^ 1;
      buttons.buttonA     = digitalRead(BUTTON_A) ^ 1;
      buttons.buttonStart = digitalRead(BUTTON_START) ^ 1;
      break;
    case 1:
      //read these buttons after setting the Select pin high
      buttons.buttonLeft  = digitalRead(BUTTON_LEFT) ^ 1;
      buttons.buttonRight = digitalRead(BUTTON_RIGHT) ^ 1;
      buttons.buttonB     = digitalRead(BUTTON_B) ^ 1;
      buttons.buttonC     = digitalRead(BUTTON_C) ^ 1;
      break;
  }
}

int getButtonBits(GenesisButtons &buttons) {
  return buttons.buttonUp | (buttons.buttonDown << 1) | (buttons.buttonLeft << 2) | (buttons.buttonRight << 3) | (buttons.buttonStart << 4) | (buttons.buttonA << 5) | (buttons.buttonB << 6) | (buttons.buttonC << 7);
}

void getButtonString(GenesisButtons &buttons, char *output, size_t outputSize) {
  snprintf (output, outputSize, "%d,%d,%d,%d,%d,%d,%d,%d", buttons.buttonUp, buttons.buttonDown, buttons.buttonLeft, buttons.buttonRight, buttons.buttonStart, buttons.buttonA, buttons.buttonB, buttons.buttonC);
}

bool buttonsChanged(GenesisButtons &buttons) {
  int previousBits = genesisButtons.oldButtonBits;
  genesisButtons.oldButtonBits = getButtonBits(genesisButtons);
  return previousBits != genesisButtons.oldButtonBits;
}

bool updateGenesisButtons(GenesisButtons &buttons) {
  int tickTime = millis() - buttons.startTime;
  if (tickTime < 0) {
    //counter rolled over
    tickTime = 0 - tickTime;
    buttons.startTime = SELECT_TOGGLE_TIME - tickTime;
  }
  
  if (tickTime > SELECT_TOGGLE_TIME) {
    readButtons(buttons, buttons.currentButtonGroup);
    buttons.currentButtonGroup ^= HIGH;
    digitalWrite(BUTTON_SELECT, buttons.currentButtonGroup);
    buttons.startTime = millis();
  }
  return buttonsChanged(genesisButtons);
}
//////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  pinMode(JOY_PIN_1, INPUT);
  pinMode(JOY_PIN_2, INPUT);
  pinMode(JOY_PIN_3, INPUT);
  pinMode(JOY_PIN_4, INPUT);
  pinMode(JOY_PIN_6, INPUT);
  pinMode(JOY_PIN_7, INPUT);
  pinMode(JOY_PIN_7, OUTPUT);
  pinMode(JOY_PIN_9, INPUT);
  digitalWrite(BUTTON_SELECT, LOW);
  delay(SELECT_TOGGLE_TIME);
  Serial.println("setup done");
}
//////////////////////////////////////////////////////////////////////////
void loop() {

  bool buttonsChanged = updateGenesisButtons(genesisButtons);

  if (buttonsChanged) {
    char output[20];
    getButtonString(genesisButtons, output, sizeof(output));
    Serial.println(output);
  }
}

//////////////////////////////////////////////////////////////////////////
