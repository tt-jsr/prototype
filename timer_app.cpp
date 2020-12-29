#include "common.h"
#if defined(USE_TIMER_APP)

#include "Arduino.h"
#include <SoftwareSerial.h>

#include "display.h"
#include "message_queue.h"

int counter = 0;
MessageQueue message_queue;
int pulsePin = 2;
int TIMER_ID = 0;
int PULSE_ID = 0;

#define MILLISECONDS 1000
void appSetup() {
  message_queue.setDebug(true);
  display_ns::setLargeFont();
  message_queue.create_timer(TIMER_ID, 100, true);
  message_queue.digitalPulse(PULSE_ID, pulsePin, 1000);
}

void appLoop() {
    int msg, arg1, arg2;
    message_queue.get_message(msg, arg1, arg2);
    
    switch(msg)
    {
    case TIMER_EVENT:
        {
            if (arg1 == TIMER_ID)
            {
                message_queue.pulse(PULSE_ID);
            }
        }
        break;
    case VALUE_EVENT:
        break;
    case IDLE_EVENT:
        break;
    }
}
#endif  // USE_TIMER_APP
