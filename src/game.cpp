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
 * src/game.cpp
 *
 * Implementation of include/game.h
 */

#include <cmath>
#include <ctime>
#include <sstream>
#include <sys/time.h>
#include <X11/Xlib.h>

#include "button.h"
#include "error.h"
#include "pixmaps.h"
#include "state.h"

const int frameRate = 30;

unsigned long now() {
    timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

// paintTerrain(): Repaints all terrain
void paintTerrain( XInfo& xinfo, PixmapArray& pixmapArray,
                   GameData& gameData ) {
    int base = floor(gameData.origin);
    double offset = gameData.origin - base;
    for( int i=0; i<41; i++ ) {
        double width = xinfo.windowAttr.width / 40.0;
        double height = (double)(gameData.terrain[base]) *
                        (double)(xinfo.windowAttr.height) / 30.0;
        double x = (i - offset) * width;
        double y = xinfo.windowAttr.height - height;

        XSetForeground( xinfo.display, xinfo.gc,
                        xinfo.color[ (int) XInfoColor::GREEN ].pixel );
        XFillRectangle( xinfo.display, xinfo.window, xinfo.gc,
                        (int) x, (int) y, (int) width + 1, (int) height );

        base = (base + 1) % 41;
    }
}

// paintPlayer(): Paints the player's entity
void paintPlayer( XInfo& xinfo, GameData& gameData ) {
    static int color = 0; // XInfoColor code for the next colour to draw

    double scaleX = xinfo.windowAttr.width / 800.0;
    double scaleY = xinfo.windowAttr.height / 600.0;
    double screenUnitX = xinfo.windowAttr.width / 40.0;
    double screenUnitY = xinfo.windowAttr.height / 30.0;
    XPoint points[] = {
        { gameData.playerX*screenUnitX + scaleX*(-20.0),
          gameData.playerY*screenUnitY + scaleY*4.0 },
        { 0, scaleY*(-12.0) },
        { scaleX*4, 0 },
        { scaleX*4, scaleY*4 },
        { scaleX*22, 0 },
        { scaleX*10, scaleY*8 },
        { scaleX*(-18), 0 },
        { scaleX*(-4), scaleY*4 },
        { scaleX*(-8), 0 },
        { scaleX*4, scaleY*(-4) }
    };
    int numPoints = sizeof(points) / sizeof(XPoint);
    XSetBackground( xinfo.display, xinfo.gc,
                    xinfo.color[ (int) XInfoColor::SKYBLUE ].pixel );
    XSetForeground( xinfo.display, xinfo.gc,
                    xinfo.color[ color+2 ].pixel );
    XFillPolygon( xinfo.display, xinfo.window, xinfo.gc,
                  points, numPoints, Nonconvex, CoordModePrevious );

    color = (color + 1) % 9;
}

// paintPropaganda(): paints the propaganda projectiles dropped by the player
//   (only active ones are painted)
void paintPropaganda( XInfo& xinfo, GameData& gameData ) {
    double screenUnitX = xinfo.windowAttr.width / 40.0;
    double screenUnitY = xinfo.windowAttr.height / 30.0;
    XSetForeground( xinfo.display, xinfo.gc,
                    xinfo.color[ (int) XInfoColor::YELLOW ].pixel );
    for( int i=0; i<gameData.maxPropaganda; i++ ) {
        if( gameData.propagandas[i].isActive ) {
            XFillRectangle( xinfo.display, xinfo.window, xinfo.gc,
                            screenUnitX * gameData.propagandas[i].x - 3,
                            screenUnitY * gameData.propagandas[i].y - 5,
                            6, 10 );
        }
    }
}

// paintPeasants(): Paint the peasants standing on pillars
void paintPeasants( XInfo& xinfo, GameData& gameData ) {
    double screenUnitX = xinfo.windowAttr.width / 40.0;
    double screenUnitY = xinfo.windowAttr.height / 30.0;
    int base = floor(gameData.origin);
    double offset = gameData.origin - base;
    for( int i=0; i<41; i++ ) {
        if( gameData.peasants[base] > 0 ) {
            if( gameData.peasants[base] == 1 ) {
                XSetForeground( xinfo.display, xinfo.gc,
                                xinfo.color[ (int) XInfoColor::BLACK ].pixel );
            }
            if( gameData.peasants[base] == 2 ) {
                XSetForeground( xinfo.display, xinfo.gc,
                                xinfo.color[ (int) XInfoColor::RED ].pixel );
            }
            double width = 0.5 * screenUnitX;
            double height = screenUnitY;
            double x = (i - offset + 0.25) * screenUnitX;
            double y = xinfo.windowAttr.height - height -
                    (gameData.terrain[base] * screenUnitY);
            XFillRectangle( xinfo.display, xinfo.window, xinfo.gc,
                            (int) x, (int) y, (int) width, (int) height );
        }
        base = (base + 1) % 41;
    }
}

// displayScore(): Print score at top-right of screen
void displayScore( XInfo& xinfo, GameData& gameData ) {
    std::stringstream ss; //gotta use stringstreams just to convert int to str...
    ss << gameData.score;
    std::string str = ss.str();
    XSetForeground( xinfo.display, xinfo.gc,
                    xinfo.color[ (int) XInfoColor::BLACK ].pixel );
    XDrawImageString( xinfo.display, xinfo.window, xinfo.gc, 
            xinfo.windowAttr.width - 20, 20, str.c_str(), str.length() );
}

// paintGame(): Repaints the whole game screen
void paintGame( XInfo& xinfo, PixmapArray& pixmapArray,
                GameData& gameData,
                MenuButton** buttonArray, int numButtons ) {
    XClearWindow( xinfo.display, xinfo.window );
    paintTerrain( xinfo, pixmapArray, gameData );
    paintPeasants( xinfo, gameData );
    paintPropaganda( xinfo, gameData );
    paintPlayer( xinfo, gameData );
    displayScore( xinfo, gameData );
    paintAllButtons( xinfo, pixmapArray, buttonArray, numButtons );
}

// buildPillar(): Given a previous pillar, returns a random height which
//   is at most 4 away from the previous pillar, and is at least 0 and at
//   most 12.
int buildPillar( int prevHeight ) {
    return std::max( 1, std::min( 12, prevHeight + (rand() % 5) - 2 ) );
}

// generateTerrain(): Completely generates new terrain
void generateTerrain( GameData& gameData ) {
    gameData.origin = 0.0;
    gameData.terrain[0] = buildPillar(7);
    for( int i=1; i<41; i++ ) {
        gameData.terrain[i] = buildPillar( gameData.terrain[i-1] );
    }
    for( int i=0; i<41; i++ ) { gameData.peasants[i] = 0; }
}

// moveTerrain(): Moves game origin to the right by a given amount, 
//   generating a new piece of terrain if required. Returns true if an
//   unbrainwashed peasant was removed to create the new terrain (indicating
//   that the player has failed)
bool moveTerrain( GameData& gameData ) {
    bool ret = false;
    double step = gameData.terrainMoveStep;
    double oldOrigin = gameData.origin;
    int oldBase = floor(oldOrigin);
    gameData.origin = fmod(oldOrigin + step, 41);

    if( gameData.peasants[oldBase] == 1 ) {
        ret = true;
    }

    // If we crossed a whole number, a new piece of terrain must be generated
    if( (int)(oldOrigin) < (int)(oldOrigin+step) ) {
        gameData.terrain[ oldBase ] = buildPillar(
                gameData.terrain[ (oldBase+40)%41 ] );
        int j = rand() % 10;
        if( j == 0 ) {
            gameData.peasants[ oldBase ] = 1;
        } else {
            gameData.peasants[ oldBase ] = 0;
        }
    }

    return ret;
}

// movePlayer(): Moves player entity based on the direction values in the
//   game data
void movePlayer( GameData& gameData ) {
    gameData.playerX = fmin( 40.0, fmax( 0.0, 
            gameData.playerX + gameData.playerVelX * gameData.playerMoveStep ) );
    gameData.playerY = fmin( 40.0, fmax( 0.0,
            gameData.playerY + gameData.playerVelY * gameData.playerMoveStep ) );
}

// movePropaganda(): Moves all propaganda entities
void movePropaganda( GameData& gameData ) {
    for( int i=0; i<gameData.maxPropaganda; i++ ) {
        Propaganda* p = &gameData.propagandas[i];
        if( p->isActive ) {
            p->x += p->velX * gameData.terrainMoveStep;
            p->y += p->velY * gameData.terrainMoveStep;
            if( p->x > 40.0 || p->x < 0.0 || p->y > 40.0 || p->y < 0.0 ) {
                p->isActive = false;
            }
        }
    }
}

// spawnPropaganda(): Spawns a propaganda object at the player's location 
//   on a frame interval
void spawnPropaganda( GameData& gameData ) {
    static int spawnTimer = 0;

    if( spawnTimer < gameData.propagandaBurstLength &&
            spawnTimer % gameData.propagandaSpawnRate == 0 ) {
        double randVel = 0.25 * (double)(rand()) / RAND_MAX;
        int i = gameData.nextPropaganda;
        gameData.propagandas[i].isActive = true;
        gameData.propagandas[i].x = gameData.playerX;
        gameData.propagandas[i].y = gameData.playerY;
        gameData.propagandas[i].velX = gameData.playerVelX - 1.0 + randVel;
        gameData.propagandas[i].velY = 1.0 + randVel;
        gameData.nextPropaganda = (gameData.nextPropaganda + 1) % 
                gameData.maxPropaganda;
    }
    spawnTimer = (spawnTimer + 1) %
            (gameData.propagandaBurstLength + gameData.propagandaRecharge);
}

bool playerCollidingWithTerrain( GameData& gameData ) {
    int pillar = fmod( gameData.playerX + gameData.origin, 41 );
    return (30.0 - gameData.playerY) <= (double) gameData.terrain[pillar];
}

void doPropagandaCollisions( GameData& gameData ) {
    for( int i=0; i<gameData.maxPropaganda; i++ ) {
        Propaganda* p = &gameData.propagandas[i];
        int pillar = fmod( p->x + gameData.origin, 41 );
        if( (30.0 - p->y) <= (double) gameData.terrain[pillar] ) {
            p->velX = -1.0;
            p->velY = 0.0;

            // Check for collision with peasant
            if( gameData.peasants[pillar] == 1 &&
                    (30.0 - p->y) + 0.5 >= (double) gameData.terrain[pillar] ) {
                gameData.peasants[pillar] = 2;
                gameData.score++;
            }
        }
    }
}

void initializeGame( GameData& gameData ) {
    generateTerrain( gameData );
    gameData.terrainMoveStep = 0.2;
    gameData.playerMoveStep = 0.4;
    gameData.playerX = 10.0;
    gameData.playerY = 10.0;
    gameData.playerVelX = 0.0;
    gameData.playerVelY = 0.0;
    for( int i=0; i<gameData.maxPropaganda; i++ ) {
        gameData.propagandas[i].isActive = false;
    }
    gameData.nextPropaganda = 0;
    gameData.propagandaSpawnRate = 4;
    gameData.propagandaBurstLength = 64;
    gameData.propagandaRecharge = 48;
    gameData.score = 0;
}

void gameloop( XInfo& xinfo, PixmapArray& pixmapArray,
               GameState::Enum& gameState, GameData& gameData ) {
    XEvent event;
    KeySym keySym;
    XWindowAttributes windowAttr;
    XGetWindowAttributes( xinfo.display, xinfo.window, &windowAttr);

    // Set window background to sky blue
    XSetWindowBackground( xinfo.display, xinfo.window,
                          xinfo.color[ (int) XInfoColor::SKYBLUE ].pixel );

    // Create the array of all buttons appearing on-screen
    int numButtons = 1;
    MenuButton* buttonArray[ numButtons ];
    buttonArray[0] = new MenuButton( 10, 50, true, false,
                                     XInfoColor::BLACK, XInfoColor::YELLOW,
                                     MenuButton::QUIT );

    // Do initial paint of screen
    paintGame( xinfo, pixmapArray, gameData, buttonArray, numButtons );
    XFlush( xinfo.display );

    unsigned long lastFrame = now();

    // Game loop
    while( 1 ) {
        if( XPending( xinfo.display ) > 0 ) {
            XNextEvent( xinfo.display, &event );
            switch (event.type) {
            case Expose:
                // If window is exposed, repaint everything
                //paintGame( xinfo, pixmapArray, gameData, buttonArray, numButtons );
                break;
            case ButtonPress:
                // If mouse is clicked, visually press down any buttons which
                // are under the mouse
                pressButtonForLocation( xinfo, pixmapArray,
                                        buttonArray, numButtons,
                                        event.xbutton.x, event.xbutton.y,
                                        false );
                break;
            case ButtonRelease:
                // If mouse is released, visually unpress all buttons, and do
                // the action of the first button which is under the mouse
                MenuButton::Enum buttonType;
                buttonType = releaseButtonForLocation(
                        xinfo, pixmapArray,
                        buttonArray, numButtons,
                        event.xbutton.x, event.xbutton.y,
                        false );

                switch( buttonType ) {
                    case MenuButton::QUIT:
                        gameState = GameState::PAUSE;
                        return;
                    default:
                        break;
                }
                break;
            case KeyPress:
                keySym = XKeycodeToKeysym( xinfo.display, event.xkey.keycode, 0 );
                switch( keySym ) {
                case XK_Up:
                    gameData.playerVelY = -1.0;
                    break;
                case XK_Down:
                    gameData.playerVelY = 1.0;
                    break;
                case XK_Left:
                    gameData.playerVelX = -1.0;
                    break;
                case XK_Right:
                    gameData.playerVelX = 2.0;
                    break;
                case XK_q:
                    gameState = GameState::PAUSE;
                    return;
                case XK_f:
                    gameState = GameState::PAUSE;
                    return;
                case XK_z:
                    gameData.terrainMoveStep = fmax( 0.1,
                            gameData.terrainMoveStep - 0.025 );
                    break;
                case XK_x:
                    gameData.terrainMoveStep = fmin( 0.3,
                            gameData.terrainMoveStep + 0.025 );
                    break;
                default:
                    break;
                }
                break;
            case KeyRelease:
                keySym = XKeycodeToKeysym( xinfo.display, event.xkey.keycode, 0 );
                switch( keySym ) {
                case XK_Up:
                    gameData.playerVelY = 0.0;
                    break;
                case XK_Down:
                    gameData.playerVelY = 0.0;
                    break;
                case XK_Left:
                    gameData.playerVelX = 0.0;
                    break;
                case XK_Right:
                    gameData.playerVelX = 0.0;
                    break;
                }
                break;
            case ConfigureNotify:
                // Resize window if requested and repaint
                XResizeWindow( xinfo.display, xinfo.window,
                        event.xconfigure.width, event.xconfigure.height );
                XGetWindowAttributes( xinfo.display, xinfo.window,
                                      &xinfo.windowAttr);
                break;
            }
        }

        unsigned long end = now();
        if( (end-lastFrame) >= (1000000/frameRate) ) {
            bool ret = moveTerrain( gameData );
            movePlayer( gameData );
            movePropaganda( gameData );
            if( ret || playerCollidingWithTerrain( gameData ) ) {
                sleep(1);
                gameState = GameState::FAIL;
                return;
            }
            spawnPropaganda( gameData );
            doPropagandaCollisions( gameData );
            paintGame( xinfo, pixmapArray, gameData, buttonArray, numButtons );
            XFlush( xinfo.display );
            lastFrame = now();
        }
        if( XPending( xinfo.display ) == 0 ) {
            usleep( 1000000/frameRate - (end-lastFrame) );
        }
    }
}
