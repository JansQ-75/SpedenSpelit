#include "buttons.h"
volatile int buttonNumber;
unsigned long lastDebounceTime = 0; // Debounce time
const unsigned long debounceDelay = 500; // Debounce delay in ms


void initButtonsAndButtonInterrupts(void)
{
  for (int i = 2; i < 6; i++) { // Setting the buttons to input_pullup mode
    pinMode(i, INPUT_PULLUP);
  }
  cli(); // stops interrupts
    // Activating PCINT2 interrupts for pins 2-5
  PCICR |= (1 << PCIE2); // Activating the interrups
  PCMSK2 |= B00111100;   // Activating the pins 2-5
  sei(); // allows interrupts
}

  ISR(PCINT2_vect) { //Handles the interrupts for pins 2-5 
unsigned long currentTime = millis();
 
  for (int i = 2; i < 6; i++) {    // Checking which of the buttons are in low state
    if (digitalRead(i) == LOW) {

      if ((currentTime - lastDebounceTime) > debounceDelay) { // Checking the debounce time
         buttonNumber = i - 2;  // Saves the button that  has been pressed
        lastDebounceTime = currentTime; // Updates the debounce time
        playerPressedButton = true;
      }
      

        break; // Breaks if a button has been pressed.
    }
  }
  }