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
 * Contains the menuloop() function which displays the game menu and does
 * relevant event handling.
 */

#ifndef __MENU_H__
#define __MENU_H__

#include "state.h"

class PixmapArray;

void menuloop( XInfo&, PixmapArray&, GameState::Enum& );

#endif
