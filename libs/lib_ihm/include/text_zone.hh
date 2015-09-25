#ifndef __TEXT_ZONE_HH__
#define __TEXT_ZONE_HH__

#include <widget.hh>
#include <stdio.h>

class TEXT_ZONE : public WIDGET
{
private:
    char* font;
    int font_size;
    int font_style;
    char* text;
public:        
    TEXT_ZONE(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void),char* f, int fsize, int fstyle, char* text);
};

#endif
