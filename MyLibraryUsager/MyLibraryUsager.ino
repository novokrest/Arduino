#include "KeyboardSimulator.h"
#include "Blinker.h"
#include "Shiffer.h"

Blinker blinker(13);
Shiffer shiffer;

KeyboardSimulator keyboardSimulator;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  keyboardSimulator.Run();
}
