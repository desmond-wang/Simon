#include <cstdlib>
#include <cmath>
#include "controller.h"

int main(int argc, const char *argv[]) {
    int numButtons = 4;
    if (argc > 1) {
        numButtons = atoi(argv[1]);
    }
    numButtons = max(1, min(numButtons, 6));
    Controller ct{numButtons};
    ct.start();
}
