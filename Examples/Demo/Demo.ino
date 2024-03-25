/* 
  This is an example usage of this library
  Connect CS pin of MAX7219 to Arduino pin 10
*/
#include <Rs_DotMatrix.h>

#define CSPin 10
#define Intensity 0xFF

// Initiate the module with CS pin of 10 and set to maximum led intensity
Rs_DotMatrix display = Rs_DotMatrix(CSPin, Intensity);

void setup() {
  // Print Whatsup with 400ms delay between each letter
  display.PrintString("Whatsup", 400);

  // Set the led intensity down
  display.SetIntensity(0x01);

  // Print scrolling Hello with 400ms delay between each letter
  display.PrintScrollString("Hello", 400);

  // Turn off display
  display.DisplayOff();
}

void loop() {

}
