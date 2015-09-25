#ifndef __LABEL_TEXT_HH__
#define __LABEL_TEXT_HH__

#include <widget.hh>
#include <stdio.h>

class LABEL_TEXT : public WIDGET
{
private:
    char* font;
    int font_size;
    int font_style;
    char* label;
public:    
    LABEL_TEXT(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void),char* font, int fsize, int fstyle, char* label);
};

#endif
