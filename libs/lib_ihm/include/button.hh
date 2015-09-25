#ifndef __BUTTON_HH__
#define __BUTTON_HH__

#include <widget.hh>
#include <stdio.h>

class BUTTON : public WIDGET
{
public:
    BUTTON(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void), int mode, char* text);
    ~BUTTON();
private:
    int mode;
    char* text;
};

#endif
