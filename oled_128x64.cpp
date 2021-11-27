#include "oled_128x64.h"
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans18pt7b.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int currentFontHeight_(0);

void setLargeFont()
{
  int x, y, w, h;

  display.setFont(&FreeSans18pt7b);
  display.getTextBounds("HELLO", 0, 0, &x, &y, &w, &h);
  currentFontHeight_ = h;
}

void setSmallFont()
{
  int x, y, w, h;
  display.setFont(&FreeSans9pt7b);
  display.getTextBounds("HELLO", 0, 0, &x, &y, &w, &h);
  currentFontHeight_ = h;
}

void displaySetup() 
{

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setRotation(0);
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);

  setLargeFont();

  display.clearDisplay();
  display.display();
}
