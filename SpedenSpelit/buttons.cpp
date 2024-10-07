#include "buttons.h"

volatile int buttonNumber = 0; //
unsigned long lastDebounceTime = 0; // Debounce aika
const unsigned long debounceDelay = 200; //debounce viive 200ms


void initButtonsAndButtonInterrupts(void)
{
  for (int i = 2; i < 6; i++) {
    pinMode(i, INPUT_PULLUP);
  }
    // Aktivoidaan PCINT2 keskeytykset PORTD:lle (pinnit 2-5)
  PCICR |= (1 << PCIE2); // Pin keskeytykset aktivoidaan
  PCMSK2 |= B00111100;   // Aktivoidaan PCINT2 pinnit (pinnit 2-5)
}

  ISR(PCINT2_vect) { // PCINT2 keskeytyspalvelu, käsittelee pinnit 2–5 (PORTD)
unsigned long currentTime = millis();
 
  for (int i = 2; i < 6; i++) {    // Tarkistetaan, mikä pinni 2–5 on LOW-tilassa
    if (digitalRead(i) == LOW) {

      if ((currentTime - lastDebounceTime) > debounceDelay) { // Varmistetaan, että debounce-aika on kulunut
         buttonNumber = i;  // Tallennetaan painettu nappi
        lastDebounceTime = currentTime; // Päivitetään debounce-aika
      }

        break; // Keskeytetään silmukka, kun löydetään painettu nappi
    }
  }
  }