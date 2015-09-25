#include <text_zone.hh>

TEXT_ZONE
::
TEXT_ZONE(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void), char* f, int fsize, int fstyle, char* text)
:
WIDGET(x,y,w,h,opf,orf)
{
    this->font = f;
    this->font_size = fsize;
    this->font_style = fstyle;
    this->text = text;
}
