#include <select_case.hh>

SELECT_CASE
::
SELECT_CASE(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void), char* label, int value)
:
WIDGET(x,y,w,h,opf,orf)
{
    this->label = label;
    this->value = value;
}
