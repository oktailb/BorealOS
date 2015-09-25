#ifndef __SELECT_CASE_HH__
#define __SELECT_CASE_HH__

#include <widget.hh>
#include <stdio.h>

class SELECT_CASE : public WIDGET
{
private:
    char* label;
    int value;
public:
    SELECT_CASE(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void),char* label, int value);
};

#endif
