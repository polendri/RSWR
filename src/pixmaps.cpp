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
 * src/pixmaps.h
 *
 * Implementation of include/pixmaps.h
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "error.h"
#include "pixmaps.h"
#include "state.h"

// checkBitmapError(): Examines the presumed return code from a call to
//   XReadBitmapFile() and, if it represents an error, exit
void checkBitmapError( int rc ) {
    switch (rc) {
        case BitmapOpenFailed:
            errorExit("Could not open X Bitmap file.");
            break;
        case BitmapFileInvalid:
            errorExit("File opened which does not contain a valid X Bitmap.");
            break;
        case BitmapNoMemory:
            errorExit("Not enough memory to open X Bitmap.");
            break;
    }
}

// readBitmap(): Reads a specified bitmap into the pixmap slot specified by dest
void readBitmap( XInfo& xinfo, Pixmap* pixmaps, std::string fileName,
                 PixmapArray::Enum dest ) {
    Window rootWin = DefaultRootWindow( xinfo.display );
    unsigned int width, height;
    int x, y, rc;

    rc = XReadBitmapFile( xinfo.display, rootWin,
                          fileName.c_str(),
                          &width, &height,
                          &pixmaps[ dest ],
                          &x, &y );
    checkBitmapError( rc );
}

PixmapArray::PixmapArray( XInfo& xinfo ) {
    readBitmap( xinfo, pixmaps, "res/button_quit.xbm",
                PixmapArray::BUTTON_QUIT );
    readBitmap( xinfo, pixmaps, "res/button_quit_pressed.xbm",
                PixmapArray::BUTTON_QUIT_PRESSED );
    readBitmap( xinfo, pixmaps, "res/button_start.xbm",
                PixmapArray::BUTTON_START );
    readBitmap( xinfo, pixmaps, "res/button_start_pressed.xbm",
                PixmapArray::BUTTON_START_PRESSED );
    readBitmap( xinfo, pixmaps, "res/button_resume.xbm",
                PixmapArray::BUTTON_RESUME );
    readBitmap( xinfo, pixmaps, "res/button_resume_pressed.xbm",
                PixmapArray::BUTTON_RESUME_PRESSED );
    readBitmap( xinfo, pixmaps, "res/button_retry.xbm",
                PixmapArray::BUTTON_RETRY );
    readBitmap( xinfo, pixmaps, "res/button_retry_pressed.xbm",
                PixmapArray::BUTTON_RETRY_PRESSED );
    readBitmap( xinfo, pixmaps, "res/stalin.xbm",
                PixmapArray::STALIN );
    readBitmap( xinfo, pixmaps, "res/title.xbm",
                PixmapArray::TITLE );
    readBitmap( xinfo, pixmaps, "res/failure.xbm",
                PixmapArray::FAIL );
}
