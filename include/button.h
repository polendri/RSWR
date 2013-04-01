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
 * include/button.h
 *
 * Defines a MenuButton object for creating and manipulating a button of a
 * given type at a given location on the screen, and a method for painting
 * all buttons to the screen.
 */

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "pixmaps.h"
#include "state.h"

class MenuButton {
public:
    enum Enum {
        NONE = 0,
        QUIT,
        START,
        RESUME,
        RETRY
    };
private:
    int x, y;                 // XY-coords of button
    bool fromLeft, fromTop;   // Define origin corner for the XY-coords
    int w, h;                 // Width and height of button
    int pressed;              // Whether button is pressed
    Enum type;                // Type of button ("quit", "start", etc)
    PixmapArray::Enum pixmapNames[2]; // Names of unpressed/pressed pixmaps
    XInfoColor::Enum fgColor; // Foreground/background colours with which to draw
    XInfoColor::Enum bgColor; //

public:
    MenuButton( int bx, int by, bool bfromLeft, bool bfromTop,
                XInfoColor::Enum, XInfoColor::Enum,
                Enum buttonType );

    // isWithinButton(): Returns true if the given (x,y)-coordinates are within
    //   the bounds of this button
    bool isWithinButton( XInfo& xinfo, int mx, int my ) const {
        int screenX, screenY;
        if( fromLeft ) { screenX = x; }
        else           { screenX = xinfo.windowAttr.width - x; }
        if( fromTop )  { screenY = y; }
        else           { screenY = xinfo.windowAttr.height - y; }
        return mx >= screenX && mx < screenX+w && my >= screenY && my < screenY+h;
    }

    // isPressed(): True if button is pressed down, false otherwise
    bool isPressed() const { return pressed; }

    // press(): Marks the button as being pressed
    void press() { pressed = 1; }

    // release(): Marks the button as being unpressed
    void release() { pressed = 0; }

    // paint(): Draws the button to the given XInfo's window
    //   (NOTE: Does not clear the window first, or flush after!)
    void paint( XInfo&, PixmapArray& ) const;

    friend void pressButtonForLocation( XInfo&, PixmapArray&,
                                        MenuButton**, int numButtons,
                                        int x, int y, bool doPaint);
    friend Enum releaseButtonForLocation( XInfo&, PixmapArray&,
                                        MenuButton**, int numButtons,
                                        int x, int y, bool doPaint);
};

// paintAllButtons(): Run paint() on all buttons in an array
void paintAllButtons( XInfo&, PixmapArray&,
                      MenuButton** buttonArray, int numButtons );

// pressButtonForLocation(): Given the coordinates of a mouse click and array
//   of buttons, press the first button under the click and repaint it
void pressButtonForLocation( XInfo&, PixmapArray&,
                             MenuButton** buttonArray, int numButtons,
                             int x, int y, bool doPaint=true );

// releaseButtonForLocation(): Given the coordinates of a mouse click and
//   array of buttons, release all pressed buttons found and return the button
//   type of the first pressed button found (or the NONE button type if none
//   is found).
MenuButton::Enum releaseButtonForLocation(
        XInfo&, PixmapArray&,
        MenuButton** buttonArray, int numButtons,
        int x, int y, bool doPaint=true );

#endif
