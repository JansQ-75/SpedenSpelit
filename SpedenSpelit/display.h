#ifndef DISPLAY_H
#define DISPLAY_H
#include <arduino.h>


/*
  initializeDisplay subroutine initializes 5 pins needed for controlling 7-segment
  displays as outputs:

  Arduino pin 8 = clock pin in first shift register 
  Arduino pin 9 = latch pin in first shift register
  Arduino pin 10 = input pin in both shift registers
  Arduino pin 11  = clock pin in second shift register
  Arduino pin 12 = latch pin in second shift register
*/

void initializeDisplay(void);


/*
  WriteByte subroutine writes number 0,1,...,9 to
  7-segment display. Booleans define when it's time to activate second display or write decimal points in 7-segment displays.
  
  Parameters:
  uint8_t number: accepts values 0,1,..,9
  bool ten: true = writes tens to second display
  bool hundred: true = writes tens with decimalpoint to second display. Decimalpoint indicates number 100 has been reached
  bool twoHundred: true = writes ones with decimalpoint to first display. Decimalpoint indicates number 200 has been reached
*/


void writeByte(uint8_t number, bool last);


/*
  writeHighAndLowNumber subroutine writes a number 0,1,..,255
  to 7-segment displays. This subroutine uses
  WriteByte subroutine to write 2 numbers to the display.
  
  Parameters:
  
  uint8_t tens: number 0,1,..,9
  uint8_t ones: number 0,1,..,9
  bool hundred: true = number is 100-199
  bool twoHundred: true = number is 200 or more
  
*/
void writeHighAndLowNumber(uint8_t tens,uint8_t ones);


/*
  showResuts(byte result) This function separates tens and ones
  from a result number and calls writeHighAndLowNumber subroutine
  to actually write the number to 7-segment display.
  
  Parameters:
  byte result: A number between 0,1,..,255
*/
void showResult(byte result);

/*
  textGameOver() function is called when player loses the game.
  Function writes text "Loser...see ya" to 7-segment display. 
  Text is shown letter by letter.
*/
void textGameOver();

#endif
