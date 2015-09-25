#ifndef __PROGRESS_BAR_HH__
#define __PROGRESS_BAR_HH__

#include <widget.hh>
#include <stdio.h>

class PROGRESS_BAR : public WIDGET
{
private:
    int ext_angle;
    int int_angle;
    int level;
public:
    PROGRESS_BAR(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void),int ea, int ia, int l);
};

#endif
