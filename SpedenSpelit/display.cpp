#include "display.h"
const int DS_pin = 10; // Arduino pin 13 = input pin in first shift register
const int STCP_pin = 9; // Arduino pin 12 = latch pin in first shift register
const int SHCP_pin = 8; // Arduino pin 11 = clock pin in first shift register
const int STCP_pin2 = 12; // Arduino pin 9 = latch pin in second shift register
const int SHCP_pin2 = 11; // Arduino pin 8 = clock pin in second shift register
const int DS_pin2 = 13;
void initializeDisplay(void)
{


// set shift registers pins to output
pinMode(DS_pin, OUTPUT);   
pinMode(STCP_pin, OUTPUT);
pinMode(SHCP_pin, OUTPUT);
  
pinMode(DS_pin2, OUTPUT);   
pinMode(STCP_pin2, OUTPUT);
pinMode(SHCP_pin2, OUTPUT);
}

void writeByte(uint8_t number,bool ten, bool hundred, bool twoHundred)

{
  uint8_t dec_digits [10] {126, 48, 109, 121, 51, 91, 95, 112, 127, 123}; // decimals indicating numbers 0-9
  uint8_t dec_digitsHundred [10] {254, 176, 237, 249, 179, 219, 223, 240, 255, 251}; // decimals indicating numbers 0-9 with decimalpoint 
  uint8_t blank = 0; // decimal to write 7-segment display blank
 

  
  if (twoHundred == false) // following code is run if result is less than 200
  {
  
  	if (ten == false && hundred == false && twoHundred == false) {

      // writes ones to first 7-segment    
    	digitalWrite(STCP_pin,LOW);
    	shiftOut(DS_pin, SHCP_pin, LSBFIRST,dec_digits[number]);
    	digitalWrite(STCP_pin, HIGH);

      // writes second 7-segment display blank
      digitalWrite(STCP_pin2,LOW);
    	shiftOut(DS_pin, SHCP_pin2, LSBFIRST,blank);
    	digitalWrite(STCP_pin2, HIGH);
  	}
  // writes tens to second 7-segment display when bool ten is true and bool hundred is false
  	else if (ten == true && hundred == false) {
    	digitalWrite(STCP_pin2,LOW);
    	shiftOut(DS_pin, SHCP_pin2, LSBFIRST,dec_digits[number]);
    	digitalWrite(STCP_pin2, HIGH);
  	}
    // writes tens with decimalpoint to second 7-segment display when both booleans are true. Decimalpoint indicates number 100 has been reached
  	else if(ten == true && hundred == true && twoHundred == false)
  	{
    	digitalWrite(STCP_pin2,LOW);
    	shiftOut(DS_pin, SHCP_pin2, LSBFIRST,dec_digitsHundred[number]);
    	digitalWrite(STCP_pin2, HIGH);
  	}
  } else // following code is run if result is 200 or more
  {
    // writes ones with decimalpoint to first 7-segment display. Decimalpoint indicates number 200 has been reached
    if (ten == false && hundred == false && twoHundred == true) {
  
    	digitalWrite(STCP_pin,LOW);
    	shiftOut(DS_pin, SHCP_pin, LSBFIRST,dec_digitsHundred[number]);
    	digitalWrite(STCP_pin, HIGH);
    }
    // writes tens with decimalpoint to second 7-segment display when both booleans are true. Decimalpoint indicates number 100 has been reached
    else if (ten == true && hundred == true && twoHundred == true) {
    
      digitalWrite(STCP_pin2,LOW);
    	shiftOut(DS_pin, SHCP_pin2, LSBFIRST,dec_digitsHundred[number]);
    	digitalWrite(STCP_pin2, HIGH);
    }
  }
}

void writeHighAndLowNumber(uint8_t tens, uint8_t ones, bool hundred, bool twoHundred)
{
  // code is run if result is under 10
  if (tens < 1 && hundred == false && twoHundred == false) {
  	writeByte(ones, false, false, false); // calls writeByte subroutine to write ones on first shift register
    
  }
  // code is run if result is 10-99
  if (tens >= 1 && hundred == false && twoHundred == false) {
    writeByte(ones, false, false, false); // calls writeByte subroutine to write ones on first shift register
  	writeByte(tens, true, false, false);// calls writeByte subroutine to write tens on second shift register
   }

   // code is run if result is 100-199
  if (hundred == true && twoHundred == false) {
  	writeByte(ones, false, false, false); // calls writeByte subroutine to write ones on first shift register
  	writeByte(tens, true, true, false); // calls writeByte subroutine to write tens on second shift register
  }

  // code is run if result is 200 or more
  if (hundred == true && twoHundred == true) {
    writeByte(ones, false, false, true);// calls writeByte subroutine to write ones on first shift register
  	writeByte(tens, true, true, true); // calls writeByte subroutine to write tens on second shift register
  }
}

void showResult(byte result)
{
  // code is run if result is less than 100
  if (result < 100) {
  	uint8_t tens = result / 10; // divides by 10 to get the tens out of game result
  	uint8_t ones = result % 10; // gets remainder after dividing by 10 to get the ones out of the game result
  	writeHighAndLowNumber(tens, ones, false, false); // calls writeHighAndLowNumber subroutine to write the result on 7-segment display
  }
  // code is run if result is 100-199
  if (result >= 100 && result < 200) {
  	uint8_t tens = (result / 10) - 10; // divides by 10 and then decrements 10 (because result is over 100) to get the tens out of game result
  	uint8_t ones = result % 10; // gets remainder after dividing by 10 to get the ones out of the game result
    writeHighAndLowNumber(tens, ones, true, false); // calls writeHighAndLowNumber subroutine to write the result on 7-segment display
  }
  //code is run if result is 200 or more
  if (result >= 200) {
    uint8_t tens = (result / 10) - 20; // divides by 10 and then decrements 10 (because result is over 200) to get the tens out of game result
  	uint8_t ones = result % 10; // gets remainder after dividing by 10 to get the ones out of the game result
    writeHighAndLowNumber(tens, ones, true, true); // calls writeHighAndLowNumber subroutine to write the result on 7-segment display
  }

}

void gameOver()
{
  uint8_t dec_letters [16] {14, 29, 91, 79, 5, 128, 128, 128, 91, 79, 79, 0, 51, 119, 0, 0}; // decimals indicating text "LOSER...SEE(blank)YA(blank)(blank)" to shift register

  // loop to display letters in order
    
  for(int i=0;i<16;i++)
  {
    // shows the letter in first 7-segment display
    digitalWrite(STCP_pin,LOW);
    shiftOut(DS_pin, SHCP_pin, LSBFIRST,dec_letters[i]);
    digitalWrite(STCP_pin, HIGH);
    delay(600); // delay to make reading easier
    // shows the letter in second 7-segment display
    digitalWrite(STCP_pin2,LOW);
    shiftOut(DS_pin2, SHCP_pin2, LSBFIRST,dec_letters[i]);
    digitalWrite(STCP_pin2, HIGH);
  
  }
}


