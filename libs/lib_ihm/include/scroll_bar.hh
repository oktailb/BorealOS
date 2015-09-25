#ifndef __SCROLL_BAR_HH__
#define __SCROLL_BAR_HH__

#include <widget.hh>
#include <stdio.h>

class SCROLL_BAR : public WIDGET
{
public:
    SCROLL_BAR(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void),int o, int l);
    void set_orientation(int o);
    void set_level(int l);
private:
    int orientation;
    int level;
    
};

#endif
