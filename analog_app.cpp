#include "common.h"
#if defined(USE_ANALOG_APP)

#include "Arduino.h"
#include <SoftwareSerial.h>

#include "display.h"
#include "message_queue.h"

MessageQueue message_queue(8, 1, 1, 1, 1);

// pin reads
static const int SIG_GEN_INPUT_ID = 0;
static const int sigGenInputPin = A0;

void appSetup() {
  message_queue.setDebug(false);
  display_ns::setSmallFont();
  analogReference(INTERNAL4V3);
  message_queue.analogRead(SIG_GEN_INPUT_ID, sigGenInputPin, 0);
}

static const int line1_flags = display_ns::FLAG_CLEAR | display_ns::FLAG_LINES;
static const int line2_flags = display_ns::FLAG_DISPLAY | display_ns::FLAG_LINES;

static const int WINDOW = 10;
struct MovingAverage
{
    MovingAverage()
    :head(0)
    ,tail(1)
    {
        memset(buf, 0, sizeof(buf));
    }
    void add(int n)
    {
        buf[head] = n;
        inc();
    }
    
    int avg()
    {
        int total = 0;
        uint8_t t = tail;
        while (t != head)
        {
            total += buf[t];
            if (++t == WINDOW)
                t = 0;
        }
        total += buf[head];
        return total/WINDOW;
    }

    void inc()
    {
        if (++head == WINDOW)
            head = 0;
        if (++tail == WINDOW)
            tail = 0;
    }

    int buf[WINDOW];
    uint8_t head;
    uint8_t tail;
};

MovingAverage moveavg;

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
            //if (arg2 < (lastRead-epsilon))
                //lastRead = arg2;
            //if (arg2 > (lastRead+epsilon))
                //lastRead = arg2;
                //
            moveavg.add(arg2);
            int avg = moveavg.avg();
            display_ns::print(line1_flags, 0, 0, "Input: ");
            display_ns::display.print(arg2);
            display_ns::print(display_ns::FLAG_LINES, 0, 2, "Avg: ");
            display_ns::print(line2_flags, -1, -1, avg);
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
