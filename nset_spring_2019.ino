// NSET - Spring 2019

// receives a message over serial from the Xbee, which determines the mode
// sends a time value (in ms) to the linear actuator to set that mode. 

#include <SoftwareSerial.h>
#define rxPin 7
#define txPin 8
#define lin_act_pin 9
SoftwareSerial xbee_ACS =  SoftwareSerial(rxPin, txPin);
int retract_mode = 1000; // in ms
int vent_mode = 1500;
int detach_mode = 2000;

#include <Servo.h> 
Servo lin_act;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  xbee_ACS.begin(9600); // xbee in the ACS
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  lin_act.attach(lin_act_pin);
}

void loop() {
  // when a newline arrives, determine if a new mode is being set:
  if (stringComplete) {

    // VENT MODE
    if (inputString == "vent") {
      lin_act.writeMicroseconds(vent_mode);
    }

    // DETACH MODE
    else if (inputString == "detach") {
      lin_act.writeMicroseconds(detach_mode);
    }

    // RETRACT MODE
    else {
      lin_act.writeMicroseconds(retract_mode);
    }

    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (xbee_ACS.available()) {
    // get the new byte:
    char inChar = (char)xbee_ACS.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
