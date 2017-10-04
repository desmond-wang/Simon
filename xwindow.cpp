#include "xwindow.h"

#include <chrono>
#include <iostream>
#include <thread>
#include "displayable.h"

using namespace std;
using namespace std::chrono_literals;

const int BUFFER_SIZE = 10;

XWindow::XWindow(int width, int height) {
    // open display
    xinfo.display = XOpenDisplay("");
    if (!xinfo.display) exit (-1);
    xinfo.screen = DefaultScreen(xinfo.display);
    xinfo.window = XCreateSimpleWindow(xinfo.display, DefaultRootWindow(xinfo.display),
            10, 10, width, height, 2,
            BlackPixel(xinfo.display, xinfo.screen), WhitePixel(xinfo.display, xinfo.screen));
    XSelectInput(xinfo.display, xinfo.window, ButtonPressMask | KeyPressMask | StructureNotifyMask | PointerMotionMask);
    XMapRaised(xinfo.display, xinfo.window);
    XFlush(xinfo.display);

    // give server 10ms to get set up before sending drawing commands
    this_thread::sleep_for(10ms);

    xinfo.gc = XCreateGC(xinfo.display, xinfo.window, 0, 0);       // create a graphics context
    XSetForeground(xinfo.display, xinfo.gc, BlackPixel(xinfo.display, xinfo.screen));
    XSetBackground(xinfo.display, xinfo.gc, WhitePixel(xinfo.display, xinfo.screen));

    //load a larger font
    XFontStruct *font;
    font = XLoadQueryFont(xinfo.display, "12x24");
    XSetFont(xinfo.display, xinfo.gc, font->fid);

    XGetWindowAttributes(xinfo.display, xinfo.window, &xinfo.w);

    int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
    xinfo.pixmap = XCreatePixmap(xinfo.display, xinfo.window, xinfo.w.width, xinfo.w.height, depth);

#ifdef ENHANCED
    XColor xcolor;
    Colormap cmap;
    const char color_vals[4][20]={"cyan", "violet", "light cyan", "midnight blue"};

    cmap=DefaultColormap(xinfo.display,xinfo.screen);
    for(int i=0; i< 4; ++i){
        XParseColor(xinfo.display, cmap, color_vals[i], &xcolor);
        XAllocColor(xinfo.display,cmap, &xcolor);
        xinfo.colors[i]=xcolor.pixel;
    }
#endif
}

XWindow::~XWindow() {
    XCloseDisplay(xinfo.display);
}

void XWindow::addElement(Displayable *element) {
    elements.emplace_back(element);
}

void XWindow::start() {
    while (true) {
        auto start = chrono::high_resolution_clock::now();

        update();

        auto end = chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> sleep = 1000ms / fps - (end - start);
        if (sleep > 0ms)
            this_thread::sleep_for(sleep);
    }
}

void XWindow::close() {
    XCloseDisplay(xinfo.display);
}

// Function to repaint a display list
void XWindow::repaint() {
    /* XClearWindow(xinfo.display, xinfo.window); */
    XSetForeground(xinfo.display, xinfo.gc, WhitePixel(xinfo.display, DefaultScreen(xinfo.display)));
    XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc, 0, 0, xinfo.w.width, xinfo.w.height);

    for (const auto &disp : elements) {
        disp->paint(xinfo);
    }

    XCopyArea(xinfo.display, xinfo.pixmap, xinfo.window, xinfo.gc, 0, 0, xinfo.w.width, xinfo.w.height, 0, 0);
    XFlush(xinfo.display);
}

void XWindow::update() {
    while (XPending(xinfo.display)) {
        XEvent event;
        XNextEvent(xinfo.display, &event);

        switch (event.type) {
            case ButtonPress:
                for(auto &i : elements) {
                    i->mouseClick(event.xbutton.x, event.xbutton.y);
                }
                break;

            case ConfigureNotify: // resize
                {
                    XFreePixmap(xinfo.display, xinfo.pixmap);

                    XGetWindowAttributes(xinfo.display, xinfo.window, &xinfo.w);

                    int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
                    xinfo.pixmap = XCreatePixmap(xinfo.display, xinfo.window, xinfo.w.width, xinfo.w.height, depth);
                }

                for(auto &i : elements) {
                    i->resize(event.xconfigure.width, event.xconfigure.height);
                }
                break;

            case MotionNotify: // mouse
                for(auto &i : elements){
                    i->mouseMove(event.xmotion.x, event.xmotion.y);
                }
                break;

            case KeyPress:
                KeySym key;
                char buffer[BUFFER_SIZE];
                int len = XLookupString(reinterpret_cast<XKeyEvent*>(&event), buffer, BUFFER_SIZE, &key, 0);
                string text(buffer, len);
                for (auto &i : elements) {
                    i->keyPress(text);
                }
                break;
        }
    }

    repaint();
}
