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
 * src/main.cpp
 *
 * Performs Xlib initializations and runs the appropriate code for the current
 * game state.
 */

#include <cstdlib>
#include <ctime>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "error.h"
#include "game.h"
#include "fail.h"
#include "menu.h"
#include "pixmaps.h"
#include "state.h"

// Defines for window geometry and keyboard events
const int Border = 5;

void initX(int argc, char *argv[], XInfo& xinfo) {
    srand ( time(NULL) );

    XSizeHints hints;
    int screen;
    unsigned long white, black;

    // Open display
    xinfo.display = XOpenDisplay("");
    if (!xinfo.display) {
        errorExit("Unable to connect to X server.");
    }

    // Get some info about the display
    screen = DefaultScreen( xinfo.display );
    white = WhitePixel( xinfo.display, screen );
    black = BlackPixel( xinfo.display, screen );

    // Set up hints and properties for the window manager, and open a window
    hints.x = 100;
    hints.y = 100;
    hints.width = 800;
    hints.height = 600;
    hints.flags = PSize;
    xinfo.window = XCreateSimpleWindow(
            xinfo.display, DefaultRootWindow( xinfo.display ),
            hints.x, hints.y, hints.width, hints.height,
            Border, black, white );
    XSetStandardProperties(
            xinfo.display, xinfo.window,
            "Rainbow Stalin's Wild Ride",
            "RSWR",
            None, argv, argc, &hints );

    // Get window attributes
    XGetWindowAttributes( xinfo.display, xinfo.window, &xinfo.windowAttr);

    // Get graphics context
    xinfo.gc = XCreateGC (xinfo.display, xinfo.window, 0, 0 );
    XSetBackground( xinfo.display, xinfo.gc, white );
    XSetForeground( xinfo.display, xinfo.gc, black );

    // Tell the WM what input events to register for
    XSelectInput( xinfo.display, xinfo.window,
                  StructureNotifyMask | ExposureMask |
                  ButtonPressMask | ButtonReleaseMask |
                  KeyPressMask | KeyReleaseMask );

    // Set up colour map
    xinfo.colorMap = DefaultColormap( xinfo.display, 0 );
    XParseColor( xinfo.display, xinfo.colorMap, "#000000",
                 &xinfo.color[ XInfoColor::BLACK ] );
    XParseColor( xinfo.display, xinfo.colorMap, "#ffffff",
                 &xinfo.color[ XInfoColor::WHITE ] );
    XParseColor( xinfo.display, xinfo.colorMap, "#ff0000",
                 &xinfo.color[ XInfoColor::RED ] );
    XParseColor( xinfo.display, xinfo.colorMap, "#ff8000",
                 &xinfo.color[ XInfoColor::ORANGE ] );
    XParseColor( xinfo.display, xinfo.colorMap, "#ffff00",
                 &xinfo.color[ XInfoColor::YELLOW ] );
    XParseColor( xinfo.display, xinfo.colorMap, "#00ff00",
                 &xinfo.color[ XInfoColor::GREEN ] );
    XParseColor( xinfo.display, xinfo.colorMap, "#0000ff",
                 &xinfo.color[ XInfoColor::BLUE ] );
    XParseColor( xinfo.display, xinfo.colorMap, "#4b0082",
                 &xinfo.color[ XInfoColor::INDIGO ] );
    XParseColor( xinfo.display, xinfo.colorMap, "#ff00ff",
                 &xinfo.color[ XInfoColor::PURPLE ] );
    XParseColor( xinfo.display, xinfo.colorMap, "#80d0f0",
                 &xinfo.color[ XInfoColor::SKYBLUE ] );
    for( int i=0; i<XInfoColor::numColors; i++ ) {
        XAllocColor( xinfo.display, xinfo.colorMap, &xinfo.color[i] );
    }

    // Put the window on the screen
    XMapRaised( xinfo.display, xinfo.window );
}



void mainloop( XInfo xinfo, PixmapArray pixmapArray ) {
    GameState::Enum gameState = GameState::MENU;
    GameData gameData;
    initializeGame( gameData );

    bool quit = false;
    while (!quit) {
        switch (gameState) {
            case GameState::MENU:
                menuloop( xinfo, pixmapArray, gameState );
                break;
            case GameState::GAME:
                gameloop( xinfo, pixmapArray, gameState, gameData );
                break;
            case GameState::PAUSE:
                menuloop( xinfo, pixmapArray, gameState );
                break;
            case GameState::FAIL:
                failloop( xinfo, pixmapArray, gameState, gameData.score );
                initializeGame( gameData );
                break;
            case GameState::QUIT:
                quit = true;
                break;
        }
    }
}

int main( int argc, char* argv[] ) {
    // Create and initialize X window
    XInfo xinfo;
    initX(argc, argv, xinfo);

    // Load pixmaps
    PixmapArray pixmapArray( xinfo );

    // Run main game loop
    mainloop( xinfo, pixmapArray);

    // Close display and exit
    XCloseDisplay( xinfo.display );
}
