#ifndef __BOREAL_WINDOW_HH__
#define __BOREAL_WINDOW_HH__

#define L_CLOSE			1
#define L_MAXIMISE        	2
#define L_MINIMISE        	4
#define L_MENU            	8

#define R_CLOSE           	16
#define R_MAXIMISE        	32
#define R_MINIMISE        	64
#define R_MENU            	128

#define DESKTOP           	1073741824

#include <widget.hh>
#include <stdio.h>

class BOREAL_WINDOW : public WIDGET
{
private:
    int window_control;
    char* theme_path;
    char* title;
    int focus;

    WIDGET** widgets;
    int nb_widgets;
public:
    BOREAL_WINDOW(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void), int wc, char* tp, char* title, int fo);
};

#endif
