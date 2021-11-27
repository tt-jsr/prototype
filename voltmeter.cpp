#include "common.h"
#if defined(USE_VOLTMETER_APP)

#include <Arduino.h>
#include "message_queue.h"
#include "oled_128x64.h"

//AnalogRead<4, A0> meter;
AnalogReadInterval<4, A0, 1000000> meter;

uint16_t height;

void appSetup() {
  Serial.begin(9600);
  displaySetup();
  setSmallFont();
  int16_t x, y;
  uint16_t w;
  display.getTextBounds("volt", 0, 0, &x, &y, &w, &height);
  analogReference(INTERNAL4V3);
}

const int32_t microvolts_per_level = (int32_t)(4.3/1024.0 * 1000000) ; // 4.3v/1024 steps - micro volts per level
int counter = 0;

void appLoop() 
{
    if (meter.execute())
    {
        char buf[128];
        int value = meter.getValue();

        uint32_t micros = (uint32_t)value * microvolts_per_level;
        uint32_t volts = micros/1000000;
        micros -= volts*1000000;
        uint32_t millis = micros/1000;
        display.clearDisplay();
        display.setCursor(0, height);
        sprintf(buf, "volts: %d.%d\n", (int)volts, (int)millis);
        display.print(buf);
        //sprintf(buf, "arg2: %d %d", value, ++counter);
        //display.print(buf);
        display.display();
    }
}

#endif  // USE_VOLTMETER_APP
