/*
Version 1.0 supports OLED display's with either SDD1306 or with SH1106 controller
*/

#include <Wire.h>
#include "font.h"
#define offset 0x00    // SDD1306                      // offset=0 for SSD1306 controller
#define OLED_address  0x3c                             // all the OLED's I have seen have this address

//==========================================================//
// Resets display depending on the actual mode.
public static void reset_display(void);

//==========================================================//
// Turns display on.
public static void displayOn(void);

//==========================================================//
// Turns display off.
public static void displayOff(void);

//==========================================================//
// Clears the display by sending 0 to all the screen map.
public static void clear_display(void);

//==========================================================//
// Actually this sends a byte, not a char to draw in the display. 
// Display's chars uses 8 byte font the small ones and 96 bytes
// for the big number font.
private static void SendChar(unsigned char data);

//==========================================================//
// Prints a display char (not just a byte) in coordinates X Y,
// being multiples of 8. This means we have 16 COLS (0-15) 
// and 8 ROWS (0-7).
public static void sendCharXY(unsigned char data, int X, int Y);

//==========================================================//
// Used to send commands to the display.
private static void sendcommand(unsigned char com);

//==========================================================//
// Set the cursor position in a 16 COL * 8 ROW map.
private static void setXY(unsigned char row,unsigned char col);

//==========================================================//
// Prints a string in coordinates X Y, being multiples of 8.
// This means we have 16 COLS (0-15) and 8 ROWS (0-7).
public static void sendStrXY( char *string, int X, int Y);

//==========================================================//
// Inits oled and draws logo at startup
public static void init_OLED(void);


