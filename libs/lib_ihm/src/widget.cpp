#include <widget.hh>


WIDGET::
WIDGET(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void))
{
    this->x_pos = x;
    this->y_pos = y;
    this->height = h;
    this->width = w;
    this->on_press_func = opf;
    this->on_release_func = orf;
}

void WIDGET::set_x_pos(int x)
{
    x_pos = x;
}

void WIDGET::set_y_pos(int y)
{
    y_pos = y;
}

void WIDGET::set_width(int w)
{
    width = w;
}

void WIDGET::set_height(int h)
{
    height = h;
}

void WIDGET::set_draw(void (*f)(void*))
{
    draw = f;
}

void WIDGET::exec_on_press_func(void)
{
    on_press_func();
}

void WIDGET::exec_on_release_func(void)
{
    on_release_func();
}
