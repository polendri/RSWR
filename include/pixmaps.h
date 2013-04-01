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
 * include/pixmaps.h
 *
 * Defines a structure into which all pixmaps used by the game can be loaded
 * and referenced
 */

#ifndef __PIXMAPS_H__
#define __PIXMAPS_H__

#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct XInfo;

// checkBitmapError(): Examines the presumed return code from a call to
//   XReadBitmapFile() and, if it represents an error, exit
void checkBitmapError( int );

class PixmapArray {
    static const int numPixmaps = 11;
    Pixmap pixmaps[numPixmaps];
public:
    enum Enum {
        BUTTON_QUIT = 0,
        BUTTON_QUIT_PRESSED,
        BUTTON_START,
        BUTTON_START_PRESSED,
        BUTTON_RESUME,
        BUTTON_RESUME_PRESSED,
        BUTTON_RETRY,
        BUTTON_RETRY_PRESSED,
        STALIN,
        TITLE,
        FAIL
    };
    PixmapArray( XInfo& xinfo );
    Pixmap& operator[] (int x) {
        return pixmaps[x];
    }
};

#endif
