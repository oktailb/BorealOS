#include <progress_bar.hh>

PROGRESS_BAR
::
PROGRESS_BAR(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void), int ea, int ia, int l)
:
WIDGET(x,y,w,h,opf,orf)
{
    this->ext_angle = ea;
    this->int_angle = ia;
    this->level = l;
}
