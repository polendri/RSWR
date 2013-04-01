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
 * include/error.h
 *
 * A couple functions for exiting the game, either with success or with an
 * error message.
 */

#ifndef __ERROR_H__
#define __ERROR_H__

#include <cstdlib>
#include <iostream>

// errorExit(): Print a message and exit with failure exit code
void errorExit( const char* msg );

// successExit(): exit with success exit code
void successExit();

#endif
