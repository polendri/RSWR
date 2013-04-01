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
 * src/state.h
 *
 * Contains constants and class definitions defining game/window state
 */

#ifndef __STATE_H__
#define __STATE_H__

#include <X11/Xlib.h>

namespace XInfoColor {
    const int numColors = 10;
    enum Enum {
        BLACK = 0,
        WHITE,
        RED,
        ORANGE,
        YELLOW,
        GREEN,
        BLUE,
        INDIGO,
        PURPLE,
        SKYBLUE,
    };
}

// XInfo: Collects X info for drawing to the window
struct XInfo {
    Display* display;
    Window   window;
    XWindowAttributes windowAttr;
    GC       gc;
    Colormap colorMap;
    XColor color[ XInfoColor::numColors ];
};

// GameState: enum of the possible game states
namespace GameState {
    enum Enum {
        MENU = 0,
        GAME,
        PAUSE,
        FAIL,
        QUIT
    };
};

struct Propaganda {
    bool isActive;
    double x,y;
    double velX, velY;
};

struct GameData {
    // Height of each of the 41 terrain pillars
    int terrain[41];

    // Indicator for what occupies each pillar. 0 indicates nothing, 1
    // indicates an unbrainwashed peasant, and 2 represents a brainwashed one.
    int peasants[41];

    // Point, mod 40, which is the leftmost point of the 
    // terrain (so if origin=20.5, terrain[20] is the leftmost
    // pillar on the screen and is partly obstructed)
    double origin;

    // Rate of movement of the terrain, player, and propaganda
    double terrainMoveStep, playerMoveStep, propagandaMoveStep;

    // X- and Y-coordinates of the player
    double playerX, playerY;

    // Player velocity
    double playerVelX, playerVelY;

    // Maximum number of propaganda which can exist simultaneously
    static const int maxPropaganda = 30;

    // Array holding the propaganda objects (some of which may not be active)
    Propaganda propagandas[maxPropaganda];

    // Index of the next propaganda object set to be overwritten
    int nextPropaganda;

    // How often propaganda spawns (in frames)
    int propagandaSpawnRate;
    int propagandaBurstLength;
    int propagandaRecharge;

    // How many peasants the player has brainwashed
    int score;
};

#endif
