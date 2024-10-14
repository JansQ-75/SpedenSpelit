#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

// variables for arrays needed in game
volatile int ledArray[300]; // array to store generated numbers aka leds lit
volatile int ledIndex = 0; // variable to indicate where to store number in 'led'array
volatile int buttonArray[300]; // array to store data of buttons pushed
volatile int buttonIndex = 0; // variable to indicate where to store number in 'button' array

// variables for timer1
volatile bool newTimerInterrupt = false;  // for timer interrupt handler
volatile int interruptCounter = 0; // variable to count amount of interrupts
volatile unsigned long currentOCR1A = 15624; // variable to OCR1A, value is increased after 10 interrupts
int randomLed; // variable used to light up certain LED.

// variables for buttons
bool playerPressedButton = false;  // boolean to indicate when player has pressed some button
unsigned long buttonPressTime = 0;
bool isButtonPressed = false;
int playerButton; // variable in checkGame function. (buttonNumber = playerButton).



bool gameStarted = false; // boolean to indicate when game has been started by player.
// bool ledActive = false; // voi poistaa?
volatile int score = 0; // score of right button clicks. This is send to function 'void showResult(byte result)'


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
  // Before game has been started, first LED indicates which button is the "start button". Player must press "start button" for 1 second to start the game
  if (!gameStarted) {
    setLed(0);
  }
  if (digitalRead(2) == LOW) { // Check if button 0 (analog 2) is being pressed to start the game
    if (!isButtonPressed) {
      buttonPressTime = millis(); // If the button hasn't yet been pressed save the starting time of press
      isButtonPressed = true; // Gives the signal that the button has been pressed
    }

    if (millis() - buttonPressTime >= 1000 && !gameStarted) { // Checking if the button has been pressed for 1 second (1000 ms)
      startTheGame(); // Start the game if the button has been pressed for 1 second
      Serial.println("Peli alkaa!");
      initializeTimer(); // Timer starts
      gameStarted = true; // Informs that the game has been started
    }
  } else {
    isButtonPressed = false; //gives the signal if the button hasn't been pressed 
  }

    if (newTimerInterrupt){
    ledArray[ledIndex] = randomLed; // stores random number to ledArray
    /*Serial.print("ledi taulukkoon talletetaan: ");
    Serial.println(ledArray[arrayIndex]);*/
    ledIndex++; //increments arrayIndex by 1
    setLed(randomLed); //lights up the led
    Serial.print("LED numero:  ");  // Debugging statement
    Serial.println(randomLed);
    newTimerInterrupt = false;
    }

    // This code is run only if game has been started
    if (gameStarted) {
      if (playerPressedButton) {// boolean is true --> player has pressed some button and we start checking buttonpresses
    checkGame(buttonNumber); // calls checkGame() -funktion with the button number player has pressed
    playerPressedButton = false; // boolean is set to false to avoid unwanted re-run
    }
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

  randomLed = random(0,4); // Generate a random number when the interrupt occurs
    
    
  interruptCounter++; //increments interruptcounter


  if (interruptCounter >= 10) { //checks if 10 interrupts have occurred
    currentOCR1A = (unsigned long)(currentOCR1A * 0.9);  // Increase timers interrupts by 10%

    // Maximum value for 16-bit register is 65535. We have to ensure the new OCR1A value is within valid range
    if (currentOCR1A < 65536) {
      OCR1A = currentOCR1A;
    } else {
      OCR1A = 65535;  // Maximum value is set for OCR1A
    }

    interruptCounter = 0; // Reset counter after increasing timer
  }

  newTimerInterrupt = true; // notify loop that the new random number is ready


}


void checkGame(int playerButton) //checkGame
{
  buttonArray[buttonIndex] = playerButton; // stores the button pressed
         Serial.print("Painettu nappi");
         Serial.println(playerButton);
         Serial.print("button taulukkoon talletetaan: ");
         Serial.println(buttonArray[buttonIndex]);
  if (buttonArray[buttonIndex] == ledArray[buttonIndex]){ // comparing the pressed button with the led
    shutLed(buttonArray[buttonIndex]); // turn off the correct led
    buttonIndex++;
    score++; //increments score
    Serial.print("Pisteet: ");
    Serial.println(score);
    showResult(score); // update display with current score
  }
  else { // Wrong button press equals gameover
    gameOver();
  }
  
}

void gameOver(){
        Serial.print("Peli ohi..."); //Gamer over message in serial monitor
        clearAllLeds(); // clear leds
        blinkLeds();
        textGameOver(); // calls 'textGameOver' -function to write text to 7-segment display
        cli(); //stop timer
        gameStarted = false; // reset game state
        isButtonPressed = false; // palataan alkuun
        sei(); // ei toimi, koska timer1 jatkaa tästä eteenpäin. Pitäisi nollata jotenkin.
}

void startTheGame(){
  initializeGame(); // Initialize game variables
  blinkLeds(); // Blink leds for the start indicator
}

void initializeGame()
{
  gameStarted = true; // flags game started as true
    score = 0; // reset score on game initialization
  showResult (0); // Show starting score 0
  clearAllLeds(); // Clears leds
  buttonIndex = 0;
  playerPressedButton = false;


} 

// voi poistaa?
/*void buttonPressed(){
  if (ledActive && digitalRead(2) == LOW || digitalRead(3) == LOW || digitalRead(4) == LOW || digitalRead(5) == LOW ){ //Check if a valid button is pressed and that the leds are active
   Serial.print("Button pressed: ");
        Serial.println(buttonNumber);

    checkGame(buttonNumber); //check if the pressed button is correct
  }
  }
  if (digitalRead(2) == HIGH && digitalRead(3) == HIGH  && digitalRead(4) == HIGH && digitalRead(5) == HIGH) {
  
    if (digitalRead(2) == LOW) { // Check if button on pin 2 is pressed
      playerButton = 0; // Set buttonNumber to 0 for button on pin 2
    } 
    else if (digitalRead(3) == LOW) { // Check if button on pin 3 is pressed
      playerButton = 1; // Set buttonNumber to 1 for button on pin 3
    }
    else if (digitalRead(4) == LOW) { // Check if button on pin 4 is pressed
      playerButton = 2; // Set buttonNumber to 2 for button on pin 4
    }
    else if (digitalRead(5) == LOW) { // Check if button on pin 5 is pressed
      playerButton = 3; // Set buttonNumber to 3 for button on pin 5
    }
  }
    
    // If any button was pressed, call checkGame()
    if (playerButton >= 0 && playerButton <= 3) {
      Serial.print("Button pressed: ");
      Serial.println(playerButton);

      checkGame(playerButton); // Call checkGame() with the pressed button number
      playerButton = 5; // sets playerButton value to 5. In order to fail if-clause
    }
  
}*/



