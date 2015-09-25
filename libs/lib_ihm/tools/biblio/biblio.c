#include <conio.h>
#include <stdio.h>
#include <allegro.h>
#include "tkgui.h"

char* mois[12]={
"Janvier",
"Fvrier",
"Mars",
"Avril",
"Mai",
"Juin",
"Juillet",
"Aot",
"Septembre",
"Octobre",
"Novembre",
"Dcembre"
};

int nbj_mois[12]={31,29,31,30,31,30,30,31,30,31,30,31};

char* jours[7]={
"Lundi",
"Mardi",
"Mercredi",
"Jeudi",
"Vendredi",
"Samedi",
"Dimanche"};


int main (int argc, char* argv[])
{
        WINDOW* win;
        WINDOW* dialog;
        win = (WINDOW*) malloc(sizeof(WINDOW));
        dialog = (WINDOW*) malloc(sizeof(WINDOW));

        init_tkgui(1024,768,16);

        add_window(win, L_MENU|R_MINIMISE|R_MAXIMISE|R_CLOSE, "vide", 800, 600, 100, 100, "Une fentre de 800 par 600 ! ...");
//        draw_window(win);

        add_window(dialog, R_CLOSE, "vide", 200, 100, 200, 200, "Une fentre diffrente");
        printf("\a\a\a\a\a\a\a\a");
//        draw_window(dialog);

        sleep(10);

        allegro_exit();
        
        return 0;
}


