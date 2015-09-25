#ifndef __RADIO_BUTTON_H__
#define __RADIO_BUTTON_H__

#include <widget.hh>
#include <stdio.h>

class RADIO_BUTTON : public WIDGET
{
private:
    char* label;
    int grp;
    int value;
public:
    RADIO_BUTTON(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void), char* label, int grp, int value);
};

#endif
