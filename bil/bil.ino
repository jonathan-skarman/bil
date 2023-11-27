#include <IRremote.hpp>  // Including the IRremote library

#define IR_RECEIVE_PIN 3 // Define the IR receiver pin

uint16_t kommando = 0; // Variable to store the received command

void setup() {
  pinMode(4, OUTPUT); // Set pin 4 as output
  pinMode(2, OUTPUT); // Set pin 2 as output

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Initialize IR receiver
}

//---------------------------------------------------------------

void loop() { // Main loop

  if (IrReceiver.decode()) { // Check if an IR signal is received
    uint16_t kommando = IrReceiver.decodedIRData.command; // Store the received command

    maine(kommando); // Call the 'maine' function with the received command
    IrReceiver.resume(); // Resume receiving IR signals
  }
  else {
    maine(kommando); // Call 'maine' function with the stored command if no new signal is received
  }
}

//---------------------------------------------------------------

void maine(int kommando) { // Function to handle commands received from IR

  switch (kommando) { // Switch-case to perform actions based on the received command
    case 70:
      forward(); // Perform forward action
      break;
    case 21:
      backward(); // Perform backward action
      break;
    case 64:
      broms(); // Perform braking action
      break;
    case 67:
      right(); // Perform right turn action
      break;
    case 68:
      left(); // Perform left turn action
      break;
    default:
      // Handle default case if needed
      break;
  }
  delay(1000); // Delay after performing the action
}

/*----------*/

// Functions to handle steering actions

//set "speed" actually means set amount of electricity given, but that decides speed and is therefore called speed below

void forward() {
  digitalWrite(4, HIGH); // Left motor forward
  analogWrite(5, 100); // Set speed for left motor
  digitalWrite(2, HIGH); // Right motor forward
  analogWrite(9, 100); // Set speed for right motor
}

void backward() {
  digitalWrite(4, LOW); // Left motor backward
  analogWrite(5, 100); // Set speed for left motor
  digitalWrite(2, LOW); // Right motor backward
  analogWrite(9, 100); // Set speed for right motor
}

void right() {
  digitalWrite(4, LOW); // Left motor backward
  analogWrite(5, 100); // Set speed for left motor
  digitalWrite(2, HIGH); // Right motor forward
  analogWrite(9, 100); // Set speed for right motor
}

void left() {
  digitalWrite(4, HIGH); // Left motor forward
  analogWrite(5, 100); // Set speed for left motor
  digitalWrite(2, LOW); // Right motor backward
  analogWrite(9, 100); // Set speed for right motor
}

void broms() {
  analogWrite(5, 0); // Stop left motor
  analogWrite(9, 0); // Stop right motor
}