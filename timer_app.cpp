#include "common.h"
#if defined(USE_TIMER_APP)

#include "Arduino.h"
#include <SoftwareSerial.h>

#include "display.h"
#include "message_queue.h"

MessageQueue message_queue;
const int sigGenOutputPin = 7;

// Timers
const int TIMER_ID = 0;
const int ELAPSED_TIME_TIMER_ID = 1;

// Pulses
const int PULSE_ID = 0;
const int pulsePin = 2;

// pin reads
const int SIG_GEN_INPUT_ID = 0;
const int sigGenInputPin = 4;

#define MILLISECONDS 1000
void appSetup() {
  message_queue.setDebug(false);
  display_ns::setLargeFont();
  message_queue.create_timer(TIMER_ID, 100, true);
  message_queue.create_timer(ELAPSED_TIME_TIMER_ID, 1000, true);
  message_queue.digitalPulse(PULSE_ID, pulsePin, 1000);
  message_queue.digitalRead(SIG_GEN_INPUT_ID, sigGenInputPin, LOW, 0);
  pinMode(sigGenOutputPin, OUTPUT);
}

const int line1_flags = display_ns::FLAG_CLEAR | display_ns::FLAG_LINES;
const int line2_flags = display_ns::FLAG_DISPLAY | display_ns::FLAG_LINES;

void appLoop() {
    int msg, arg1, arg2;
    message_queue.get_message(msg, arg1, arg2);
    
    switch(msg)
    {
    case TIMER_EVENT:
        if (arg1 == TIMER_ID)
        {
            message_queue.pulse(PULSE_ID);
        }
        if (arg1 == ELAPSED_TIME_TIMER_ID)
        {
            display_ns::print(line1_flags, 0, 0, "# secs:");
            display_ns::print(line2_flags, 0, 1, micros()/1000000);
        }
        break;
    case VALUE_EVENT:
        if (arg1 == SIG_GEN_INPUT_ID)
        {
            //Serial.println("SIG GEN Input");
            digitalWrite(sigGenOutputPin, arg2);
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
#endif  // USE_TIMER_APP
