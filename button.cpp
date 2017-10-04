#include <cmath>
#include "button.h"

using namespace std;

Button::Button(string content, int diameter, function<void(Button &)> handler)
    : content{content}, diameter{diameter}, handler{handler} {}

void Button::changeState(State state, int progress) {
    this->state = state;
    this->progress = progress;
}

void Button::enable() {
    disabled = false;
}

void Button::disable() {
    disabled = true;
}

void Button::updatePosition(int x, int y) {
    this->x = x;
    this->y = y;
}

// the CONTROLLER
void Button::mouseClick(int mx, int my) {
    if (disabled)
        return;

    float dist = sqrt(pow(mx - x, 2) + pow(my - y, 2));
    if (dist < diameter / 2) {
        changeState(State::SHRINKING);
        handler(*this);
    }
}
void Button::mouseMove(int mx, int my) {
    float dist = sqrt(pow(mx - x, 2) + pow(my - y, 2));
    hover = dist < diameter / 2;
}

// the VIEW  
void Button::paint(XInfo& xinfo) {
    if (state == State::SHRINKING) {
        const int curDiameter = diameter * (30 - progress) / 30;

        XSetForeground(xinfo.display, xinfo.gc, 
#ifdef ENHANCED
                xinfo.colors[2]
#else
                BlackPixel(xinfo.display, xinfo.screen)
#endif
                );
        XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc,
                x - diameter / 2,
                y - diameter / 2,
                diameter, diameter,
                0, 360 * 64);
        XSetForeground(xinfo.display, xinfo.gc, 
#ifdef ENHANCED
                xinfo.colors[3]
#else
                WhitePixel(xinfo.display, xinfo.screen)
#endif
                );
        XDrawArc(xinfo.display, xinfo.pixmap, xinfo.gc,
                x - curDiameter / 2,
                y - curDiameter / 2,
                curDiameter, curDiameter,
                0, 360 * 64);

        if (progress >= 29) {
            changeState(State::STATIC);
        }
    }
    else {
        int curY = state == State::BOBBING ?
            y + 10.0 * sin(progress / 10.0) :
            y;

        if (hover) {
            int diam = diameter + 8;
            XSetForeground(xinfo.display, xinfo.gc,
#ifdef ENHANCED
                xinfo.colors[0]
#else
                BlackPixel(xinfo.display, xinfo.screen)
#endif
                );
            XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc,
                    x - diam / 2,
                    curY - diam / 2,
                    diam, diam,
                    0, 360 * 64);
        }

        XSetForeground(xinfo.display, xinfo.gc, 
#ifdef ENHANCED
                xinfo.colors[1]
#else
                WhitePixel(xinfo.display, xinfo.screen)
#endif
                );
        XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc,
                x - diameter / 2,
                curY - diameter / 2,
                diameter, diameter,
                0, 360 * 64);

        XSetForeground(xinfo.display, xinfo.gc,BlackPixel(xinfo.display, xinfo.screen));
#ifdef ENHANCED
        XSetBackground(xinfo.display, xinfo.gc, xinfo.colors[1]);

#endif
        if (!hover) {
            XDrawArc(xinfo.display, xinfo.pixmap, xinfo.gc,
                    x - diameter / 2,
                    curY - diameter / 2,
                    diameter, diameter,
                    0, 360 * 64);
        }

        XDrawImageString(xinfo.display, xinfo.pixmap, xinfo.gc,
                x - 4, curY + 9, content.c_str(), content.length());
    }
    progress++;
}
