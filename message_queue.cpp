#include "Arduino.h"
#include "message_queue.h"

MessageQueue::MessageQueue()
:consumer_(0)
,producer_(0)
, debug_(false)
{
    for (int i = 0; i < MAX_MESSAGES;++i)
    {
        msg_queue_[i].msg_type = 0;
        msg_queue_[i].arg1 = 0;
        msg_queue_[i].arg2 = 0;
    }

    for (int i = 0; i < MAX_TIMERS;++i)
    {
        timers_[i].interval = 0;
        timers_[i].repeat = false;
        timers_[i].nextTrigger = 0;
    }

    for (int i = 0; i < MAX_PINS; ++i)
    {
        pins_[i].flags = 0;
        pins_[i].debounceTime = 0;
        pins_[i].triggerComplete = 0;
        pins_[i].debounceValue = 0;
    }
    for (int i = 0; i < MAX_PULSES; ++i)
    {
        pulses_[i].pin = 0;
        pulses_[i].duration = 0;
        pulses_[i].pulseComplete = 0;
    }
    for (int i = 0; i < MAX_VALUES; ++i)
    {
        values_[i].id = 0;
        values_[i].value = 0;
    }
}

bool MessageQueue::empty()
{
    return consumer_ == producer_;
}

void MessageQueue::get_message(int& msg, int& arg1, int& arg2)
{
    if (consume_message(msg, arg1, arg2))
        return;

    OnGenerator();
    check_timers();
    check_pins();
    msg = IDLE_EVENT;
    arg1 = 0;
    arg2 = 0;
}

void MessageQueue::post_message(int msg, int arg1, int arg2)
{
    msg_queue_[producer_].msg_type = msg;
    msg_queue_[producer_].arg1 = arg1;
    msg_queue_[producer_].arg2 = arg2;
    producer_ = (++producer_) & (MAX_MESSAGES-1);
    if (producer_ == consumer_)
    {
        // If we just posted and reached the consumer_, our queue is full
        // so advance the consumer and drop the message
        ++consumer_;
        if (debug_)
            Serial.println("MessageQueue: Queue full, message dropped");
    }
}

bool MessageQueue::consume_message(int& msg, int& arg1, int& arg2)
{
    if (producer_ == consumer_)
    {
        msg_queue_[consumer_].msg_type = 0;
        msg_queue_[consumer_].arg1 = 0;
        msg_queue_[consumer_].arg2 = 0;
        return false;
    }

    msg = msg_queue_[consumer_].msg_type;
    arg1 = msg_queue_[consumer_].arg1;
    arg2 = msg_queue_[consumer_].arg2;
    consumer_ = (++consumer_) & (MAX_MESSAGES-1);
    return true;
}

bool MessageQueue::peek_message(int& msg, int& arg1, int& arg2)
{
    if (producer_ == consumer_)
    {
        msg_queue_[consumer_].msg_type = 0;
        msg_queue_[consumer_].arg1 = 0;
        msg_queue_[consumer_].arg2 = 0;
        return false;
    }

    msg = msg_queue_[consumer_].msg_type;
    arg1 = msg_queue_[consumer_].arg1;
    arg2 = msg_queue_[consumer_].arg2;
    return true;
}

bool MessageQueue::create_timer(int id, unsigned long interval, bool repeat)
{
    if (id < 0 || id >= MAX_TIMERS)
    {
        if (debug_)
        {
            Serial.print("MessageQueue: create_timer() failed, id=");
            Serial.println(id);
        }
        return false;
    }
    timers_[id].interval = interval;
    timers_[id].repeat = repeat;
    timers_[id].nextTrigger = millis() + interval;
    if (debug_)
    {
        Serial.print("MessageQueue: create_timer(), id=");
        Serial.println(id);
    }
    return true;
}

void MessageQueue::cancel_timer(int id)
{
    if (id < 0 || id >= MAX_TIMERS)
    {
        if (debug_)
        {
            Serial.print("MessageQueue: cancel_timer() failed, id=");
            Serial.println(id);
        }
        return false;
    }

    timers_[id].interval = 0;
    timers_[id].repeat = false;
    timers_[id].nextTrigger = 0;
    if (debug_)
    {
        Serial.print("MessageQueue: cancel_timer(), id=");
        Serial.println(id);
    }
    return true;
}

void MessageQueue::reset_timer(int id, unsigned long interval, bool repeat)
{
    if (id < 0 || id >= MAX_TIMERS)
    {
        if (debug_)
        {
            Serial.print("MessageQueue: reset_timer() failed, id=");
            Serial.println(id);
        }
        return false;
    }

    timers_[id].interval = interval;
    timers_[id].repeat = repeat;
    timers_[id].nextTrigger = millis() + timers_[id].interval;
    if (debug_)
    {
        Serial.print("MessageQueue: reset_timer(id, interval, repeat), id=");
        Serial.print(id);
        Serial.print(" interval=");
        Serial.print(interval);
        Serial.print(" repeat=");
        Serial.println(repeat);
    }
    return true;
}

void MessageQueue::reset_timer(int id)
{
    if (id < 0 || id >= MAX_TIMERS)
    {
        if (debug_)
        {
            Serial.print("MessageQueue: reset_timer() failed, id=");
            Serial.println(id);
        }
        return false;
    }
    timers_[id].nextTrigger = millis() + timers_[id].interval;
    if (debug_)
    {
        Serial.print("MessageQueue: reset_timer(), id=");
        Serial.println(id);
    }
    return true;
}

void MessageQueue::pause_timer(int id)
{
    if (id < 0 || id >= MAX_TIMERS)
    {
        if (debug_)
        {
            Serial.print("MessageQueue: pause_timer() failed, id=");
            Serial.println(id);
        }
        return false;
    }
    timers_[id].nextTrigger = 0;
    if (debug_)
    {
        Serial.print("MessageQueue: pause_timer(), id=");
        Serial.println(id);
    }
    return true;
}

void MessageQueue::check_timers()
{
    unsigned long now = millis();
    for (int id = 0; id < MAX_TIMERS; ++id)
    {
        if (timers_[id].nextTrigger && timers_[id].nextTrigger <= now)
        {
            post_message(TIMER_EVENT, id, now-timers_[id].nextTrigger);
            if (timers_[id].repeat)
                timers_[id].nextTrigger = now + timers_[id].interval;
            else
                timers_[id].nextTrigger = 0;
        }
    }
}

bool MessageQueue::digitalPulse(int id, int pin, unsigned long duration_us)
{
    if (id < 0 || id >= MAX_PULSES)
    {
        if (debug_)
        {
            Serial.print("MessageQueue: digitalPulse() failed, id=");
            Serial.println(id);
        }
        return false;
    }
    pulses_[id].pin = pin;
    pinMode(pin, OUTPUT);
    pulses_[id].duration = duration_us;
    if (debug_)
    {
        Serial.print("MessageQueue: digitalPulse(), id=");
        Serial.println(id);
    }
    return true;
}

bool MessageQueue::pulse(int id)
{
    if (id < 0 || id >= MAX_PULSES)
    {
        if (debug_)
        {
            Serial.print("MessageQueue: pulse() id out of range, id=");
            Serial.println(id);
        }
        return false;
    }
    digitalWrite(pulses_[id].pin, HIGH);
    pulses_[id].pulseComplete = micros() + pulses_[id].duration;
    return true;
}

bool MessageQueue::digitalRead(int id, int pin, int def, unsigned long debounceTimeMicros)
{
    if (id < 0 || id >= MAX_PINS)
    {
        if (debug_)
        {
            Serial.print("MessageQueue: digitalRead() id out of range, id=");
            Serial.println(id);
        }
        return false;
    }
    PIN_SET_PIN(pins_[id], pin);
    PIN_SET_DIGITAL(pins_[id]);
    pins_[id].debounceTime = debounceTimeMicros;
    pins_[id].triggerComplete = 0;
    if (debug_)
    {
        Serial.print("MessageQueue: digitalRead(), id=");
        Serial.println(id);
    }
    create_value(id, def);
    return true;
}

bool MessageQueue::analogRead(int id, int pin, unsigned long timeMicros)
{
    if (id < 0 || id >= MAX_PINS)
    {
        if (debug_)
        {
            Serial.print("MessageQueue: analogRead() id out of range, id=");
            Serial.println(id);
        }
        return false;
    }
    PIN_SET_PIN(pins_[id], pin);
    PIN_UNSET_DIGITAL(pins_[id]);
    pins_[id].debounceTime = timeMicros;
    pins_[id].triggerComplete = 0;
    if (debug_)
    {
        Serial.print("MessageQueue: analogRead(), id=");
        Serial.println(id);
    }
    create_value(id, 0);
    return true;

}

void MessageQueue::check_pins()
{
    unsigned long now = micros();
    for (int id = 0; id < MAX_PULSES; id++)
    {
        if (pulses_[id].pulseComplete && pulses_[id].pulseComplete < now)
        {
            if (debug_)
                Serial.println("pulse complete");
            digitalWrite(pulses_[id].pin, LOW);
            pulses_[id].pulseComplete = 0;
        }
    }
    for (int id = 0; id < MAX_PINS; id++)
    {
        if (PIN_IS_DIGITAL(pins_[id]))
            digital_check(id);
        else
            analog_check(id);
    }
}

void MessageQueue::digital_check(int id)
{
    Pin& pin = pins_[id];
    int s = ::digitalRead(PIN_PIN(pin));
    if (pin.debounceTime == 0)
    {
        set_value(id, s);
    }
    else if(pin.triggerComplete == 0)
    {
        pin.debounceValue = s;
        pin.triggerComplete = micros() + pin.debounceTime;
    }
    else if (pin.triggerComplete < micros())
    {
        pin.triggerComplete = 0;
        if (pin.debounceValue == s)
        {
            set_value(id, s);
        }
    }
}

void MessageQueue::analog_check(int id)
{
    Pin& pin = pins_[id];
    if (pin.debounceTime == 0)
    {
        int v = ::analogRead(PIN_PIN(pin));

        set_value(id, v);
    }
    if (pin.triggerComplete == 0)
    {
        pin.triggerComplete = micros() + pin.debounceTime;
    }

    if (pin.triggerComplete < micros())
    {
        int v = ::analogRead(PIN_PIN(pin));
        set_value(id, v);
        pin.triggerComplete = micros() + pin.debounceTime;
    }
}

bool MessageQueue::create_value(int id, int v)
{
    if (id < 0 || id >= MAX_VALUES)
    {
        if (debug_)
        {
            Serial.print("MessageQueue: create_value() id out of range, id=");
            Serial.println(id);
        }
        return false;
    }
    values_[id].value = v;
    if (debug_)
    {
        Serial.print("MessageQueue: create_value() success, id=");
        Serial.println(id);
    }
    return true;
}

void MessageQueue::set_value(int id, int v)
{
    if (id < 0 || id >= MAX_VALUES)
    {
        if (debug_)
        {
            Serial.print("MessageQueue: set_value() id out of range, id=");
            Serial.println(id);
        }
        return false;
    }
    if (values_[id].value != v)
    {
        values_[id].value = v;
        post_message(VALUE_EVENT, id, v);
    }
    return;
}

int MessageQueue::get_value(int id)
{
    if (id < 0 || id >= MAX_VALUES)
    {
        if (debug_)
        {
            Serial.print("MessageQueue: get_value() id out of range, id=");
            Serial.println(id);
        }
        return -1;
    }
    return values_[id].value;
}

void MessageQueue::toggle_value(int id)
{
    if (id < 0 || id >= MAX_VALUES)
    {
        if (debug_)
        {
            Serial.print("MessageQueue: toggle_value() id out of range, id=");
            Serial.println(id);
        }
        return;
    }
    values_[id].value = !values_[id].value;
    post_message(VALUE_EVENT, id, values_[id].value);
}

void MessageQueue::OnGenerator()
{
}

void MessageQueue::setDebug(bool dbg)
{
    debug_ = dbg;
}

