#include <button.hh>

BUTTON
::
BUTTON(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void), int mode, char* text)
:
WIDGET(x,y,w,h,opf,orf)
{
    this->mode = mode;
    this->text = text;
}

