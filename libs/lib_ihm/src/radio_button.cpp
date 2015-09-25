#include <radio_button.hh>

RADIO_BUTTON
::
RADIO_BUTTON(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void), char* label, int grp, int value)
:
WIDGET(x,y,w,h,opf,orf)
{
    this->label = label;
    this->grp = grp;
    this->value = value;
}
