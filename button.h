#ifndef SIMON_BUTTON_H
#define SIMON_BUTTON_H

#include <string>
#include <functional>
#include "displayable.h"

class Button : public Displayable {
    public:
        enum class State{
            BOBBING,
            SHRINKING,
            STATIC
        };

        Button(std::string content, int diameter, std::function<void(Button &)> handler);

        void changeState(State state, int progress = 0);

        void enable();

        void disable();

        void updatePosition(int x, int y);

        // the CONTROLLER
        void mouseClick(int mx, int my) override;

        void mouseMove(int mx, int my) override;

        // the VIEW  
        void paint(XInfo& xinfo) override;

    private:
        // VIEW "essential geometry"
        std::string content;
        int num; // same as name, just esay to compare
        int x;
        int y;
        int diameter;
        // clicked animation diameter
        int progress;
        bool hover = false;
        bool disabled = false;
        // toggle event callback
        std::function<void(Button &)> handler;
        State state = State::STATIC;
};

#endif
