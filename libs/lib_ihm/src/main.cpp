/************************************************************************
*									*
*	BorealIHM, Fichier principal, point d'entree du code.		*
*									*
************************************************************************/

#include <IHM.hh>
#include <task.hh>
#include <libBoreal.hh>
#include <borealGL.hh>
#include <toolkit.hh>

#include <unistd.h>
#include <stdlib.h>


IHM* i;

int base=1;

void r(void*)
{
}

int te(void*)
{
    BorealAPP* b;
    int xm,ym;
    SDL_Surface* scr;

    scr = i->get_screen();
    
    xm = i->get_width()-1;
    ym = i->get_height()-1;

    b = new BorealAPP("apps/ex1.xml", NULL, NULL, i);

    return 0;
}

void redraw(void)
{
    base *= 2;
    printf("base changee : %d...\n",base);
    i->redraw();
}

void unredraw(void)
{
    base /= 5;
    printf("base changee : %d...\n",base);
    i->redraw();
}

void quit(void)
{
    delete i;
}

int ww(char* arg[])
{
    pthread_mutex_t b;
    pthread_mutex_init(&b, NULL);
    i = new IHM(atoi(arg[0]),atoi(arg[1]), atoi(arg[2]), arg[3], WINDOW, te);
    i->set_KactPress(quit, 'q');
    i->set_KactPress(redraw, 'r');
    i->set_KactPress(unredraw, 'e');
    i->set_BactPress(redraw,1);
    i->show();
    return 0;
}

int main(int argc, char* argv[])
{
    //Task* g;
    if(argc<5){printf("usage : %s width height depth name\n", argv[0]);exit(0);}


    char* v[] = {argv[1],argv[2],argv[3],argv[4], NULL};
    ww(v);
    //g = new Task(ww,v);
    //g->set_cbackground(FOREGROUND);
    //g->run();
    //delete g;
    return 0;
}
