#ifndef SIMON_DISPLAYABLE_H
#define SIMON_DISPLAYABLE_H

#include <string>
#include "xinfo.h"

class Displayable {
    public:
        virtual void paint(XInfo& xinfo) = 0;
        virtual void mouseClick(int mx, int my) {
            (void)mx;
            (void)my;
        }
        virtual void resize(int width, int height) {
            (void)width;
            (void)height;
        }
        virtual void mouseMove(int mx, int my) {
            (void)mx;
            (void)my;
        }
        virtual void keyPress(std::string key) {
            (void)key;
        }
};

#endif
