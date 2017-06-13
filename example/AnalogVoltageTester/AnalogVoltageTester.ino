#include <SPI.h>
#include <DACx311.h>
#define PIN_CS 10

DACxx11 dacChip;

void setup() {
  // Argument 2 can be either DAC5311, DAC6311 or DAC7311.
  // See class file for options
  dacChip.begin( PIN_CS, DAC7311, 5.0 );
  dacChip.setVoltage(2.3);
}

void loop() {
  
}
