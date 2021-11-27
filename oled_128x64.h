#ifndef OLED_128X64_H_
#define OLED_128X64_H_

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Print.h>

#define PRINT1(a1, a2) Serial.print(a1);Serial.println(a2)
#define PRINT2(a1, a2, a3) Serial.print(a1);Serial.print(a2);Serial.println(a3)

static const int SCREEN_WIDTH = 128;
static const int SCREEN_HEIGHT = 64;
static const int OLED_RESET = 4;

// Set the current font to small
void setSmallFont();

// Set the current font to large
void setLargeFont();

void displaySetup();

extern Adafruit_SSD1306 display;

#endif //OLED_128X64_H_
