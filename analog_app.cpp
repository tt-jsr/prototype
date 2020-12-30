#include "common.h"
#if defined(USE_ANALOG_APP)

#include "Arduino.h"
#include <SoftwareSerial.h>

#include "display.h"
#include "message_queue.h"

MessageQueue message_queue;

// pin reads
static const int SIG_GEN_INPUT_ID = 0;
static const int sigGenInputPin = A0;

void appSetup() {
  message_queue.setDebug(true);
  display_ns::setLargeFont();
  analogReference(INTERNAL4V3);
  message_queue.analogRead(SIG_GEN_INPUT_ID, sigGenInputPin, 10000);
}

static const int line1_flags = display_ns::FLAG_CLEAR | display_ns::FLAG_LINES;
static const int line2_flags = display_ns::FLAG_DISPLAY | display_ns::FLAG_LINES;

void appLoop() {
    int msg, arg1, arg2;
    message_queue.get_message(msg, arg1, arg2);
    
    switch(msg)
    {
    case TIMER_EVENT:
        break;
    case VALUE_EVENT:
        if (arg1 == SIG_GEN_INPUT_ID)
        {
            display_ns::print(line1_flags, 0, 0, "Input:");
            display_ns::print(line2_flags, 0, 1, arg2);
        }
        else
        {
            Serial.print("Unknown value event, arg1: ");
            Serial.println(arg1);
        }
        break;
    case IDLE_EVENT:
        break;
    default:
        Serial.print ("Unknown message, msg: ");
        Serial.print (msg);
        Serial.print (" arg1: ");
        Serial.print (arg1);
        Serial.print (" arg2: ");
        Serial.println (arg2);
    }
}
#endif  // USE_ANALOG_APP
