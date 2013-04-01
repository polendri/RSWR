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
 * src/menu.cpp
 *
 * Implementation of include/menu.h
 */

#include <list>
#include <X11/Xlib.h>

#include "button.h"
#include "error.h"
#include "pixmaps.h"
#include "state.h"

void paintRainbowArc( XInfo& xinfo, int arcNum, XInfoColor::Enum color ) {
    int w = (int)(xinfo.windowAttr.width * 0.95);
    int h = xinfo.windowAttr.height;
    int x = (int)(xinfo.windowAttr.width * 0.025);
    int y = -100;
    int stepX = (int)(xinfo.windowAttr.width * 0.05);
    int stepY = (int)(xinfo.windowAttr.height * 0.05);

    XSetForeground( xinfo.display, xinfo.gc,
                    xinfo.color[ (int) color ].pixel );
    XFillArc( xinfo.display, xinfo.window, xinfo.gc,
              x+arcNum*stepX/2, y+arcNum*stepY/2,
              w-arcNum*stepX, h-arcNum*stepY, 0, 360*32 );
}

void paintBackground( XInfo& xinfo, PixmapArray& pixmapArray ) {
    XClearWindow( xinfo.display, xinfo.window );

    // Paint rainbow
    paintRainbowArc( xinfo, 0, XInfoColor::ORANGE );
    paintRainbowArc( xinfo, 1, XInfoColor::YELLOW );
    paintRainbowArc( xinfo, 2, XInfoColor::GREEN );
    paintRainbowArc( xinfo, 3, XInfoColor::BLUE );
    paintRainbowArc( xinfo, 4, XInfoColor::INDIGO );
    paintRainbowArc( xinfo, 5, XInfoColor::PURPLE );
    paintRainbowArc( xinfo, 6, XInfoColor::RED );

    // Paint title
    int titleX = xinfo.windowAttr.width/2 - (290/2);
    int titleY = 50;
    XSetForeground( xinfo.display, xinfo.gc,
                    xinfo.color[ (int) XInfoColor::BLACK ].pixel );
    XSetBackground( xinfo.display, xinfo.gc,
                    xinfo.color[ (int) XInfoColor::RED ].pixel );
    XCopyPlane( xinfo.display,
                pixmapArray[ PixmapArray::TITLE ],
                xinfo.window, xinfo.gc,
                0, 0, 290, 184, titleX, titleY, 1 );
    
    // Paint Stalin and instructions
    int stalinX = xinfo.windowAttr.width - 469;
    int stalinY = xinfo.windowAttr.height - 307;
    XCopyPlane( xinfo.display,
                pixmapArray[ PixmapArray::STALIN ],
                xinfo.window, xinfo.gc,
                0, 0, 469, 307, stalinX, stalinY, 1 );
}

void paintMenu( XInfo& xinfo, PixmapArray& pixmapArray,
               MenuButton** buttonArray, int numButtons ) {
    paintBackground( xinfo, pixmapArray );
    paintAllButtons( xinfo, pixmapArray, buttonArray, numButtons );
}

void menuloop( XInfo& xinfo, PixmapArray& pixmapArray, GameState::Enum& gameState ) {
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
    if( gameState == GameState::MENU ) {
        buttonArray[1] = new MenuButton( 10, 100, true, false,
                                         XInfoColor::BLACK, XInfoColor::YELLOW,
                                         MenuButton::START );
    }
    else { //if( gameState == GameState::PAUSE ) {
        buttonArray[1] = new MenuButton( 10, 100, true, false,
                                         XInfoColor::BLACK, XInfoColor::YELLOW,
                                         MenuButton::RESUME );
    }

    // Do initial paint of screen
    paintMenu( xinfo, pixmapArray, buttonArray, numButtons );
    XFlush( xinfo.display );

    // Event loop
    while( 1 ) {
        XNextEvent( xinfo.display, &event );
        switch (event.type) {
        case Expose:
            // If window is exposed, repaint everything
            paintMenu( xinfo, pixmapArray, buttonArray, numButtons );
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
            case MenuButton::QUIT:
                gameState = GameState::QUIT;
                return;
            case MenuButton::START:
            case MenuButton::RESUME:
                gameState = GameState::GAME;
                return;
            default:
                break;
            }
            break;
        case KeyPress:
            keySym = XKeycodeToKeysym( xinfo.display, event.xkey.keycode, 0 );
            switch( keySym ) {
            case XK_s:
                if( gameState == GameState::MENU ) {
                    gameState = GameState::GAME;
                    return;
                }
                break;
            case XK_r:
                if( gameState == GameState::PAUSE ) {
                    gameState = GameState::GAME;
                    return;
                }
                break;
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
            paintMenu( xinfo, pixmapArray, buttonArray, numButtons );
            break;
        default:
            break;
        }
    }

    for( int i=0; i<numButtons; i++ ) {
        delete buttonArray[i];
    }
}
