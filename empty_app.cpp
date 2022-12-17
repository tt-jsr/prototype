#include "common.h"
#if defined(USE_EMPTY_APP)

#include <Arduino.h>
#include "message_queue.h"
#include "oled_128x64.h"

MessageQueue message_queue(8);
Timer<2, 250, true> timer;
DigitalWrite<13> output;

int counter(0);
int pinState = LOW;

uint16_t height;

void appSetup() {
  Serial.begin(9600);
  displaySetup();
  setSmallFont();
  int16_t x, y;
  uint16_t w;
  display.getTextBounds("timer", 0, 0, &x, &y, &w, &height);
  analogReference(INTERNAL4V3);
}

void appLoop() 
{
    int msg, arg1;
    unsigned long arg2;

    timer.execute(&message_queue);
    message_queue.get_message(msg, arg1, arg2);

    switch(msg)
    {
    case TIMER_EVENT:
        if (arg1 == timer.id)
        {
            char buf[64];
            sprintf(buf, "Count: %d", ++counter);
            display.clearDisplay();
            display.setCursor(0, height);
            display.println(buf);
            display.display();
            pinState = pinState == LOW ? HIGH : LOW;
            output.write(pinState);
        }
        break;
    case STATE_EVENT:
        break;
    case DIGITAL_READ_EVENT:
        break;
    case ANALOG_READ_EVENT:
        break;
    case IDLE_EVENT:
        break;
    }
}

#endif  // USE_EMPTY_APP
