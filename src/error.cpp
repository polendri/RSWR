/*
 * /----------------------------\
 * | Rainbow Stalin's Wild Ride |
 * \----------------------------/
 *
 * Author:     Paul Hendry (pshendry)
 * Term:       Winter 2013
 * Course:     CS 349
 * Assignment: 1
 *
 * src/error.cpp
 *
 * Implementation of include/error.h
 */

#include <cstdlib>
#include <iostream>

#include "error.h"

void errorExit( const char* msg ) {
    std::cerr << "ERROR: " << msg << std::endl;
    exit( EXIT_FAILURE );
}

void successExit() {
    exit( EXIT_SUCCESS );
}
