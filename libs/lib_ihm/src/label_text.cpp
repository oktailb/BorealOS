#include <label_text.hh>

LABEL_TEXT
::
LABEL_TEXT(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void), char* font, int fsize, int fstyle, char* label)
:
WIDGET(x,y,w,h,opf,orf)
{
    this->font = font;
    this->font_size = fsize;
    this->font_style = fstyle;
    this->label = label;
}
