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
 * include/game.h
 *
 * Contains the gameloop() function which controls actual gameplay, and the
 * initializeGame() function for initialization of the game
 */

#ifndef __GAME_H__
#define __GAME_H__

#include "state.h"

class PixmapArray;

void initializeGame( GameData& );

void gameloop( XInfo&, PixmapArray&, GameState::Enum&, GameData& );

#endif
