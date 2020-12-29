#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <Adafruit_SSD1306.h>
#include <Print.h>

#define PRINT1(a1, a2) Serial.print(a1);Serial.println(a2)
#define PRINT2(a1, a2, a3) Serial.print(a1);Serial.print(a2);Serial.println(a3)

namespace display_ns
{
    // Display settings
    static const int SCREEN_WIDTH = 128;
    static const int SCREEN_HEIGHT = 64;
    static const int OLED_RESET = 4;
    extern Adafruit_SSD1306 display;
    extern int TEXT_HEIGHT; // Height of current font

    static const int FLAG_CLEAR = 0x01;         // Clear the display
    static const int FLAG_SMALL_FONT = 0x02;    // Use small font
    static const int FLAG_LARGE_FONT = 0x04;    // use large font
    static const int FLAG_DISPLAY = 0x08;       // Write to screen
    static const int FLAG_LINES = 0x10;         // y-coordinate is in lines

    // Write a message to the screen
    // if x or y < 0, use the current position
    void print(int flags, int x, int y, char *text);
    void print(int flags, int x, int y, int);
    void print(int flags, int x, int y, unsigned long);

    // Clesar the display
    void clearDisplay();

    // Set the current font to small
    void setSmallFont();

    // Set the current font to large
    void setLargeFont();

    void setup(bool debug);

}

#endif DISPLAY_H_

