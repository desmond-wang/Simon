#ifndef SIMON_XWINDOW_H
#define SIMON_XWINDOW_H

#include <vector>
#include "xinfo.h"

class Displayable;

class XWindow {
    public:
        explicit XWindow(int width = 800, int height = 400);
        ~XWindow();
        void addElement(Displayable *element);
        void start();
        void close();

    private:
        void update();
        void repaint();

        int fps = 60;
        XInfo xinfo;
        std::vector<Displayable *> elements;
};

#endif
