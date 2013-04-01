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
 * include/menu.h
 *
 * Contains the failloop() function which displays the failure screen.
 */

#ifndef __FAIL_H__
#define __FAIL_H__

#include "state.h"

class PixmapArray;

void failloop( XInfo&, PixmapArray&, GameState::Enum&, int score );

#endif
