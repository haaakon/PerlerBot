//
// PerlerBot
//
// Making a bot that can print using perler beads
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author	 	Håkon Bogen
// 				Håkon Bogen
//
// Date			03.02.14 17:47
// Version		0.1
//
// Copyright	© Håkon Bogen, 2014
// License		MIT
//
// See			ReadMe.txt for references
//


// Core library for code-sense
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad MSP430, Stellaris and Tiva, Experimeter Board FR5739 specific
#include "Energia.h"
#elif defined(CORE_TEENSY) // Teensy specific
#include "WProgram.h"
#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#elif defined(ARDUINO) && (ARDUINO < 100) // Arduino 23 specific
#include "WProgram.h"
#else // error
#error Platform not defined
#endif

// Include application, user and local libraries
#include "LocalLibrary.h"

#include <Stepper.h>
#include <Servo.h>
#include "PatternReader.h"


// Prototypes


// Define variables and constants
//
// Brief	Name of the LED
// Details	Each board has a LED but connected to a different pin
//
uint8_t myLED;

Servo myservo;
int threshold = 830;


// PINS
Stepper conveyorStepper(200, 0,1,2,3);

Stepper xValueStepper(200, 4,5,6,7);

Stepper yValueStepper(200,8,9,10,11);

int redledpin = 13;

// microswitches
int outOfBoundsXNorthPin = 14;
int outOfBoundsXSouthPin = 15;

int outOfBoundsYNorthPin = 16;
int outOfBoundsYSouthPin = 17;

int servopin = 18;

// bead measurer
int analogPin = A1;

int stepperSpeed = 20;

int pos = 0;    // variable to store the servo position

int hasRegisteredBead = 0;

// MACHINE STATE

// stores the number of steps the printer has taken on x axis
int printerPositionX = 0;
// printer is moving forward
int isGoingForward = 1;

PatternReader patternReader;


void setup() {
    
    patternReader.currentPositionX = 0;
    patternReader.beadPuttingState = BeadPuttingStateWaitingForRead;
    
    // sets the pin as output
    
    pinMode(redledpin, OUTPUT);   // sets the pin as output
    Serial.begin(9600);
    xValueStepper.setSpeed(stepperSpeed);
    conveyorStepper.setSpeed(stepperSpeed);
    myservo.attach(servopin);
}

void sweepServo()
{
    for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees
    {
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
                                         // waits 15ms for the servo to reach the position
    }
    for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees
    {                                  // in steps of 1 degree
        myservo.write(pos);
        delay(15);    // tell servo to go to position in variable 'pos'                        // waits 15ms for the servo to reach the position
    }
}


bool isRobotIsOutOfBoundsInXAxis()
{
    if (digitalRead(outOfBoundsXSouthPin)){
        return true;
    }
    if (digitalRead(outOfBoundsXNorthPin)){
        return true;
    }
    
    return false;
}

bool isRobotIsOutOfBoundsInYAxis() {
    
    if (digitalRead(outOfBoundsYNorthPin)){
        return true;
    }
    if (digitalRead(outOfBoundsYSouthPin)){
        return true;
    }
    return false;
}

void moveXaxis(int steps) {
    if (isRobotIsOutOfBoundsInXAxis() == false) {
        xValueStepper.step(steps);
    }
}

void moveYaxis(int steps) {
    if (isRobotIsOutOfBoundsInYAxis() == false) {
        yValueStepper.step(steps);
    }
}

void moveToNextBeadX (){
    moveXaxis(10);
}

void resetXAxis() {
    
}

void resetYAxis() {
    
}

void loop() {
    Serial.print("bead putting state on top of loop:");
    Serial.println(patternReader.beadPuttingState);
    if (patternReader.beadPuttingState == BeadPuttingStateWaitingForRead) {
        BeadReadingState readState = patternReader.read();
        switch (readState) {
                // if read found bead, fire up the conveyour belt to get it down there
            case BeadReadingStateFoundBead:{
                patternReader.beadPuttingState = BeadPuttingStateConveyorMoving;
                break;
            }
            case BeadReadingStateFoundEmptyBeadSpot: {
                moveToNextBeadX();
                break;
            }
            case BeadReadingStateWaitingForRead: {
                break;
            }
                
            default:
                break;
        }
        
    }
    
    while (!digitalRead(53)) {
        Serial.println("Waiting for someone to hit me on pin 53...");
        delay(1000);
    }
    
    if (printerPositionX == 300) {
        isGoingForward = 0;
    } else if (printerPositionX == 0) {
        isGoingForward = 1;
    }
    
    if (patternReader.beadPuttingState == BeadPuttingStateConveyorMoving){
        conveyorStepper.step(-1);
    }
    
    if (analogRead(analogPin) < threshold) {
        patternReader.beadPuttingState = BeadPuttingStateBeadEnteredInfrared;
        digitalWrite(redledpin, HIGH);  // turn LED ON
        conveyorStepper.step(-1);
        Serial.println(analogRead(analogPin));
        hasRegisteredBead = 1;
    } else {
        if (patternReader.beadPuttingState  == BeadPuttingStateBeadEnteredInfrared){
            patternReader.beadPuttingState = BeadPuttingStateBeadExitedInfrared;
            // Step twice to be sure to drop bead
            conveyorStepper.step(-2);
            // Sweep servo twice to be sure to drop bead
            sweepServo();
            sweepServo();
            hasRegisteredBead = 0;
        }
        digitalWrite(redledpin, LOW);
    }
    
    if (patternReader.beadPuttingState == BeadPuttingStateBeadExitedInfrared){
        moveToNextBeadX();
        // do stuff?
    }
}
