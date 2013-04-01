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
 * src/fail.cpp
 *
 * Implementation of include/fail.h
 */

#include <sstream>
#include <X11/Xlib.h>

#include "button.h"
#include "error.h"
#include "pixmaps.h"
#include "state.h"

void paintWindow( XInfo& xinfo, PixmapArray& pixmapArray,
               MenuButton** buttonArray, int numButtons, int score ) {
    XClearWindow( xinfo.display, xinfo.window );
    XSetForeground( xinfo.display, xinfo.gc,
                    xinfo.color[ (int) XInfoColor::BLACK ].pixel );
    XSetBackground( xinfo.display, xinfo.gc,
                    xinfo.color[ (int) XInfoColor::RED ].pixel );
    int failX = xinfo.windowAttr.width/2 - 300;
    int failY = xinfo.windowAttr.height/2 - 200;
    XCopyPlane( xinfo.display,
                pixmapArray[ PixmapArray::FAIL ],
                xinfo.window, xinfo.gc,
                0, 0, 600, 400, failX, failY, 1 );
    std::stringstream ss; //gotta use stringstreams just to convert int to str...
    ss << score;
    std::string str = ss.str();
    XSetForeground( xinfo.display, xinfo.gc,
                    xinfo.color[ (int) XInfoColor::BLACK ].pixel );
    XDrawImageString( xinfo.display, xinfo.window, xinfo.gc, 
            xinfo.windowAttr.width - 20, 20, str.c_str(), str.length() );
    paintAllButtons( xinfo, pixmapArray, buttonArray, numButtons );
}

void failloop( XInfo& xinfo, PixmapArray& pixmapArray, GameState::Enum& gameState, int score ) {
    XEvent event;
    KeySym keySym;

    // Set window background to red
    XSetWindowBackground( xinfo.display, xinfo.window,
                          xinfo.color[ (int) XInfoColor::RED ].pixel );

    // Create the array of all buttons appearing on the menu
    int numButtons = 2;
    MenuButton* buttonArray[numButtons];
    buttonArray[0] = new MenuButton( 10, 50, true, false,
                                     XInfoColor::BLACK, XInfoColor::YELLOW,
                                     MenuButton::QUIT );
    buttonArray[1] = new MenuButton( 10, 100, true, false,
                                     XInfoColor::BLACK, XInfoColor::YELLOW,
                                     MenuButton::RETRY );

    // Do initial paint of screen
    paintWindow( xinfo, pixmapArray, buttonArray, numButtons, score );
    XFlush( xinfo.display );

    // Event loop
    while( 1 ) {
        XNextEvent( xinfo.display, &event );
        switch (event.type) {
        case Expose:
            // If window is exposed, repaint everything
            paintWindow( xinfo, pixmapArray, buttonArray, numButtons, score );
            XFlush( xinfo.display );
            break;
        case ButtonPress:
            // If mouse is clicked, visually press down any buttons which
            // are under the mouse
            pressButtonForLocation( xinfo, pixmapArray,
                                    buttonArray, numButtons,
                                    event.xbutton.x, event.xbutton.y );
            XFlush( xinfo.display );
            break;
        case ButtonRelease:
            // If mouse is released, visually unpress all buttons, and do
            // the action of the first button which is under the mouse
            MenuButton::Enum buttonType;
            buttonType = releaseButtonForLocation(
                    xinfo, pixmapArray,
                    buttonArray, numButtons,
                    event.xbutton.x, event.xbutton.y );
            XFlush( xinfo.display );

            switch( buttonType ) {
            case MenuButton::RETRY:
                gameState = GameState::GAME;
                return;
            case MenuButton::QUIT:
                gameState = GameState::QUIT;
                return;
            default:
                break;
            }
            break;
        case KeyPress:
            keySym = XKeycodeToKeysym( xinfo.display, event.xkey.keycode, 0 );
            switch( keySym ) {
            case XK_r:
                gameState = GameState::GAME;
                return;
            case XK_q:
                gameState = GameState::QUIT;
                return;
            default:
                break;
            }
            break;
        case ConfigureNotify:
            // Resize window if requested and repaint
            XResizeWindow( xinfo.display, xinfo.window,
                    event.xconfigure.width, event.xconfigure.height );
            XGetWindowAttributes( xinfo.display, xinfo.window,
                                  &xinfo.windowAttr);
            paintWindow( xinfo, pixmapArray, buttonArray, numButtons, score );
            break;
        default:
            break;
        }
    }

    for( int i=0; i<numButtons; i++ ) {
        delete buttonArray[i];
    }
}
