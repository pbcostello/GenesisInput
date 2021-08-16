

#include "GenesisInput.h"

GenesisButtons gb = GenesisButtons();

//////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  gb.setup();
  Serial.println ("setup done");
}
//////////////////////////////////////////////////////////////////////////
void loop() {

  if (gb.updateGenesisButtons()) {
    gb.print();
  }
}

//////////////////////////////////////////////////////////////////////////
