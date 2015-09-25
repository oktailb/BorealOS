#ifndef __TKGUI_H__
#define __TKGUI_H__

#define TYPE_BUTTON          1
#define TYPE_SCROLL_BAR      2
#define TYPE_PROGRESS_BAR    4
#define TYPE_TEXT_ZONE       8
#define TYPE_RADIO_BUTTON    16
#define TYPE_SELECT_CASE     32
#define TYPE_LABEL_TEXT      64

#define L_CLOSE           1
#define L_MAXIMISE        2
#define L_MINIMISE        4
#define L_MENU            8

#define R_CLOSE           16
#define R_MAXIMISE        32
#define R_MINIMISE        64
#define R_MENU            128

#define DESKTOP           1073741824

int nb_windows;

typedef struct
{
        int type;
        int x_pos;
        int y_pos;
        int height;
        int width;
        int mode;
        void (*on_press_func)(void);
        void (*on_release_func)(void);
        char* text;

}BUTTON;

typedef struct
{
        int type;
        int x_pos;
        int y_pos;
        int width;
        int height;
        void (*on_press_func)(void);
        void (*on_release_func)(void);
        int orientation;
        int level;
        
}SCROLL_BAR;

typedef struct
{
        int type;
        int x_pos;
        int y_pos;
        int width;
        int height;
        void (*on_press_func)(void);
        void (*on_release_func)(void);
        int ext_angle;
        int int_angle;
        int level;
        
}PROGRESS_BAR;

typedef struct
{
        int type;
        int x_pos;
        int y_pos;
        int width;
        int height;
        void (*on_press_func)(void);
        void (*on_release_func)(void);
        char* font;
        int font_size;
        int font_style;
        char* text;
        
}TEXT_ZONE;

typedef struct
{
        int type;
        int x_pos;
        int y_pos;
        int width;
        int height;
        void (*on_press_func)(void);
        void (*on_release_func)(void);
        char* label;
        int grp;
        int value;
        
}RADIO_BUTTON;

typedef struct
{
        int type;
        int x_pos;
        int y_pos;
        int width;
        int height;
        void (*on_press_func)(void);
        void (*on_release_func)(void);
        char* label;
        int value;
        
}SELECT_CASE;

typedef struct
{
        int type;
        int x_pos;
        int y_pos;
        int width;
        int height;
        void (*on_press_func)(void);
        void (*on_release_func)(void);
        char* font;
        int font_size;
        int font_style;
        char* label;
        
}LABEL_TEXT;

typedef struct
{
        int type;
        int x_pos;
        int y_pos;
        int width;
        int height;
        void (*on_press_func)(void);
        void (*on_release_func)(void);

}WIDGET_SELECT;

typedef union
{
        WIDGET_SELECT   widget_select;
        BUTTON          button;
        SCROLL_BAR      scroll_bar;
        PROGRESS_BAR    progress_bar;
        TEXT_ZONE       text_zone;
        RADIO_BUTTON    radio_button;
        SELECT_CASE     select_case;
        LABEL_TEXT      label_text;
        
}WIDGET;

typedef struct
{

        int window_control;
        char* theme_path;
        char* title;
        int height;
        int width;
        int x_pos;
        int y_pos;
        int focus;
        
        WIDGET** widgets;
        int nb_widgets;

}WINDOW;

WINDOW** windows_list;


int init_tkgui(int w, int h, int bpp);
void add_window(WINDOW* w, unsigned int window_control, char* theme_path, int width, int height, int x_pos, int y_pos, char* title);
void add_button(WINDOW* w, int x_pos, int y_pos, int width, int height, int mode, void (*on_press)(void), void (*on_release)(void), char* text);
void add_scroll_bar(WINDOW* w, int x_pos, int y_pos, int width, int height, int orientation, void (*on_press)(void), void (*on_release)(void), int level);
void add_progress_bar(WINDOW* w, int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), int ext_angle, int int_angle, int level);
void add_text_zone(WINDOW* w, int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* font, int font_size, int font_style, char* text);
void add_radio_button(WINDOW* w, int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* label, int grp, int value);
void add_select_case(WINDOW* w, int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* label, int value);
void add_label_text(WINDOW* w, int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* font, int font_size, int font_style, char* label);
void draw_button(BUTTON w);
void draw_scroll_bar(SCROLL_BAR w);
void draw_progress_bar(PROGRESS_BAR w);
void draw_text_zone(TEXT_ZONE w);
void draw_radio_button(RADIO_BUTTON w);
void draw_select_case(SELECT_CASE w);
void draw_label_text(LABEL_TEXT w);
void draw_window(WINDOW* w);

#endif

