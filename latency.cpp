#include "common.h"
#if defined(USE_LATENCY_APP)

#include <Arduino.h>
#include "message_queue.h"
#include "oled_128x64.h"

MessageQueue mq(4);

DigitalRead<0, 14> readpin(HIGH);
Pulse<15, 500000*1000000, LOW> pulse;


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

int counter = 0;

using callback = void(unsigned long);

void OnReadPin(unsigned long state)
{
    pulse.trigger();
}

callback * dispatch[] = {OnReadPin};


void appLoop() 
{
    int msg, arg1;
    unsigned long arg2;

    readpin.execute(&mq);
    if (pulse.execute())
    {
        char buf[32];
        --counter;
        display.clearDisplay();
        display.setCursor(0, height);
        sprintf (buf, "counter: %d", counter);
        display.print(buf);
        display.display();
    }

    mq.get_message(msg, arg1, arg2);

    switch(msg)
    {
    case TIMER_EVENT:
    case DIGITAL_READ_EVENT:
    case ANALOG_READ_EVENT:
    case STATE_EVENT:
        (*dispatch[arg1])(arg2);
        break;
    case IDLE_EVENT:
        break;
    }
}

#endif  // USE_LATENCY_APP
