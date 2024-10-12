# SpedenSpelit_Ryhma26
Project name: SpedenSpelit

    in the 90's in Finland ther was a gameshow on tv called SpedenSpelit. In this gameshow Nopeustesti -game was introduced to public. Nopeustesti -game is a reaction game that has 4 LED buttons. LED buttons are lit randomly and player must press the right button in order to continue the game. As the game progresses, LEDs light up faster and the game comes more difficult.

Project goals:

    Main goal for the project is to make our own version of the SpedenSpelit reaction game. We are using Arduino UNO microcontroller for the game. All needed components are connected to breadboard in order of achieving working hardware.
    Game also needs functioning software and coding is done by using Arduino IDE. The code consists of four modules: buttons, leds, display and gamelogic. We need to integrate these modules to get a working game.

Hardware:

    For the game we had these components:

        - Arduino UNO microcontroller
        - 4 LEDs
        - 4 pushbuttons
        - 2 7-segment displays (common cathode)
        - 2 shift registers (74HC595)
        - breadboard(s)
        - Hook up wires and jump wires
        - resistors for the leds (minimun 470 ohm)

    LEDs were connected to Arduino UNO analog pins A2-A5.

    Buttons were connected to Arduino UNO digital pins 2-5.

    First shift register's clock pin was connected to Arduino UNO digital pin 8, latch pin to digital pin 9 and input pin to digital pin 10.

    Second shift register's clock pin was connected to digital pin 11, latch pin to digital pin 12 and input pin to digital pin 10 (same as first shift register).

    Both shift register's power pin and SRCLR input pin were connected to Arduino UNO's 5V pin. Output Enable pin and ground pin were connected to ground.

    Shift register's output pins were connected via resistors (minimum 470 ohm) to 7-segment display as followed:
        QA - DP
        QB - A
        QC - B
        QD - C
        QE - D
        QF - E
        QG - F
        QH - G
        QH' - NOT CONNECTED

    7-segment common cathode pin was connected to ground.

Software:
    Coding was done by using Arduino IDE.
    We were given baseline code for the project. This code consisted of four modules; leds, buttons, display and gamelogic. Gamelogic is basically the main .ino file. We ended up also adding module gamechecking.

    Firstly we coded leds, buttons and display modules. After that we concentrated on gamelogic and gamechecking module was made as by-product.

    Modules includes following functions and subroutines:

    Leds.cpp
        -
    Buttons.cpp
        -
    Display.cpp
        -
    GameChecking.cpp
        -
    Gamelogic (SpedenSpelit.ino)
        -

lopputulos

arviointi

lähteet&vastuualueet