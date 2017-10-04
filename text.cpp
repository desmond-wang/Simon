#include "text.h"

using namespace std;

Text::Text(string s, int x, int y) : s{s}, x{x}, y{y} {}

void Text::updateText(string s) {
    this->s = s;
}
void Text::updatePositions(int x,int y){
    this->x = x;
    this->y = y;
}
void Text::paint(XInfo& xinfo) {
#ifdef ENHANCED
    XSetBackground(xinfo.display, xinfo.gc,WhitePixel(xinfo.display, xinfo.screen));
#endif
    XDrawImageString(xinfo.display, xinfo.pixmap, xinfo.gc,
            this->x, this->y, this->s.c_str(), this->s.length());
}
