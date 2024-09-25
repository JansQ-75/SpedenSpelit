#include "display.h"


void initializeDisplay(void)
{
int DS_pin = 13; // Arduino pin 13 = input pin in first shift register
int STCP_pin = 12; // Arduino pin 12 = latch pin in first shift register
int SHCP_pin = 11; // Arduino pin 11 = clock pin in first shift register

int DS_pin2 = 10; // Arduino pin 10 = input pin in second shift register
int STCP_pin2 = 9; // Arduino pin 9 = latch pin in second shift register
int SHCP_pin2 = 8; // Arduino pin 8 = clock pin in second shift register

// set shift registers pins to output
pinMode(DS_pin, OUTPUT);   
pinMode(STCP_pin, OUTPUT);
pinMode(SHCP_pin, OUTPUT);
  
pinMode(DS_pin2, OUTPUT);   
pinMode(STCP_pin2, OUTPUT);
pinMode(SHCP_pin2, OUTPUT);
}

void writeByte(uint8_t number,bool last)

{
  bool last;  // boolean to control writing ones and tens
  int buttonPressed = 2; // number of correct buttons pressed (won't be set as 2 in finished code!!)
  uint8_t number = buttonPressed; // 

// writes ones when bool last is false
  if(number>=0 && number<10 && last == false) {
    digitalWrite(STCP_pin,LOW);
    shiftOut(DS_pin, SHCP_pin, LSBFIRST,dec_digits[number]);
    digitalWrite(STCP_pin, HIGH);
  }
  // writes tens when bool last is true
  if (number>=0 && number<10 && last == true) {
    digitalWrite(STCP_pin2,LOW);
    shiftOut(DS_pin2, SHCP_pin2, LSBFIRST,dec_digits[number]);
    digitalWrite(STCP_pin2, HIGH);
  }
}

void writeHighAndLowNumber(uint8_t tens,uint8_t ones)
{
// See requirements for this function from display.h
}

void showResult(byte number)
{
// See requirements for this function from display.h
}


