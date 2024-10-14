#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"
// Digitalpins for shift registers

bool playerPressedButton = false;
volatile bool newTimerInterrupt = false;  // for timer interrupt handler
volatile bool gameChecked = true;
volatile int ledArray[300]; // array to store generated numbers
volatile int arrayIndex = 0; // variable to indicate where to store number in array
volatile int buttonsPushed[300]; // array to store data of buttons pushed
volatile int buttonIndex = 0;
int randomLed;
// variables for timer1

volatile int interruptCounter = 0; // variable to count amount of interrupts
volatile unsigned long currentOCR1A = 15624; // variable to OCR1A, value is increased after 10 interrupts
extern volatile int buttonNumber;
unsigned long buttonPressTime = 0;
bool isButtonPressed = false;
bool gameStarted = false;
bool ledActive = false;
volatile int score = 0; // score of right button clicks. This is send to function 'void showResult(byte result)'
int playerButton;

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
      ledActive = false; // Setting led activity to false
    }
  } else {
    isButtonPressed = false; //gives the signal if the button hasn't been pressed 
  }

    if (newTimerInterrupt){
    ledArray[arrayIndex] = randomLed; // stores random number to ledArray
    /*Serial.print("ledi taulukkoon talletetaan: ");
    Serial.println(ledArray[arrayIndex]);*/
    arrayIndex++; //increments arrayIndex by 1
    setLed(randomLed); //lights up the led
    Serial.print("LED numero:  ");  // Debugging statement
    Serial.println(randomLed);
    ledActive = true; // set ledactive to ttrue when the first led is active
    newTimerInterrupt = false;
    }

    if (gameStarted) {
      if (playerPressedButton) {
    checkGame(buttonNumber); // If the game has been started starts checking buttonpresses
    playerPressedButton = false;
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
  buttonsPushed[buttonIndex] = playerButton; // stores the button pressed
         Serial.print("Painettu nappi");
         Serial.println(playerButton);
         Serial.print("button taulukkoon talletetaan: ");
         Serial.println(buttonsPushed[buttonIndex]);
  if (buttonsPushed[buttonIndex] == ledArray[buttonIndex]){ // comparing the pressed button with the led
    shutLed(buttonsPushed[buttonIndex]); // turn off the correct led
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
        textGameOver(); // display game over text
        clearAllLeds(); // clear leds
        gameStarted = false; // reset game state
        cli(); //stop timer
        isButtonPressed = false; // palataan alkuun
        sei();
}

void startTheGame(){
  initializeGame(); // Initialize game variables
  blinkLeds(); // Blink leds for the start indicator
  ledActive = false; //reset ledactive state
}
  
void blinkLeds(){ // blinks all leds for 0.5 seconds and then turns all leds off for 1.5seconds
  for(int i = 0; i<3; i++){
    setAllLeds();
    delay(500);
    clearAllLeds();
    delay(1500);
  }
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

void buttonPressed(){
  /*if (ledActive && digitalRead(2) == LOW || digitalRead(3) == LOW || digitalRead(4) == LOW || digitalRead(5) == LOW ){ //Check if a valid button is pressed and that the leds are active
   Serial.print("Button pressed: ");
        Serial.println(buttonNumber);

    checkGame(buttonNumber); //check if the pressed button is correct
  }
  }*/
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
  
}



