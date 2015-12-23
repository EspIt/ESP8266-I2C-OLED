/*
Version 1.0 supports OLED display's with either SDD1306 or with SH1106 controller
*/

#include "EspIt_Screen.h"

unsigned char screen[128][8]; //1kB | Screenmap

void set8Pixel(int row, int x, unsigned char data){
  screen[x][row] = data;
  setXY(x, row);
  SendChar(screen[x][row]);
}

inline void verLine(int x, int y1, int y2, bool on){
  if(y1/8 == y2/8){
    if(on){
      set8Pixel(y1/8, x, screen[x][y1/8] | (((0xFF) << (y1 & 7)) & ((0xFF) >> (8-(y2 & 7)))));
    }
    else{
      set8Pixel(y1/8, x, screen[x][y1/8] & ~(((0xFF) << (y1 & 7)) & ((0xFF) >> (8-(y2 & 7)))));
    }
    return;
  }
  
  if(on){
    set8Pixel(y1/8, x, screen[x][y1/8] | (0xFF) << (y1 & 7));
    set8Pixel(y2/8, x, screen[x][y2/8] | (0xFF) >> (8-(y2 & 7)));
  }
  else{
    set8Pixel(y1/8, x, screen[x][y1/8] & ~((0xFF) << (y1 & 7)));
    set8Pixel(y2/8, x, screen[x][y2/8] & ~((0xFF) >> (8-(y2 & 7))));
  }
  for(int i = (y1/8) + 1; i <= (y2/8) - 1; ++i){
    if(on){
      set8Pixel(i, x, screen[x][i] | (0xFF));
    }
    else{
      set8Pixel(i, x, screen[x][i] & ~(0xFF));
    }
  }
}

void setPixel(int x, int y, bool on){
  if(on){
    set8Pixel(y/8, x, screen[x][y/8] | ((unsigned char)true) << (y & 7));
  }
  else{
    set8Pixel(y/8, x, screen[x][y/8] & ~(((unsigned char)true) << (y & 7)));
  }
}

void drawLine(int x1, int y1, int x2, int y2, bool on){
  //Source: Rosetta Code, http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm#C
  int dx = abs(x2-x1), sx = x1<x2 ? 1 : -1;
  int dy = abs(y2-y1), sy = y1<y2 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    setPixel(x1,y1, on);
    if (x1==x2 && y1==y2) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x1 += sx; }
    if (e2 < dy) { err += dx; y1 += sy; }
  }
}

void fillSquare(int left, int top, int width, int height, bool on){
  for(int i = left; i < width + left; ++i){
    verLine(i, top, top + height, on);
  }
}

void drawSquare(int left, int top, int width, int height, bool on){
  verLine(left, top, top + height - 1, on);
  drawLine(left, top, left + width - 1, top, on);
  drawLine(left, top + height - 1, left + width - 1, top + height - 1, on);
  verLine(left + width - 1, top, top + height - 1, on);
}

void drawCircle(int r, int x0, int y0, bool on){
  //Source: Wikipedia, https://en.wikipedia.org/wiki/Midpoint_circle_algorithm#Example
  int x = r;
  int y = 0;
  int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0

  while( y <= x )
  {
    setPixel( x + x0,  y + y0, on); // Octant 1
    setPixel( y + x0,  x + y0, on); // Octant 2
    setPixel(-x + x0,  y + y0, on); // Octant 4
    setPixel(-y + x0,  x + y0, on); // Octant 3
    setPixel(-x + x0, -y + y0, on); // Octant 5
    setPixel(-y + x0, -x + y0, on); // Octant 6
    setPixel( x + x0, -y + y0, on); // Octant 8
    setPixel( y + x0, -x + y0, on); // Octant 7
    y++;
    if (decisionOver2<=0)
    {
      decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
    }
    else
    {
      x--;
      decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
    }
  }
}

void fillCircle(int r, int x0, int y0, bool on){
  //Source: Wikipedia, https://en.wikipedia.org/wiki/Midpoint_circle_algorithm#Example
  int x = r;
  int y = 0;
  int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0

  while( y <= x )
  {
    verLine( x + x0, -y + y0, y + y0, on);
    verLine( y + x0, -x + y0, x + y0, on);
    verLine( -x + x0, -y + y0, y + y0, on);
    verLine( -y + x0, -x + y0, x + y0, on);
    y++;
    if (decisionOver2<=0)
    {
      decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
    }
    else
    {
      x--;
      decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
    }
  }
}

//==========================================================//
// Resets display depending on the actual mode.
 void reset_display(void)
{
  displayOff();
  clear_display();
  displayOn();
}

//==========================================================//
// Turns display on.
 void displayOn(void)
{
  sendcommand(0xaf);        //display on
}

//==========================================================//
// Turns display off.
 void displayOff(void)
{
  sendcommand(0xae);		//display off
}

//==========================================================//
// Clears the display by sendind 0 to all the screen map.
 void clear_display(void)
{
  unsigned char i,k;
  for(k=0;k<8;k++)
  {	
    setRowCol(k,0);    
    {
      for(i=0;i<(128 + 2 * off);i++)     //locate all COL
      {
        screen[i][k] = 0x00; //Clear internal Buffer
        SendChar(0);         //clear all COL
        //delay(10);
      }
    }
  }
}

//==========================================================//
// Actually this sends a byte, not a char to draw in the display. 
// Display's chars uses 8 byte font the small ones and 96 bytes
// for the big number font.
inline void SendChar(unsigned char data) 
{
  //if (interrupt && !doing_menu) return;   // Stop printing only if interrupt is call but not in button functions
  
  Wire.beginTransmission(OLED_address); // begin transmitting
  Wire.write(0x40);//data mode
  Wire.write(data);
  Wire.endTransmission();    // stop transmitting
}

//==========================================================//
// Prints a display char (not just a byte) in coordinates X Y,
// being multiples of 8. This means we have 16 COLS (0-15) 
// and 8 ROWS (0-7).
 void sendCharXY(unsigned char data, int X, int Y)
{
  setRowCol(X, Y);
  Wire.beginTransmission(OLED_address); // begin transmitting
  Wire.write(0x40);//data mode
  
  for(int i=0;i<8;i++){
    unsigned char tmp = pgm_read_byte(myFont[data-0x20]+i);
    Wire.write(tmp);

    screen[X + i][Y] = tmp;
  }
    
  Wire.endTransmission();    // stop transmitting
}

//==========================================================//
// Used to send commands to the display.
inline void sendcommand(unsigned char com)
{
  Wire.beginTransmission(OLED_address);     //begin transmitting
  Wire.write(0x80);                          //command mode
  Wire.write(com);
  Wire.endTransmission();                    // stop transmitting
}

//==========================================================//
// Set the cursor position in a 16 COL * 8 ROW map.
inline void setRowCol(unsigned char row,unsigned char col)
{
  sendcommand(0xb0+row);                //set page address
  sendcommand(off+(8*col&0x0f));       //set low col address
  sendcommand(0x10+((8*col>>4)&0x0f));  //set high col address
}

inline void setXY(unsigned char x, unsigned char y){
  sendcommand(0xb0+(y));
  sendcommand(off+(x&0x0f));     
  sendcommand(0x10+((x>>4)&0x0f));
}


//==========================================================//
// Prints a string in coordinates X Y, being multiples of 8.
// This means we have 16 COLS (0-15) and 8 ROWS (0-7).
 void sendStrXY(const char *string, int X, int Y)
{
  setRowCol(Y,X);
  unsigned char i=0;
  while(*string)
  {
    for(i=0;i<8;i++)
    {
      unsigned char tmp = pgm_read_byte(myFont[*string-0x20]+i);

      screen[X + i][Y] = tmp;
      SendChar(tmp);
    }
    *string++;
  }
}


//==========================================================//
// Inits oled and draws logo at startup
 void init_OLED(void)
{
	pinMode(15, OUTPUT);
	digitalWrite(15, HIGH); //Switch from Bootpins to I2C-Screen
	Wire.begin(0,2);
  sendcommand(0xae);		//display off
  sendcommand(0xa6);            //Set Normal Display (default)
    // Adafruit Init sequence for 128x64 OLED module
    sendcommand(0xAE);             //DISPLAYOFF
    sendcommand(0xD5);            //SETDISPLAYCLOCKDIV
    sendcommand(0x80);            // the suggested ratio 0x80
    sendcommand(0xA8);            //SSD1306_SETMULTIPLEX
    sendcommand(0x3F);
    sendcommand(0xD3);            //SETDISPLAYOFFSET
    sendcommand(0x0);             //no offset
    sendcommand(0x40 | 0x0);      //SETSTARTLINE
    sendcommand(0x8D);            //CHARGEPUMP
    sendcommand(0x14);
    sendcommand(0x20);             //MEMORYMODE
    sendcommand(0x00);             //0x0 act like ks0108
    
    //sendcommand(0xA0 | 0x1);      //SEGREMAP   //Rotate screen 180 deg
    sendcommand(0xA0);
    
    //sendcommand(0xC8);            //COMSCANDEC  Rotate screen 180 Deg
    sendcommand(0xC0);
    
    sendcommand(0xDA);            //0xDA
    sendcommand(0x12);           //COMSCANDEC
    sendcommand(0x81);           //SETCONTRAS
    sendcommand(0xCF);           //
    sendcommand(0xd9);          //SETPRECHARGE 
    sendcommand(0xF1); 
    sendcommand(0xDB);        //SETVCOMDETECT                
    sendcommand(0x40);
    sendcommand(0xA4);        //DISPLAYALLON_RESUME        
    sendcommand(0xA6);        //NORMALDISPLAY             

  clear_display();
  sendcommand(0x2e);            // stop scroll
  //----------------------------REVERSE comments----------------------------//
    sendcommand(0xa0);		//seg re-map 0->127(default)
    sendcommand(0xa1);		//seg re-map 127->0
    sendcommand(0xc8);
    delay(1000);
  //----------------------------REVERSE comments----------------------------//
  // sendcommand(0xa7);  //Set Inverse Display  
  // sendcommand(0xae);		//display off
  sendcommand(0x20);            //Set Memory Addressing Mode
  sendcommand(0x00);            //Set Memory Addressing Mode ab horizontal addressing mode
  //  sendcommand(0x02);         // Set Memory Addressing Mode ab Page addressing mode(RESET)  
}

