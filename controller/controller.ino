// Including necessary libraries and defining constants
#define DECODE_NEC
#define DECODE_DISTANCE_WIDTH
#include <IRremote.hpp>
#define DELAY_AFTER_SEND 2000
#define DELAY_AFTER_LOOP 5000
int IR_SEND_PIN = 3;

#include <ezButton.h>
#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
#define SW_PIN   2  // Arduino pin connected to SW  pin
ezButton button(SW_PIN);
int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
int bValue = 0; // To store value of the button

/*--------------------------------------------------*/

void setup () {
  delay(4000); // Initial delay for setup

  IrSender.begin(IR_SEND_PIN); // Initializing IR sender

  button.setDebounceTime(50); // Setting debounce time for the button
}

uint16_t sAddress = 0x0102; // Initial value for the IR address
uint16_t sCommand = 0x70;   // Initial value for the IR command
uint8_t sRepeats = 1;       // Initial value for IR repeats

/*
 * Send NEC IR protocol
 */
void send_ir_data(uint16_t sCommand ) {
  // Function to send IR data using NEC protocol
  // ... (Debugging Serial prints are commented out)
    /*
      //serial prints to handle debugging, kept in code just in case
      Serial.print(F("Sending: 0x"));
      Serial.print(sAddress, HEX);
      Serial.print(" ");
      Serial.print(sCommand, HEX);
      Serial.print(" ");
      Serial.println(sRepeats, HEX);
      Serial.flush(); // To avoid disturbing the software PWM generation by serial output interrupts
    */

  // Clip repeats at 4
  if (sRepeats > 4) {
      sRepeats = 4;
  }
  // Sending IR using NEC protocol
  IrSender.sendNEC(sAddress, sCommand, sRepeats);
}

/*--------------------------------------------------*/

void loop () {
  // Checks for button input and reads analog values
  button.loop(); // Button input handling

  xValue = analogRead(VRX_PIN); // Read X-axis analog value
  yValue = analogRead(VRY_PIN); // Read Y-axis analog value
  bValue = button.getState();   // Read button state

  // Sends appropriate signal based on input
  if (button.isPressed()) {
    broms(); // Send broms signal
  }
  else if (yValue > 923) {
    forward(); // Send forward signal
  }
  else if (yValue < 100) {
    backward(); // Send backward signal
  }
  else if (xValue > 923) {
    right(); // Send right signal
  }
  else if (xValue < 100) {
    left(); // Send left signal
  }

  // Wait for the receiver state machine to detect the end of a protocol
  delay((RECORD_GAP_MICROS / 1000) + 5);

  // Prepare data for the next loop
  sAddress += 0x0101;
  sRepeats++;

  // Delay before next iteration
  delay(100);
}

/*--------------------------------------------------*/

// Functions to send specific IR data
void forward() {
  send_ir_data(70); // Send forward IR signal
}

void backward () {
  send_ir_data(21); // Send backward IR signal
}

void right () {
  send_ir_data(67); // Send right IR signal
}

void left () {
  send_ir_data(68); // Send left IR signal
}

void broms () {
  send_ir_data(64); // Send broms IR signal
}