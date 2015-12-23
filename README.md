# ESP8266-I2C-OLED

This is a Library for use with the integrated I2C Oled Screen. It is based on [hurricaneMitch's fork of costonisp's Webserver with the Screen](https://github.com/hurricaneMitch/ESP8266-I2C-OLED "hurricaneMitch's repo"). Other features are Inspired by [Adafruit's SDD1306 Library](https://github.com/adafruit/Adafruit_SSD1306 "Adafruit's repo").

To use it just `#include "EspIt_Screen.h"` and you are good to go.

### Function table

```c
init_Oled(); //Initialized the Oled-Screen
reset_display(); //Performes a full reset of the Screen
displayOff(); //Switches the Display off
displayOn(); //Switches it Back on
clear_display(); //Switches all pixels off
sendCharXY(unsigned char data, int X, int Y); //Draws the Character "data", 8px in width, 8px in height, at position X (0-15) | Y (0-7)
sendStrXY(const char *string, int X, int Y); //Draws the String "string", 8px in width, 8px in height, at position X (0-15) | Y (0-7). automatically creates a newline if needed.
setPixel(int x, int y, bool on); //Switches pixel at pos (X|Y) to either on or off, based on the boolean.
drawLine(int x1, int y1, int x2, int y2, bool on); // Draws a Line from pos (X1|Y1) to (X2|Y2), either on or off, based on the boolean.
drawCircle(int r, int x0, int y0); // Draws a Circle at Point (X0|Y0) with the Radius r, either on or off, based on the boolean.
fillCircle(int r, int x0, int y0); // Draws a filled Circle at Point (X0|Y0) with the Radius r, either on or off, based on the boolean.

```
