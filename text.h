#ifndef SIMON_TEXT_H
#define SIMON_TEXT_H

#include <string>
#include "displayable.h"

class Text : public Displayable {
    public:
        Text(std::string s, int x, int y);

        void updateText(std::string s);
        void updatePositions(int x, int y);
        void paint(XInfo& xinfo) override;

    private:
        std::string s; // string to show
        int x;
        int y;
};

#endif
