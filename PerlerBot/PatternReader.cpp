//
// PatternReader.cpp 
// Class library C++ code
// ----------------------------------
// Developed with embedXcode 
// http://embedXcode.weebly.com
//
// Project 		PerlerBot
//
// Created by 	Håkon Bogen, 05.02.14 18:20
// 				Håkon Bogen
//
// Copyright 	© Håkon Bogen, 2014
// License     MIT
//
// See 			PatternReader.h and ReadMe.txt for references
//


// Library header
#include "PatternReader.h"

// Code
PatternReader::PatternReader() {
    
}

void PatternReader::begin() {
    _data = 0;
}

String PatternReader::WhoAmI() {
    return "PatternReader";
}

void PatternReader::set(uint8_t data) {
    _data = data;
}

perlerBoard PatternReader::testPattern1DX() {
    perlerBoard aBoard = {
        {1, 1, 1}
    };
    return aBoard;
}

void PatternReader::read() {

uint8_t PatternReader::get() {
    return _data; 
}

