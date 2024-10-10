#include "leds.h"


  // // Määritellään LEDien pinnit
const int ledPins[] = {A2, A3, A4, A5}; // LEDit kytketty analogiapinneihin A2-A5
int currentLed = 0;  // Tietoa siitä, mikä LED on tällä hetkellä päällä

// Alustetaan LEDit
void initializeLeds() {
  for (int i = 2; i < 6; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
}

void setRandomLed() {
  clearAllLeds(); // Sammutetaan kaikki LEDit ensin
  int randNumber = random(0, 4); // Valitaan satunnainen LED
  setLed(randNumber); // Sytytetään satunnainen LED
  delay (2000);

}

void setLed(int ledNumber) {
  switch (ledNumber) {
    case 0:
      digitalWrite(A2, HIGH);
      break;
    case 1:
      digitalWrite(A3, HIGH);
      break;
    case 2:
      digitalWrite(A4, HIGH);
      break;
    case 3:
      digitalWrite(A5, HIGH);
      break;
    default:
      break;
  }
}


// Sammutetaan kaikki LEDit
void clearAllLeds() {
   for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], LOW);
    }
}

// Sytyttää kaikki LEDit
void setAllLeds() {
   for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], HIGH);
    }
}

// Näyttää yksinkertaisen valoshown ennen peliä
void show1() {
    for (int i = 0; i < 4; i++) {
        setLed(i);
        delay(500);  // Pieni viive, jotta käyttäjä voi nähdä valon
    }
}

// Näyttää nopeutuvan valoshown
void show2() {
    for (int i = 0; i < 4; i++) {
        setLed(i);
        delay(250);  // Nopeampi viive
    }
    setAllLeds();
    delay(100);  // Viimeinen nopea välähdys
    clearAllLeds();
}


