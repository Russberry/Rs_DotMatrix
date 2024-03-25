/* 
  This is an example for creating simple Dot Matrix board
  Connect CS pin of MAX7219 to Arduino pin 10
  Connect LED1 and LED2 to pin 4 and 5 respectively
  Connect button with 10k pull-up resistor to pin 3 (External Interrupt 1)
*/
#define LED1 4
#define LED2 5
#define LEDInterval 0.5 // in second

#include <Rs_DotMatrix.h>

#define CSPin 10
#define Intensity 0xFF

// Initiate the module with CS pin of 10 and set to maximum led intensity
Rs_DotMatrix display = Rs_DotMatrix(CSPin, Intensity);

// Function for setup TIMER1 overflow interrupt
void Timer1_Setup() {
  TCCR1A = 0;
  TCCR1B = 0b00000101;  // Set clock prescaler to 1024
  TIMSK1 = 0b00000001;  // Enable timer overflow interrupt
  TCNT1 = 65535 - LEDInterval * (F_CPU) / 1024; // Set time between each interrupt
}

// FCallback function for External interrupt 1
void LED_Toggle() {
  digitalWrite(LED1, !digitalRead(LED1));
}

// Callback function for TIMER1 overflow interrupt
ISR(TIMER1_OVF_vect) {
  digitalWrite(LED2, !digitalRead(LED2));
  TCNT1 = 65535 - LEDInterval * (F_CPU) / 1024;
}

void setup() {
  // Setup LED pin
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // Setup interrupt callback function
  attachInterrupt(1, LED_Toggle, FALLING);

  // Setup TIMER1 overflow interrupt
  Timer1_Setup();

}

void loop() {
  display.PrintScrollString("Complete", 800);
  delay(500);
}
