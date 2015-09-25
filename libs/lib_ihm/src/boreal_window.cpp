#include <boreal_window.hh>

BOREAL_WINDOW
::
BOREAL_WINDOW(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void), int wc, char* tp, char* title, int fo)
:
WIDGET(x,y,w,h,opf,orf)
{
    this->window_control = wc;
    this->theme_path = tp;
    this->title = title;
    this->focus = fo;
}
