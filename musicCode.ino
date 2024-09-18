#include "pitches.h"

// Define pin connections
const int buzzer1 = 9;  // Buzzer 1 on pin 9 (must be a PWM capable pin)
const int buzzer2 = 10; // Buzzer 2 on pin 10 (must be a PWM capable pin)
const int buttonPin = A0; // Button on pin A0

// Variables to manage button state
bool isPlaying = false;
int lastButtonState = LOW;

// Melody notes
int melody1[] = {NOTE_B4, NOTE_C5, NOTE_A4, NOTE_A3, NOTE_A4};
int melody2[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4};
int noteDurations = 250; // Duration of each note in milliseconds

void setup() {
  pinMode(buzzer1, OUTPUT);
  pinMode(buzzer2, OUTPUT);
  pinMode(buttonPin, INPUT);

  // Begin serial communication at 9600
  Serial.begin(9600);
}

void loop() {
  // Check if data is available on the serial line
  if (Serial.available() > 0) {
    char receivedChar = Serial.read(); // Read the incoming character
    // If the character is 'M', toggle the playing state
    if (receivedChar == 'M') {
      isPlaying = true;
    }
  }

  // Read the state of the button
  int buttonState = digitalRead(buttonPin);

  // Toggle playing state on button press
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      isPlaying = !isPlaying;
    }
    delay(50); // Debouncing
    lastButtonState = buttonState;
  }

  // Play melody if the button is pressed
  if (isPlaying) {
    for (int i = 0; i < 5; i++) {
      // Check button state in every iteration
      buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH) {
        isPlaying = false;
        break;
      }

      //play buzzer1
      tone(buzzer1, melody1[i], noteDurations);
      delay(noteDurations);
      noTone(buzzer1);
      delay(100); // Short delay between notes

      //play buzzer2
      tone(buzzer2, melody2[i], noteDurations);
      delay(noteDurations);
      noTone(buzzer2);
      delay(100); // Short delay between notes
    }
  }
}
