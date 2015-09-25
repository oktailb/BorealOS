/* Tk GUI : GUI C sous Allegro */

#include <allegro.h>
#include <stdio.h>
#include "tkgui.h"

int DESKTOP_WIDTH;
int DESKTOP_HEIGHT;
int DESKTOP_BPP;


void move(void)
{

}

void close(void)
{

}

void max(void)
{

}

void min(void)
{

}

void menu(void)
{

}

void nop(void)
{

}

void radio_button_select(WINDOW* w, int grp, char* label)
{
        int i=0;
        for(i=0;i<w->nb_widgets;i++)
        {
                if(w->widgets[i]->widget_select.type == TYPE_RADIO_BUTTON)
                {
                        if(w->widgets[i]->radio_button.grp == grp)
                        {
                                if(strcmp(w->widgets[i]->radio_button.label, label))
                                {
                                        w->widgets[i]->radio_button.value=1;
                                }
                                else
                                {
                                        w->widgets[i]->radio_button.value=0;
                                }
                        }
                }
        }
}

void add_button(WINDOW* w, int x_pos, int y_pos, int width, int height, int mode, void (*on_press)(void), void (*on_release)(void), char* text)
{
        WIDGET b;
        b.button.type           =       TYPE_BUTTON;
        b.button.x_pos          =       x_pos;
        b.button.y_pos          =       y_pos;
        b.button.height         =       height;
        b.button.width          =       width;
        b.button.mode           =       mode;
        b.button.on_press_func  =       on_press;
        b.button.on_release_func=       on_release;
        b.button.text           =       (char*) malloc((strlen(text)+1)*sizeof(char));
        strcpy(b.button.text    ,       text);


        w->widgets[w->nb_widgets] = (WIDGET*) malloc(sizeof(WIDGET));
        memcpy(w->widgets[w->nb_widgets], &b, sizeof(WIDGET));
        w->nb_widgets++;
}

void add_scroll_bar(WINDOW* w, int x_pos, int y_pos, int width, int height, int orientation, void (*on_press)(void), void (*on_release)(void), int level)
{
        WIDGET b;
        b.scroll_bar.type       =       TYPE_SCROLL_BAR;
        b.scroll_bar.x_pos      =       x_pos;
        b.scroll_bar.y_pos      =       y_pos;
        b.scroll_bar.width      =       width;
        b.scroll_bar.height     =       height;
        b.scroll_bar.orientation=       orientation;
        b.scroll_bar.level      =       level;
        b.scroll_bar.on_press_func  =       on_press;
        b.scroll_bar.on_release_func=       on_release;


        w->widgets[w->nb_widgets] = (WIDGET*) malloc(sizeof(WIDGET));
        memcpy(w->widgets[w->nb_widgets], &b, sizeof(WIDGET));
        w->nb_widgets++;


}

void add_progress_bar(WINDOW* w, int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), int ext_angle, int int_angle, int level)
{
        WIDGET b;
        b.progress_bar.type       =       TYPE_PROGRESS_BAR;
        b.progress_bar.x_pos      =       x_pos;
        b.progress_bar.y_pos      =       y_pos;
        b.progress_bar.height     =       height;
        b.progress_bar.width      =       width;
        b.progress_bar.ext_angle  =       ext_angle;
        b.progress_bar.int_angle  =       int_angle;
        b.progress_bar.level      =       level;
        b.progress_bar.on_press_func  =       on_press;
        b.progress_bar.on_release_func=       on_release;
        

        w->widgets[w->nb_widgets] = (WIDGET*) malloc(sizeof(WIDGET));
        memcpy(w->widgets[w->nb_widgets], &b, sizeof(WIDGET));
        w->nb_widgets++;


}

void add_text_zone(WINDOW* w, int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* font, int font_size, int font_style, char* text)
{
        WIDGET b;
        b.text_zone.type       =       TYPE_TEXT_ZONE;
        b.text_zone.x_pos      =       x_pos;
        b.text_zone.y_pos      =       y_pos;
        b.text_zone.height     =       height;
        b.text_zone.width      =       width;
        b.text_zone.font_size  =       font_size;
        b.text_zone.font_style =       font_style;
        strcpy(b.text_zone.font,       font);
        strcpy(b.text_zone.text,       text);
        b.text_zone.on_press_func  =       on_press;
        b.text_zone.on_release_func=       on_release;


        w->widgets[w->nb_widgets] = (WIDGET*) malloc(sizeof(WIDGET));
        memcpy(w->widgets[w->nb_widgets], &b, sizeof(WIDGET));
        w->nb_widgets++;

}

void add_radio_button(WINDOW* w, int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* label, int grp, int value)
{
        WIDGET b;
        b.radio_button.type       =       TYPE_RADIO_BUTTON;
        b.radio_button.x_pos      =       x_pos;
        b.radio_button.y_pos      =       y_pos;
        b.radio_button.width      =       width;
        b.radio_button.height     =       height;
        strcpy(b.radio_button.label,      label);
        b.radio_button.on_press_func  =       on_press;
        b.radio_button.on_release_func=       on_release;
        b.radio_button.grp        =       grp;
        if(value)
        {
                radio_button_select(w, grp, label);
        }
        else
        {
                value=0;
        }
        

        w->widgets[w->nb_widgets] = (WIDGET*) malloc(sizeof(WIDGET));
        memcpy(w->widgets[w->nb_widgets], &b, sizeof(WIDGET));
        w->nb_widgets++;

}

void add_select_case(WINDOW* w, int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* label, int value)
{
        WIDGET b;
        b.select_case.type       =       TYPE_SELECT_CASE;
        b.select_case.x_pos      =       x_pos;
        b.select_case.y_pos      =       y_pos;
        b.select_case.width      =       width;
        b.select_case.height     =       height;
        strcpy(b.select_case.label,      label);
        b.select_case.on_press_func  =       on_press;
        b.select_case.on_release_func=       on_release;
        b.select_case.value      =           value;
        

        w->widgets[w->nb_widgets] = (WIDGET*) malloc(sizeof(WIDGET));
        memcpy(w->widgets[w->nb_widgets], &b, sizeof(WIDGET));
        w->nb_widgets++;


}

void add_label_text(WINDOW* w, int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* font, int font_size, int font_style, char* label)
{
        WIDGET b;
        b.label_text.type       =       TYPE_LABEL_TEXT;
        b.label_text.x_pos      =       x_pos;
        b.label_text.y_pos      =       y_pos;
        b.label_text.width      =       width;
        b.label_text.height     =       height;
        b.label_text.font_size  =       font_size;
        b.label_text.font_style =       font_style;
        strcpy(b.label_text.font,       font);
        strcpy(b.label_text.label,      label);


        w->widgets[w->nb_widgets] = (WIDGET*) malloc(sizeof(WIDGET));
        memcpy(w->widgets[w->nb_widgets], &b, sizeof(WIDGET));
        w->nb_widgets++;


}



void add_window(WINDOW* w, unsigned int window_control, char* theme_path, int width, int height, int x_pos, int y_pos, char* title)
{
        char* tmp;
        int r_pos=x_pos+width-20, l_pos=x_pos+4;
        
        w->nb_widgets=0;
        w->widgets = (WIDGET **) malloc(sizeof(WIDGET*));
        
        w->window_control        =       window_control;
        w->height                =       height;
        w->width                 =       width;
        w->x_pos                 =       x_pos;
        w->y_pos                 =       y_pos;

        w->theme_path            =       (char*) malloc((strlen(theme_path)+1)*sizeof(char));
        strcpy(w->theme_path     ,       theme_path);
        w->title                 =       (char*) malloc((strlen(title)+1)*sizeof(char));
        strcpy(w->title          ,       title);

        sprintf(tmp,"%s\\title_button.bmp",theme_path);
        add_button(w,x_pos,y_pos,width,24,1,move,nop,tmp);
        
        if(L_CLOSE&window_control)
        {
                sprintf(tmp,"%s\\close_button.bmp",theme_path);
                add_button(w,l_pos,y_pos+4,16,16,1,nop,close,tmp);
                l_pos+=20;

        }

        if(L_MAXIMISE&window_control)
        {
                sprintf(tmp,"%s\\maximise_button.bmp",theme_path);
                add_button(w,l_pos,y_pos+4,16,16,1,nop,max,tmp);
                l_pos+=20;

        }
        
        if(L_MINIMISE&window_control)
        {
                sprintf(tmp,"%s\\minimise_button.bmp",theme_path);
                add_button(w,l_pos,y_pos+4,16,16,1,nop,min,tmp);
                l_pos+=20;

        }
        
        if(L_MENU&window_control)
        {
                sprintf(tmp,"%s\\menu_button.bmp",theme_path);
                add_button(w,l_pos,y_pos+4,16,16,1,nop,menu,tmp);
                l_pos+=20;

        }
        
        if(R_CLOSE&window_control)
        {
                sprintf(tmp,"%s\\close_button.bmp",theme_path);
                add_button(w,r_pos,y_pos+4,16,16,1,nop,close,tmp);
                r_pos-=20;

        }
        
        if(R_MAXIMISE&window_control)
        {
                sprintf(tmp,"%s\\maximise_button.bmp",theme_path);
                add_button(w,r_pos,y_pos+4,16,16,1,nop,max,tmp);
                r_pos-=20;

        }

        if(R_MINIMISE&window_control)
        {
                sprintf(tmp,"%s\\maximise_button.bmp",theme_path);
                add_button(w,r_pos,y_pos+4,16,16,1,nop,min,tmp);
                r_pos-=20;

        }

        if(R_MENU&window_control)
        {
                sprintf(tmp,"%s\\menu_button.bmp",theme_path);
                add_button(w,r_pos,y_pos+4,16,16,1,nop,menu,tmp);
                r_pos-=20;

        }

        realloc( (WINDOW**) windows_list, (nb_windows+1)*sizeof(WINDOW*) );

        windows_list[nb_windows] = (WINDOW*) malloc(sizeof(WINDOW));

        memcpy(windows_list[nb_windows],w, sizeof(WINDOW));

        nb_windows++;

}


void draw_button(BUTTON w)
{
        line(screen,w.x_pos,w.y_pos,w.x_pos,w.y_pos+w.height, 0x0F0F);
        line(screen,w.x_pos,w.y_pos,w.x_pos+w.width,w.y_pos, 0x0F0F);
        line(screen,w.x_pos+w.width,w.y_pos+w.height,w.x_pos,w.y_pos+w.height, 0x0F0F);
        line(screen,w.x_pos+w.width,w.y_pos+w.height,w.x_pos+w.width,w.y_pos, 0x0F0F);

}

void draw_scroll_bar(SCROLL_BAR w)
{
        line(screen,w.x_pos,w.y_pos,w.x_pos,w.y_pos+w.height, 0x0FF0);
        line(screen,w.x_pos,w.y_pos,w.x_pos+w.width,w.y_pos, 0x0FF0);
        line(screen,w.x_pos+w.width,w.y_pos+w.height,w.x_pos,w.y_pos+w.height, 0x0FF0);
        line(screen,w.x_pos+w.width,w.y_pos+w.height,w.x_pos+w.width,w.y_pos, 0x0FF0);

}

void draw_progress_bar(PROGRESS_BAR w)
{
        line(screen,w.x_pos,w.y_pos,w.x_pos,w.y_pos+w.height, 0xF0F0);
        line(screen,w.x_pos,w.y_pos,w.x_pos+w.width,w.y_pos, 0xF0F0);
        line(screen,w.x_pos+w.width,w.y_pos+w.height,w.x_pos,w.y_pos+w.height, 0xF0F0);
        line(screen,w.x_pos+w.width,w.y_pos+w.height,w.x_pos+w.width,w.y_pos, 0xF0F0);

}

void draw_text_zone(TEXT_ZONE w)
{
        line(screen,w.x_pos,w.y_pos,w.x_pos,w.y_pos+w.height, 0xF0F0);
        line(screen,w.x_pos,w.y_pos,w.x_pos+w.width,w.y_pos, 0xF0F0);
        line(screen,w.x_pos+w.width,w.y_pos+w.height,w.x_pos,w.y_pos+w.height, 0xF0F0);
        line(screen,w.x_pos+w.width,w.y_pos+w.height,w.x_pos+w.width,w.y_pos, 0xF0F0);

}

void draw_radio_button(RADIO_BUTTON w)
{
        line(screen,w.x_pos,w.y_pos,w.x_pos,w.y_pos+w.height, 0xF0F0);
        line(screen,w.x_pos,w.y_pos,w.x_pos+w.width,w.y_pos, 0xF0F0);
        line(screen,w.x_pos+w.width,w.y_pos+w.height,w.x_pos,w.y_pos+w.height, 0xF0F0);
        line(screen,w.x_pos+w.width,w.y_pos+w.height,w.x_pos+w.width,w.y_pos, 0xF0F0);

}

void draw_select_case(SELECT_CASE w)
{
        line(screen,w.x_pos,w.y_pos,w.x_pos,w.y_pos+w.height, 0xF0F0);
        line(screen,w.x_pos,w.y_pos,w.x_pos+w.width,w.y_pos, 0xF0F0);
        line(screen,w.x_pos+w.width,w.y_pos+w.height,w.x_pos,w.y_pos+w.height, 0xF0F0);
        line(screen,w.x_pos+w.width,w.y_pos+w.height,w.x_pos+w.width,w.y_pos, 0xF0F0);

}

void draw_label_text(LABEL_TEXT w)
{
        line(screen,w.x_pos,w.y_pos,w.x_pos,w.y_pos+w.height, 0xF0F0);
        line(screen,w.x_pos,w.y_pos,w.x_pos+w.width,w.y_pos, 0xF0F0);
        line(screen,w.x_pos+w.width,w.y_pos+w.height,w.x_pos,w.y_pos+w.height, 0xF0F0);
        line(screen,w.x_pos+w.width,w.y_pos+w.height,w.x_pos+w.width,w.y_pos, 0xF0F0);

}

void draw_window(WINDOW* w)
{
        int i;

        line(screen,w->x_pos,w->y_pos,w->x_pos,w->y_pos+w->height, 0xF0F0);
        line(screen,w->x_pos,w->y_pos,w->x_pos+w->width,w->y_pos, 0xF0F0);
        line(screen,w->x_pos+w->width,w->y_pos+w->height,w->x_pos,w->y_pos+w->height, 0xF0F0);
        line(screen,w->x_pos+w->width,w->y_pos+w->height,w->x_pos+w->width,w->y_pos, 0xF0F0);
        
        for(i=0;i<w->nb_widgets;i++)
        {
                switch(w->widgets[i]->widget_select.type)
                {
                        case TYPE_BUTTON:
                             draw_button(w->widgets[i]->button);
                             break;
                        case TYPE_SCROLL_BAR:
                             draw_scroll_bar(w->widgets[i]->scroll_bar);
                             break;
                        case TYPE_PROGRESS_BAR:
                             draw_progress_bar(w->widgets[i]->progress_bar);
                             break;
                        case TYPE_TEXT_ZONE:
                             draw_text_zone(w->widgets[i]->text_zone);
                             break;
                        case TYPE_RADIO_BUTTON:
                             draw_radio_button(w->widgets[i]->radio_button);
                             break;
                        case TYPE_SELECT_CASE:
                             draw_select_case(w->widgets[i]->select_case);
                             break;
                        case TYPE_LABEL_TEXT:
                             draw_label_text(w->widgets[i]->label_text);
                             break;
                        default:
                                break;
                }
        }

        textprintf_centre(screen,font,w->x_pos+w->width/2,w->y_pos+10,0x0FF0,w->title);
}

void on_press_event(int x, int y)
{
        int i, n=0, m=0;

        for(i=0;i<nb_windows;i++)
        {
                textprintf_centre(screen,font,400,300,0x0FF0,"Fenetre %d\n",i);

                if( (windows_list[i]->x_pos<x) && ((windows_list[i]->x_pos+windows_list[i]->width )>x) )
                {
                        if( (windows_list[i]->y_pos<y) && ((windows_list[i]->y_pos+windows_list[i]->height)>y) )
                        {
                                if(windows_list[i]->focus > n)n=i;
                        }
                }
        }
        // donc : normalement c'est la fenetre 'n' qui a recu le click
        for(i=0;i<windows_list[n]->nb_widgets;i++)
        {
                textprintf_centre(screen,font,400,400,0x0FF0,"Widget %d\n",i);
                if( (windows_list[n]->widgets[i]->widget_select.x_pos<x) && ((windows_list[i]->widgets[i]->widget_select.x_pos+windows_list[i]->widgets[i]->widget_select.width )>x) )
                {
                        if( (windows_list[n]->widgets[i]->widget_select.y_pos<y) && ((windows_list[n]->widgets[i]->widget_select.y_pos+windows_list[n]->widgets[i]->widget_select.height)>y) )
                        {
                                m=i;
                        }
                }
        }
        windows_list[n]->widgets[m]->widget_select.on_press_func();
}

void on_release_event(int x, int y)
{
        int i, n=0, m=0;

        for(i=0;i<nb_windows;i++)
        {
                if( (windows_list[i]->x_pos<x) && ((windows_list[i]->x_pos+windows_list[i]->width )>x) )
                {
                        if( (windows_list[i]->y_pos<y) && ((windows_list[i]->y_pos+windows_list[i]->height)>y) )
                        {
                                if(windows_list[i]->focus > n)n=i;
                        }
                }
        }
        // donc : normalement c'est la fenetre 'n' qui a recu le click
        for(i=0;i<windows_list[n]->nb_widgets;i++)
        {
                if( (windows_list[n]->widgets[i]->widget_select.x_pos<x) && ((windows_list[i]->widgets[i]->widget_select.x_pos+windows_list[i]->widgets[i]->widget_select.width )>x) )
                {
                        if( (windows_list[n]->widgets[i]->widget_select.y_pos<y) && ((windows_list[n]->widgets[i]->widget_select.y_pos+windows_list[n]->widgets[i]->widget_select.height)>y) )
                        {
                                m=i;
                        }
                }
        }
        windows_list[n]->widgets[m]->widget_select.on_release_func();
}

int keyb_int(int key)
{
        switch(key)
        {
                case 0:
                default:
                        break;
        }
        textprintf(screen,font,16,16,0xFF00,"%c\n",key);
        return key;
}

void mouse_int(int flag)
{
 static int c=0;
        switch(flag)
        {
        
                case MOUSE_FLAG_MOVE:

                     break;

                case MOUSE_FLAG_LEFT_DOWN:
                     on_press_event(mouse_x,mouse_y);
                     break;

                case MOUSE_FLAG_LEFT_UP:
                     on_release_event(mouse_x,mouse_y);
                     break;

                case MOUSE_FLAG_RIGHT_DOWN:
                     line(screen,mouse_x,mouse_y,DESKTOP_WIDTH,DESKTOP_HEIGHT,c);
                     break;

                case MOUSE_FLAG_RIGHT_UP:
                
                     break;

                case MOUSE_FLAG_MIDDLE_DOWN:
                
                     break;

                case MOUSE_FLAG_MIDDLE_UP:
                
                     break;
                
                default:
                        break;

        }
}


int init_tkgui(int w, int h, int bpp)
{
        WINDOW* desktop;

        int c = GFX_AUTODETECT;
        char* theme_path="vide";
        char* title = "OWL Desktop !";
        char* tmp;

        desktop = (WINDOW*) malloc(sizeof(WINDOW));
        DESKTOP_WIDTH=w;
        DESKTOP_HEIGHT=h;
        DESKTOP_BPP=bpp;
        
        windows_list=0;
        nb_windows=0;

        allegro_init();

        if(install_keyboard()==-1)
        {
                allegro_exit();
                printf("Error : Keyboard required !!!\n");
                return 1;
        }
        
        keyboard_callback=keyb_int;

        install_timer();

        if(install_mouse()==-1)
        {
                allegro_exit();
                printf("Error : Mouse required !!!\n");
                return 1;
        }

        mouse_callback=mouse_int;

        set_color_depth(bpp);

        if(set_gfx_mode(c, w, h, 0, 0) != 0)
        {
                allegro_exit();
                printf("Error setting graphics mode\n%s\n\n", allegro_error);
                return 1;
        }

        show_mouse(screen);


        desktop->nb_widgets            =       0;
        desktop->widgets               =       (WIDGET **) malloc(sizeof(WIDGET*));
        desktop->window_control        =       DESKTOP;
        desktop->height                =       DESKTOP_HEIGHT+1;
        desktop->width                 =       DESKTOP_WIDTH+1;
        desktop->x_pos                 =       -1;
        desktop->y_pos                 =       -1;

        desktop->theme_path            =       (char*) malloc((strlen(theme_path)+1)*sizeof(char));
        strcpy(desktop->theme_path     ,       theme_path);
        desktop->title                 =       (char*) malloc((strlen(title)+1)*sizeof(char));
        strcpy(desktop->title          ,       title);

        sprintf(tmp,"%s\\title_button.bmp",theme_path);
                
        add_button(desktop,0,DESKTOP_HEIGHT-1-20,DESKTOP_WIDTH-1,20,0,nop,nop,tmp);
        add_button(desktop,2,DESKTOP_HEIGHT-17,128,14,0,menu,nop,tmp);

        windows_list             = (WINDOW**) malloc(sizeof(WINDOW*));
        windows_list[nb_windows] = (WINDOW*)  malloc(sizeof(WINDOW));

        memcpy(windows_list[nb_windows],desktop, sizeof(WINDOW));
        nb_windows=1;
        draw_window(desktop);
        return 0;
}

