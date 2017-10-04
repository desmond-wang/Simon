#ifndef SIMON_XINFO_H
#define SIMON_XINFO_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct XInfo {
	Display* display;
	Window window;
    Pixmap pixmap;
    XWindowAttributes w;
	int screen;
	GC gc;
#ifdef ENHANCED
    unsigned long colors[4];
#endif
};

#endif
