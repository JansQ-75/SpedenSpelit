#include "display.h"


void initializeDisplay(void)
{
const int DS_pin = 10; // Arduino pin 13 = input pin in first shift register
const int STCP_pin = 9; // Arduino pin 12 = latch pin in first shift register
const int SHCP_pin = 8; // Arduino pin 11 = clock pin in first shift register

const int DS_pin2 = 13; // Arduino pin 10 = input pin in second shift register
const int STCP_pin2 = 12; // Arduino pin 9 = latch pin in second shift register
const int SHCP_pin2 = 11; // Arduino pin 8 = clock pin in second shift register

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
  uint8_t dec_digits [10] {254, 48, 109, 121, 51, 91, 95, 112, 127, 123}; // decimals indicating numbers 0-9
 
// writes ones when bool last is false
  if(last == false) {
    digitalWrite(STCP_pin,LOW);
    shiftOut(DS_pin, SHCP_pin, LSBFIRST,dec_digits[number]);
    digitalWrite(STCP_pin, HIGH);
  }
  // writes tens when bool last is true
  else {
    digitalWrite(STCP_pin2,LOW);
    shiftOut(DS_pin2, SHCP_pin2, LSBFIRST,dec_digits[number]);
    digitalWrite(STCP_pin2, HIGH);
  }
}

void writeHighAndLowNumber(uint8_t tens,uint8_t ones)
{
  writeByte(ones, false); // calls writeByte subroutine to write ones on first shift register
  writeByte(tens, true); // calls writeByte subroutine to write tens on second shift register
}

void showResult(byte result)
{
  uint8_t tens = result / 10; // divides by 10 to get the tens out of game result
  uint8_t ones = result % 10; // gets remainder after dividing by 10 to get the ones out of the game result

  // writes the final result on 7-segment display by calling subroutine writeHighAndLowNumber

  writeHighAndLowNumber(tens, ones);
}


