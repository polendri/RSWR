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
 * src/button.cpp
 *
 * Implementation of include/button.h
 */

#include <X11/Xlib.h>

#include "button.h"
#include "state.h"
#include "pixmaps.h"

MenuButton::MenuButton( int bx, int by, bool bfromLeft, bool bfromTop,
                        XInfoColor::Enum bfgColor,
                        XInfoColor::Enum bbgColor,
                        Enum btype )
            : x(bx), y(by), fromLeft(bfromLeft), fromTop(bfromTop),
              pressed(0), type(btype),
              fgColor(bfgColor), bgColor(bbgColor) {
    // Set pixmaps and button dimensions based on which type of button this is
    switch( type ) {
        case QUIT:
            w = 150;
            h = 40;
            pixmapNames[0] = PixmapArray::BUTTON_QUIT;
            pixmapNames[1] = PixmapArray::BUTTON_QUIT_PRESSED;
            break;
        case START:
            w = 150;
            h = 40;
            pixmapNames[0] = PixmapArray::BUTTON_START;
            pixmapNames[1] = PixmapArray::BUTTON_START_PRESSED;
            break;
        case RESUME:
            w = 150;
            h = 40;
            pixmapNames[0] = PixmapArray::BUTTON_RESUME;
            pixmapNames[1] = PixmapArray::BUTTON_RESUME_PRESSED;
            break;
        case RETRY:
            w = 150;
            h = 40;
            pixmapNames[0] = PixmapArray::BUTTON_RETRY;
            pixmapNames[1] = PixmapArray::BUTTON_RETRY_PRESSED;
            break;
        default:
            break;
    }
}

void MenuButton::paint( XInfo& xinfo, PixmapArray& pixmapArray ) const {
    XSetForeground( xinfo.display, xinfo.gc,
                    xinfo.color[ (int) fgColor ].pixel );
    XSetBackground( xinfo.display, xinfo.gc,
                    xinfo.color[ (int) bgColor ].pixel );
    int screenX, screenY;
    if( fromLeft ) { screenX = x; }
    else           { screenX = xinfo.windowAttr.width - x; }
    if( fromTop )  { screenY = y; }
    else           { screenY = xinfo.windowAttr.height - y; }
    XCopyPlane( xinfo.display,
                pixmapArray[ pixmapNames[pressed] ],
                xinfo.window, xinfo.gc,
                0, 0, w, h, screenX, screenY, 1 );
}

void paintAllButtons( XInfo& xinfo, PixmapArray& pixmapArray,
                      MenuButton** buttonArray, int numButtons ) {
    for( int i=0; i<numButtons; i++ ) {
        buttonArray[i]->paint( xinfo, pixmapArray );
    }
}

void pressButtonForLocation( XInfo& xinfo, PixmapArray& pixmapArray,
                             MenuButton** buttonArray, int numButtons,
                             int x, int y, bool doPaint ) {
    for( int i=0; i<numButtons; i++) {
        if( buttonArray[i]->isWithinButton( xinfo, x, y ) ) {
            buttonArray[i]->press();
            if( doPaint ) {
                buttonArray[i]->paint( xinfo, pixmapArray );
            }
            return;
        }
    }
}

MenuButton::Enum releaseButtonForLocation(
        XInfo& xinfo, PixmapArray& pixmapArray,
        MenuButton** buttonArray, int numButtons,
        int x, int y, bool doPaint ) {
    bool buttonFound = false;
    MenuButton::Enum buttonType = MenuButton::NONE;

    for( int i=0; i<numButtons; i++) {
        if( buttonArray[i]->isPressed() ) {
            // Release any pressed button
            buttonArray[i]->release();
            if( doPaint ) {
                buttonArray[i]->paint( xinfo, pixmapArray );
            }

            if( buttonArray[i]->isWithinButton( xinfo, x, y ) ) {
                // Record the button type of the first button found under the
                // cursor
                if( !buttonFound ) {
                    buttonType = buttonArray[i]->type;
                    buttonFound = true;
                }
            }
        }
    }
    return buttonType;
}
