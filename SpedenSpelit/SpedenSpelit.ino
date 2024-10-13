#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"
// Digitalpins for shift registers

volatile bool newTimerInterrupt = false;  // for timer interrupt handler
volatile bool gameChecked = true;
volatile int ledArray[300]; // array to store generated numbers
volatile int arrayIndex = 0; // variable to indicate where to store number in array
volatile int buttonsPushed[300]; // array to store data of buttons pushed
volatile int buttonIndex = 0;
int randomLed = 0;
// variables for timer1

volatile int interruptCounter = 0; // variable to count amount of interrupts
volatile unsigned long currentOCR1A = 15624; // variable to OCR1A, value is increased after 10 interrupts
extern volatile int buttonNumber;
unsigned long buttonPressTime = 0;
bool isButtonPressed = false;
bool gameStarted = false;
bool ledActive = false;
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
    if (gameStarted){
      buttonPressed(); // If the game has been started starts checking buttonpresses
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
    ledArray[arrayIndex] = randomLed; // stores random number to ledArray
    arrayIndex++; //increments arrayIndex by 1
    setLed(randomLed); //lights up the led
    Serial.print("LED numero:  ");  // Debugging statement
    Serial.println(randomLed);

    if (arrayIndex == 1){
      ledActive = true; // set ledactive to ttrue when the first led is active
      Serial.println("Ledit valmiina.");
    }
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


void checkGame(byte lastButtonPressed) //checkGame
{
  buttonsPushed[score] = lastButtonPressed; // stores the button pressed
         Serial.print("Painettu nappi");
         Serial.println(lastButtonPressed);
  if (buttonsPushed[score] == ledArray[score]){ // comparing the pressed button with the led
    shutLed(buttonsPushed[score]); // turn off the correct led
    score++; //increments score
    Serial.print("Pisteet: ");
    Serial.println(score);
    showResult(score); // update display with current score
  }
  else{ // Wrong button press equals gameover
    gameOver();
  }
}

void gameOver(){
        Serial.print("Peli ohi..."); //Gamer over message in serial monitor
        textGameOver(); // display game over text
        clearAllLeds(); // clear leds
        gameStarted = false; // reset game state
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

} 

void buttonPressed(){
  if (ledActive && buttonNumber >= 0 && buttonNumber <4 ){ //Check if a valid button is pressed and that the leds are active
   Serial.print("Button pressed: ");
        Serial.println(buttonNumber);

    checkGame(buttonNumber); //check if the pressed button is correct
    buttonIndex++; //increments the buttonindex by 1
  }
  }



