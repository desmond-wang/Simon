#include "controller.h"

#include <cassert>
#include <thread>

using namespace std;
using namespace std::chrono_literals;

const int DIAMETER = 100;

Controller::Controller(int numButtons)
    : simon{numButtons}, scoreText{"N/A", 30, 50}, messageText{"hello world", 30, 100}

#ifdef ENHANCED
, historicalMsg{ "Highscore:",155, 680},
    highScore{"0", 200, 700} 
#endif
{
    buttons.reserve(numButtons);
    for (int i = 0; i < numButtons; ++i) {
        auto content = to_string(i + 1);
        Button button{content, DIAMETER, [this, i](Button &) { buttonClicked(i); }};
        buttons.emplace_back(button);
    }

    for (auto &button : buttons) {
        window.addElement(&button);
    }
    window.addElement(&scoreText);
    window.addElement(&messageText);
#ifdef ENHANCED
    window.addElement(&historicalMsg);
    window.addElement(&highScore);
#endif
    window.addElement(this);

    updateState(simon.getState());
}

void Controller::start() {
    window.start();
}

void Controller::paint(XInfo& xinfo) {
    (void)xinfo;

    if (delay > 0) {
        --delay;
        return;
    }

    auto state = simon.getState();
    if (state != lastState) {
        updateState(state);
    }
    else {
        if (state == Simon::COMPUTER) {
            int index = simon.nextButton();
            buttons[index].changeState(Button::State::SHRINKING);
            delay += 45;
        }
        else if (state == Simon::HUMAN) {
            for (auto &button : buttons) {
                button.enable();
            }
        }
    }

    updateScore();
}

void Controller::buttonClicked(int index) {
    simon.verifyButton(index);
    for (auto &button : buttons) {
        button.disable();
    }
    delay += 45;
}

void Controller::resize(int width, int height) {
    int numButtons = simon.getNumButtons();
    int spacing = (width - numButtons * DIAMETER) / (numButtons + 1);

    int y = height / 2;
#ifdef ENHANCED
    historicalMsg.updatePositions(width/2-55, height/12*10);
    highScore.updatePositions(width/2 ,height/12*11);

#endif
    for (int i = 0; i < numButtons; ++i) {
        buttons[i].updatePosition((i + 1) * (spacing + DIAMETER) - DIAMETER / 2, y);
    }
}

void Controller::keyPress(string key) {
    if (key == "q") {
        cout << "Terminated normally." << endl;
        window.close();
        exit(0);
    }

    if (lastState == Simon::START && key == " ") {
        simon.newRound();
    }
    else if ((lastState == Simon::WIN || lastState == Simon::LOSE) && key == "\r") {
        simon.newRound();
    }
}

void Controller::updateState(Simon::State state) {
    switch (state) {
        // will only be in this state right after Simon object is contructed
        case Simon::START:
            messageText.updateText("Press SPACE to play.");
            bobButtons();
            break;

        case Simon::COMPUTER:
            messageText.updateText("Watch what I do ...");
            for (auto &button : buttons) {
                button.changeState(Button::State::STATIC);
            }
            delay += 60;
            break;

        case Simon::HUMAN:
            messageText.updateText("Your turn ...");
            for (auto &button : buttons) {
                button.enable();
            }
            break;

        case Simon::WIN:
        case Simon::LOSE: {
                if(state == Simon::WIN)
                    messageText.updateText("You won! Press ENTER to continue.");
                else
                    messageText.updateText("You lose. Press ENTER to play again.");
                bobButtons();
#ifdef ENHANCED
                int curScore = simon.getScore();
                if(historicalHigh < curScore){
                    historicalHigh = curScore;
                    highScore.updateText(to_string(curScore));
                }
#endif
                break;
            }

        default:
            assert(false && "unreachable");
            break;
    }

    lastState = state;
}

void Controller::bobButtons() {
    int progress = 0;
    for (auto &button : buttons) {
        button.disable();
        button.changeState(Button::State::BOBBING, progress);
        progress -= 10;
    }
}

void Controller::updateScore() {
    scoreText.updateText(to_string(simon.getScore()));
}
