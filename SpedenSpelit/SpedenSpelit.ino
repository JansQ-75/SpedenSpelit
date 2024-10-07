#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler
volatile int randNumber = 0; // variable for random numbers generated with timer1
volatile int interruptCounter = 0; // variable to count amount of interrupts
volatile unsigned long currentOCR1A = 15624; // variable to OCR1A, value is increased after 10 interrupts

const int DS_pin = 10; // Arduino pin 13 = input pin in first shift register
const int STCP_pin = 9; // Arduino pin 12 = latch pin in first shift register
const int SHCP_pin = 8; // Arduino pin 11 = clock pin in first shift register
const int STCP_pin2 = 12; // Arduino pin 9 = latch pin in second shift register
const int SHCP_pin2 = 11; // Arduino pin 8 = clock pin in second shift register

void setup()
{
  /*
    Initialize here all modules
  */
  void initializeDisplay(void);
}

void loop()
{
  if(buttonNumber>=0)
  {
     // start the game if buttonNumber == 4
     // check the game if 0<=buttonNumber<4
  }

  if(newTimerInterrupt == true)
  {
     // new random number must be generated
     // and corresponding let must be activated
     void setLed(randNumber); // active the random led
     newTimerInterrupt = false; //reset the flag (boolean to false)
  }
}

void initializeTimer(void)
{
  cli(); // stops interrupts

  /*intializes Arduino Timer1 module to
  give interrupts at rate 1Hz*/

	TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  OCR1A = currentOCR1A;// starts with currentOCR1A=15624, meaning compare match register is set for 1Hz increments
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei(); // allows interrupts
}
ISR(TIMER1_COMPA_vect)
{
  /*
  Communicate to loop() that it's time to make new random number.
  Increase timer interrupt rate after 10 interrupts.
  */
  // Generate a random number when the interrupt occurs
  randNumber = random(0, 3);
  // Increments counter by 1
  interruptCounter++;

  /*checks if 10 interrupts have occurred and increases timer interrupt rate by 10%.
  Also sets interruptCounter back to 0.
  */

  if (interruptCounter >= 10) {
    currentOCR1A = (unsigned long)(currentOCR1A * 1.1);  // Increase timers interrupts by 10%

    // Maximum value for 16-bit register is 65535. We have to ensure the new OCR1A value is within valid range
    if (currentOCR1A < 65536) {
      OCR1A = currentOCR1A;
    } else {
      OCR1A = 65535;  // Maximum value is set for OCR1A
    }

    interruptCounter = 0; // Reset counter after increasing timer
  }

  // Set flag to notify loop() that new random number is ready
  newTimerInterrupt = true;
  
}


void checkGame(byte nbrOfButtonPush)
{
	// see requirements for the function from SpedenSpelit.h
}


void initializeGame()
{
	/*initializeGame() subroutine is used to initialize all variables
  needed to store random numbers and player button push data.
  This function is called from startTheGame() function.*/
  volatile int activatedLeds [300]; // array to store data of generated numbers aka activated leds
  volatile int buttonsPushed [300]; // array to store data of buttons pushed
  volatile int score; // score of right button clicks. This is send to function 'void showResult(byte result)'
  volatile bool rightButton = true; // boolean to be used as flag for the loop. When false: Indicates the player pushes wrong button
}

void startTheGame()
{
   // see requirements for the function from SpedenSpelit.h
}

