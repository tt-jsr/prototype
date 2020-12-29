#include "common.h"
#if defined(USE_IDLE_APP)

#include "Arduino.h"
//#include "avr/pgmspace.h"
//#include "spi.h"
#include <SoftwareSerial.h>

#include "display.h"
#include "message_queue.h"

int counter = 0;
MessageQueue message_queue;

void appSetup() {
  display_ns::setLargeFont();
}

void appLoop() {
    int msg, arg1, arg2;
    message_queue.get_message(msg, arg1, arg2);
    
    switch(msg)
    {
    case TIMER_EVENT:
        break;
    case VALUE_EVENT:
        break;
    case IDLE_EVENT:
        {
            display_ns::display.setCursor(0, display_ns::TEXT_HEIGHT);
            display_ns::display.print("Idle: ");
            display_ns::display.setCursor(0, 2*display_ns::TEXT_HEIGHT);
            display_ns::display.print(counter);
            display_ns::display.display();
            ++counter;
            display_ns::display.clearDisplay();
        }
        break;
    }
}

#endif  // USE_IDLE_APP
