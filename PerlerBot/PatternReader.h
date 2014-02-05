//
// File			PatternReader.h
// Class library header
//
// Details		<#details#>
//
// Project	 	PerlerBot
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author		Håkon Bogen
// 				Håkon Bogen
//
// Date			05.02.14 18:20
// Version		<#version#>
//
// Copyright	© Håkon Bogen, 2014
// License	    <#license#>
//
// See			ReadMe.txt for references
//


// Core library - IDE-based
#if defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad MSP430, Stellaris and Tiva, Experimeter Board FR5739 specific
#include "Energia.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(CORE_TEENSY) // Teensy specific
#include "WProgram.h"
#elif defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0x and 1.5x specific
#include "Arduino.h"
#elif defined(ARDUINO) && (ARDUINO < 100)  // Arduino 23 specific
#include "WProgram.h"
#endif // end IDE

#ifndef PatternReader_h

#define PatternReader_h



struct perlerBoard{
    int x[50];
    int y[50];
};

class PatternReader {
    
public:
    
    
    PatternReader();
    void begin();
    String WhoAmI();
    void set(uint8_t data);
    uint8_t get();
    
    /**
     *  the current position the reader is at on the x axis
     */
    int currentPositionX;
    
    /**
     *  Returns a  one dimensional demo pattern for testing. Its only x axis, other axis is empty.
     *
     *  @return array with patterm
     */
    perlerBoard testPattern1DX();
    
    /**
     *  Reads the next
     */
    void read();
    
private:
    uint8_t _data;
};

#endif
