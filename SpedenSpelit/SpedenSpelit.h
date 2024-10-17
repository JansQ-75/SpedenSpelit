#ifndef SPEDENSPELIT_H
#define SPEDENSPELIT_H
#include <arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*
  initializeTimer() subroutine intializes Arduino Timer1 module to
  give interrupts at rate 1Hz.
  Also allows ORC1A value to be changed during the game via 'currentORC1A' variable --> speeds up timer with 10% every 10th interrupt.
*/
void initializeTimer(void);

// Intoduce TIMER1_COMPA_vect Interrupt SeRvice (ISR) function for timer.
ISR(TIMER1_COMPA_vect);

/*
  initializeGame() subroutine is used to initialize all variables
  needed to store random numbers and player button push data.
  This function is called from startTheGame() function.
  
*/
void initializeGame(void);

/*
  checkGame() subroutine is used to check the status
  of the Game after each player button press.
  
  If the latest player button press is wrong, the game stops
  and if the latest press was right, game display is incremented
  by 1.
  
  Parameters
  byte: number of button player pressed; 0, 1, 2 or 3.
*/
void checkGame(byte);


/*
  startTheGame() subroutine calls InitializeGame()
  function and enables Timer1 interrupts to start
  the Game.
*/
void startTheGame(void);

/* 
  gameOver() subroutine blinks the leads to inform player the game has been lost.
  textGameOver() function is called to write "Loser...see ya" to 7-segment displays.
  Timer1 interrupts are stopped.
  Resets booleans 'gameStarted' and isButtonPressed to false, in order to allow new game to be started.
*/
void gameOver();

/*
  playerHighScore() function compares player's last score to game's high score. If last score is higher, function sets it as an new high score.
  After comparison, high score is shown on display

  parameter
  int: player's last score
*/
void playerHighScore(int);

#endif