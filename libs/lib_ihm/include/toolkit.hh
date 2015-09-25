#ifndef __TOOLKIT_HH__
#define __TOOLKIT_HH__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <SDL/SDL.h>

#include <libBoreal.hh>

typedef struct Widget_List
{
    WIDGET* w;
    Widget_List* next;
}WIDGET_LIST;

typedef struct Callbacks_List
{
    void* (*callback)(char**);
    char* callback_name;
    Callbacks_List* next;
}CALLBACKS_LIST;

void callback_register(void* (*callback)(char**), char* name, CALLBACKS_LIST* cb);
void add(WIDGET* w, WIDGET_LIST* l);

class BorealAPP
{
private:
    WIDGET_LIST* widget_list;
    IHM* ihm;
    CALLBACKS_LIST* callbacks;
    
public:
    BorealAPP(char* xml_file, void (**func)(void*), void** args, IHM* ihm);
    BorealAPP(void (*func)(BorealAPP*, void*), void* arg, IHM* ihm);
    ~BorealAPP(void);

    void add_window(unsigned int window_control, char* theme_path, int width, int height, int x_pos, int y_pos, char* title, void(*opf)(void), void(*orf)(void), int focus);
    void add_button(int x_pos, int  y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), int mode, char* text);
    void add_scroll_bar(int x_pos, int y_pos, int width, int height, int orientation, void (*on_press)(void), void (*on_release)(void), int level);
    void add_progress_bar(int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), int ext_angle, int int_angle, int level);
    void add_text_zone(int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* font, int font_size, int font_style, char* text);
    void add_radio_button(int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* label, int grp, int value);
    void add_select_case(int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* label, int value);
    void add_label_text(int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* font, int font_size, int font_style, char* label);

    void* exec_call_back(char* func_name, char* args[]);
};

int parse(char* xml_file, void (**func)(void*), void** args);

#endif
