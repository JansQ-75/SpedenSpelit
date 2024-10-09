void setup() {
  // // Määritellään LEDien pinnit
const int ledPins[] = {A2, A3, A4, A5}; // LEDit kytketty analogiapinneihin A2-A5
int currentLed = 0;  // Tietoa siitä, mikä LED on tällä hetkellä päällä

// Alustetaan LEDit
void initializeLeds() {
    for (int i = 0; i < 4; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
    clearAllLeds();  // Sammutetaan kaikki LEDit alussa
}

// Sytyttää tietyn LEDin ja sammuttaa muut
void setLed(int led) {
    clearAllLeds();  // Sammutetaan kaikki LEDit ensin
    digitalWrite(ledPins[led], HIGH);  // Sytytetään valittu LED
    currentLed = led;  // Päivitetään nykyinen LED
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

void setup() {
    initializeLeds();  // Alustetaan LEDit
    show1();  // Ajetaan valoshow1 ennen peliä
}

void loop() {
    // Tämä looppi on pelin aikana käytössä. Esimerkiksi valitaan syttyvä LED ja odotetaan käyttäjän syötettä.
    for (int i = 0; i < 4; i++) {
        setLed(i);  // Sytytetään yksitellen LEDit
        delay(1000);  // Viive, jotta LED palaa hetken
    }
}
