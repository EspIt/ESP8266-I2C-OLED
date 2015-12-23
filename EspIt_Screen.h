/*
Version 1.0 supports OLED display's with either SDD1306 or with SH1106 controller
*/

#include <Wire.h>
#include <Arduino.h>
#include "font.h"
#define off 0x00    // SDD1306                      // offset=0 for SSD1306 controller
#define OLED_address 0x3c                             // all the OLED's I have seen have this address

 void set8Pixel(int, int, unsigned char data);

 void setPixel(int, int, bool);

 void drawLine(int, int, int, int, bool);
 
 void drawCircle(int, int, int, bool);

 void fillSquare(int, int, int, int, bool);
 
 void drawSquare(int, int, int, int, bool);

 void fillCircle(int, int, int, bool);

//==========================================================//
// Resets display depending on the actual mode.
 void reset_display(void);

//==========================================================//
// Turns display on.
 void displayOn(void);

//==========================================================//
// Turns display off.
 void displayOff(void);

//==========================================================//
// Clears the display by sending 0 to all the screen map.
 void clear_display(void);

//==========================================================//
// Prints a display char (not just a byte) in coordinates X Y,
// being multiples of 8. This means we have 16 COLS (0-15) 
// and 8 ROWS (0-7).
 void sendCharXY(unsigned char data, int X, int Y);

//==========================================================//
// Prints a string in coordinates X Y, being multiples of 8.
// This means we have 16 COLS (0-15) and 8 ROWS (0-7).
 void sendStrXY(const char *string, int X, int Y);

//==========================================================//
// Inits oled and draws logo at startup
 void init_OLED(void);

