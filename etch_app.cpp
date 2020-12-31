#include "common.h"
#if defined(USE_ETCH_APP)

#include "Arduino.h"
#include <SoftwareSerial.h>

#include "display.h"
#include "message_queue.h"

MessageQueue message_queue(8, 1, 3, 1, 1);

// pin reads
static const int X_POT_ID = 0;
static const int xPotPin = A0;
static const int Y_POT_ID = 1;
static const int yPotPin = A1;
static const int CLEAR_SCREEN_ID = 2;
static const int clearScreenPin = 2;


void appSetup() {
  message_queue.setDebug(true);
  display_ns::setSmallFont();
  analogReference(INTERNAL4V3);
  //message_queue.analogRead(X_POT_ID, xPotPin, 100);
  //message_queue.analogRead(Y_POT_ID, yPotPin, 100);
  message_queue.digitalRead(CLEAR_SCREEN_ID, clearScreenPin, HIGH, 1000);
  pinMode(clearScreenPin, INPUT_PULLUP);
  pinMode(xPotPin, INPUT);
  pinMode(yPotPin, INPUT);
  display_ns::clearDisplay();
  display_ns::display.display();
}

void appLoop() {
    int msg, arg1, arg2;
    message_queue.get_message(msg, arg1, arg2);
    
    switch(msg)
    {
    case TIMER_EVENT:
        break;
    case VALUE_EVENT:
        if (arg1 == CLEAR_SCREEN_ID)
        {
            display_ns::display.clearDisplay();
            display_ns::display.display();
        }
        else
        {
            Serial.print("Unknown value event, arg1: ");
            Serial.println(arg1);
        }
        break;
    case IDLE_EVENT:
        {
            int x = analogRead(xPotPin);
            int y = analogRead(yPotPin);
            x = 128-x/8;
            y = 64-y/16;
            Serial.print("x:");
            Serial.print(x);
            Serial.print(" y:");
            Serial.println(y);
            display_ns::display.drawPixel(x, y, WHITE);
            display_ns::display.display();
        }
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
#endif  // USE_ETCH_APP
