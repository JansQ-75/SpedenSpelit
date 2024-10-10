#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"
// Digitalpins for shift registers

volatile bool newTimerInterrupt = false;  // for timer interrupt handler
volatile bool gameChecked = true;

volatile int randArray[300]; // array to store generated numbers
volatile int arrayIndex = 0; // variable to indicate where to store number in array
volatile int buttonsPushed[300]; // array to store data of buttons pushed
volatile int buttonIndex = 0;

// variables for timer1
volatile int randNumber = 0; // variable for random numbers generated with timer1
volatile int interruptCounter = 0; // variable to count amount of interrupts
volatile unsigned long currentOCR1A = 15624; // variable to OCR1A, value is increased after 10 interrupts

unsigned long buttonPressTime = 0;
bool isButtonPressed = false;
bool gameStarted = false;
bool ledActive = false;
  volatile int score; // score of right button clicks. This is send to function 'void showResult(byte result)'
void setup()
{
  Serial.begin(9600);
  interrupts(); // Allowing interrupts
  initButtonsAndButtonInterrupts(); // Initializing buttons and button interrupts
  initializeDisplay(); // Initializing display
  initializeLeds(); // Initializing leds

}

void loop()
{
  buttonPressed();
  if (digitalRead(2) == LOW) { // Oletetaan, että nappi 2 käynnistää pelin
    if (!isButtonPressed) {
      // Jos nappia ei ollut vielä painettu, tallennetaan painalluksen alkuaika
      buttonPressTime = millis(); // Tallennetaan nykyinen aika
      isButtonPressed = true;
    }

    // Tarkistetaan, onko nappia pidetty pohjassa yli 1000 ms (1 sekunti)
    if (millis() - buttonPressTime >= 1000 && !gameStarted) {
      // Käynnistetään peli, jos nappia on pidetty 1s
      Serial.println("Peli alkaa!");
      startTheGame(); // Käynnistetään peli
      initializeTimer(); // Ajastin alkaa
      gameStarted = true; // Merkitään, että peli on aloitettu
      ledActive = false;
    }
  } else {
    // Jos nappia ei paineta, nollataan tilat
    isButtonPressed = false;
  }

  // aloitetaan peli
  //ledi syttyy generoidun numeron perusteella
  if (gameStarted) {
  if(newTimerInterrupt == true)
    {
      setRandomLed();
      Serial.print("LEDI PÄÄLLÄ: ");
      Serial.println(randNumber);
      ledActive = true;
      newTimerInterrupt = false;
  }
 // Tarkistetaan, onko pelaaja painanut nappia, kun LED on aktiivinen
    if (ledActive && buttonNumber != 0) { 
      checkGame(buttonNumber); // Tarkistetaan, oliko oikea nappi
      ledActive = false;       // Nollataan LED-tila, koska nappia painettiin

      if (gameChecked) {
        score++; // Lisätään piste, jos nappi oli oikea
        Serial.print("Pisteet: ");
        Serial.println(score);
      } else {
        Serial.println("Peli ohi");
        textGameOver(); // Pelin loppu
        gameStarted = false; // Nollataan pelin tila
      }

      // Nollataan painetun napin numero
      buttonNumber = 0;
    }

  }
  // painetaan nappia
  // oliko oikea nappi? Kyllä: lisätään +1 näytölle --> 10x oikein generointi nopeutuu (tämä jo timerissa sisällä)

  // jos nappia ei paineta 5s sisään, peli päättyy, kaikki ledit vilkkuvat, textGameOver, näytetään loppupisteet

  // nollataan kaikki? Alustetaan peli uudelleen?

  clearAllLeds();
  delay(300);
  
  clearAllLeds();
  delay(300);
  
  
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
  randNumber = random(0, 4);

  // Increments counter by 1
  interruptCounter++;

  /*checks if 10 interrupts have occurred and increases timer interrupt rate by 10%.
  Also sets interruptCounter back to 0.
  */

  if (interruptCounter >= 10) {
    currentOCR1A = (unsigned long)(currentOCR1A * 0.9);  // Increase timers interrupts by 10%

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
  
      // stores generated number to array
   // randArray[arrayIndex] = randNumber;
    // Increments arrayIndex by 1
    //arrayIndex++;
    // resets arrayIndex if 300 has been reached
    //if (arrayIndex > 300)
      //{
      //arrayIndex = 0;
    //  }
}


void checkGame(int pressedButton) //checkGame
{


  if (pressedButton == randNumber) { // Checking if the button pressed is right with the active led
    gameChecked = true;    
    }

  else {// Checking if the button pressed was wrong with the active led, if it is then game over
    Serial.println("Peli ohi!");// For debugging with serial montior
    gameChecked = false; // Set flag to notify loop() that game is over
  }
}
  





void initializeGame()
{
	/*initializeGame() subroutine is used to initialize all variables
  needed to store random numbers and player button push data.
  This function is called from startTheGame() function.*/
  

  volatile bool rightButton = true; // boolean to be used as flag for the loop. When false: Indicates the player pushes wrong button
}

void startTheGame()
{
  initializeGame();
  clearAllLeds();
  setAllLeds();
  delay(1500);
  clearAllLeds();
  delay(300);
}
// function to read which button is pressed and to store it to array
void buttonPressed()
{
  for (int i = 2; i < 6; i++) {    // Checking which of the buttons are in low state
    if (digitalRead(i) == LOW) {
      buttonNumber = i -2;
      // stores number of pressed button to array
      buttonsPushed[buttonIndex] = buttonNumber;
      // Increments arrayIndex by 1
      buttonIndex++;
    }
    // resets buttonsIndex if 300 has been reached
    if (buttonIndex > 300)
      {
      buttonIndex = 0;
      }
  }
}


