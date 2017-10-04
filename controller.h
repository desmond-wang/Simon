#ifndef SIMON_CONTROLLER_H
#define SIMON_CONTROLLER_H

#include <vector>
#include <chrono>
#include "button.h"
#include "displayable.h"
#include "simon.h"
#include "text.h"
#include "xwindow.h"

class Controller : public Displayable {
    public:
        explicit Controller(int numButtons);
        void start();
        void paint(XInfo& xinfo) override;
        void resize(int width, int height) override;
        void keyPress(std::string key) override;

    private:
        Simon simon;
        XWindow window;
        std::vector<Button> buttons;
        Text scoreText;
        Text messageText;
#ifdef ENHANCED
        Text historicalMsg;
        Text highScore;
        int historicalHigh = 0;
#endif
        Simon::State lastState;
        int delay = 0;

        void buttonClicked(int index);
        void updateState(Simon::State state);
        void bobButtons();
        void updateScore();
};

#endif
