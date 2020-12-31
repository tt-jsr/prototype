#ifndef _COMMON_H_
#define _COMMON_H_

void appSetup();
void appLoop();

//#define USE_IDLE_APP
//#define USE_TIMER_APP
#define USE_ANALOG_APP


#if defined(USE_TIMER_APP)
#   define MAX_TIMERS          2
#   define MAX_PINS            1
#   define NUMBER_OF_VALUES    0
#   define MAX_PULSES          1
#endif 

#if defined(USE_IDLE_APP)
#   define MAX_TIMERS          1
#   define MAX_PINS            1
#   define NUMBER_OF_VALUES    0
#   define MAX_PULSES          1
#endif 

#endif // _COMMON_H_
