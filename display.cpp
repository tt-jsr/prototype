#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include "display.h"

namespace display_ns
{
    bool debug = false;
    int TEXT_HEIGHT = 0;

    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    void setLargeFont()
    {
      int x, y, w, h;

      display.setFont(&FreeSans18pt7b);
      display.getTextBounds("HELLO", 0, 0, &x, &y, &w, &h);
      TEXT_HEIGHT = h;
    }

    void setSmallFont()
    {
      int x, y, w, h;
      display.setFont(&FreeSans9pt7b);
      display.getTextBounds("HELLO", 0, 0, &x, &y, &w, &h);
      TEXT_HEIGHT = h;
    }

    void print(int flags, int x, int y, char *text)
    {
        if (flags & FLAG_CLEAR)
            display.clearDisplay();
        if (flags & FLAG_SMALL_FONT)
            setSmallFont();
        if (flags & FLAG_LARGE_FONT)
            setLargeFont();
        if (flags & FLAG_LINES)
            display.setCursor(x, (y*TEXT_HEIGHT)+TEXT_HEIGHT);
        else
            display.setCursor(x, y);
        display.print(text);
        if (flags & FLAG_DISPLAY)
            display_ns::display.display();
    }

    void print(int flags, char *text)
    {
        int x = display.getCursorX();
        int y = display.getCursorY();
        print (flags, x, y, text);
    }

    void clearDisplay()
    {
        display.clearDisplay();
        display.setCursor(0, TEXT_HEIGHT);
    }

    void setup(bool dbg) {
      debug = dbg;

      // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
      if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
      }

      display.clearDisplay();

      display.setTextSize(1);      // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE); // Draw white text
      display.setRotation(0);
      display.setCursor(0, 0);     // Start at top-left corner
      display.cp437(true);

      setLargeFont();

      setSmallFont();
    }
}
