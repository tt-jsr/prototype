#include "common.h"
#if defined(USE_WAVEFORM_APP)

#include "Arduino.h"
#include <SoftwareSerial.h>

#include "display.h"
#include "message_queue.h"

MessageQueue message_queue(8, 1, 1, 1, 1);

// pin reads
static const int SIG_GEN_INPUT_ID = 0;  // 10K pot
static const int sigGenInputPin = A0;


void appSetup() {
  message_queue.setDebug(true);
  display_ns::setSmallFont();
  analogReference(INTERNAL4V3);
  message_queue.analogRead(SIG_GEN_INPUT_ID, sigGenInputPin, 109375);
  display_ns::clearDisplay();
  display_ns::display.display();
}

int x = 0;

void appLoop() {
    int msg, arg1;
    unsigned long arg2;
    message_queue.get_message(msg, arg1, arg2);
    
    switch(msg)
    {
    case TIMER_EVENT:
        break;
    case VALUE_EVENT:
        if (arg1 == SIG_GEN_INPUT_ID)
        {
            int y = 64-arg2/16;
            display_ns::display.drawPixel(x, y, WHITE);
            ++x;
            if (x == 128)
            {
                x = 0;
                display_ns::display.clearDisplay();
            }
            display_ns::display.display();
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
#endif  // USE_WAVEFORM_APP
