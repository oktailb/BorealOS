#include <scroll_bar.hh>

SCROLL_BAR
::
SCROLL_BAR(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void), int o, int l)
:
WIDGET(x,y,w,h,opf,orf)
{
    this->orientation = o;
    this->level = l;
}
