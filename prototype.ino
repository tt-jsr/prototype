#include "Arduino.h"
//#include "avr/pgmspace.h"
//#include "spi.h"
#include <SoftwareSerial.h>

#include "common.h"
#include "display.h"

void setup() {
  Serial.begin(9600);
  display_ns::setup(false);

  appSetup();
}

void loop() {
    appLoop();
}
