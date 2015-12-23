#include "EspIt_Screen.h"

void setup() {
  init_OLED();
  reset_display();

  //Draw a Diagonal Line over the Screen
  drawLine(0, 63, 127, 16, true);

  //Draw a Circle in the top Left Corner
  drawCircle(10, 11, 27, true);

  //Draw a filled Circle in the bottom left corner
  fillCircle(15, 111, 47, true);

  //Remove a Circle from the just drawn Cricle
  fillCircle(8, 102, 44, false);

  //Draw a Filled Square in the Info Bar
  fillSquare(0,0, 63, 12, true);

  //Remove a Square bit from the filled Circle
  fillSquare(118, 48, 4, 4, false);

  //Draw a Square in the Info Bar
  drawSquare(65,0, 63, 12, true);

  //Remove a non-filled Square from the Infobar-Square
  drawSquare(10, 2, 10, 5, false);

  //Remove a Pixel from the Square
  setPixel(55, 8, false);

  //Remove a line from the Square
  drawLine(25, 9, 53, 3, false);
}

void loop() {
  //NOP
}
