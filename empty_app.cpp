#include "common.h"
#if defined(USE_EMPTY_APP)

#include <Arduino.h>
#include "message_queue.h"
#include "oled_128x64.h"

MessageQueue message_queue(8);
PulseRepeat<1, 2, 1000, 1000> pulse;
Timer<2, 5000, false> timerStart;
Timer<3, 15000, false> timerEnd(false);
AnalogRead<4, A0> xaxis;
AnalogRead<5, A1> yaxis;

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
    int x;

    pulse.execute();
    timerStart.execute(&message_queue);
    timerEnd.execute(&message_queue);
    message_queue.get_message(msg, arg1, arg2);
    xaxis.execute();
    yaxis.execute();
    
    switch(msg)
    {
    case TIMER_EVENT:
        if (arg1 == timerStart.id)
        {
            //display.setCursor(0, height);
            //display.println("timer start");
            //display.display();
            pulse.run(HIGH);
            timerEnd.reset();
        }
        if (arg1 == timerEnd.id)
        {
            pulse.stop(LOW);
            //display.print("timer end");
            //display.display();
        }
        break;
    case STATE_EVENT:
        break;
    case DIGITAL_READ_EVENT:
        break;
    case ANALOG_READ_EVENT:
        break;
    case IDLE_EVENT:
        {
            int x = xaxis.getValue();
            int y = yaxis.getValue();
            //Serial.print("x:");Serial.print(x);Serial.print(" y:");Serial.println(y);
            display.drawPixel(x/16, y/16, WHITE);
            display.display();
        }
        break;
    }
    
}

#endif  // USE_EMPTY_APP
