#ifndef __IHM_H__
#define __IHM_H__

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <pthread.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

void* IOThread(void*t);

class IHM
{
private:

    SDL_Surface *screen;
    int 	depth;
    int		width;
    int		height;
    char*	name;
    int 	done;
    static int	StartStatic_IOThread(void* pUser);
    int 	IOThread(void);
    void 	(*BactPress[3])(void);
    void 	(*KactPress[256])(void);
    void 	(*BactRelease[3])(void);
    void 	(*KactRelease[256])(void);
    SDL_Thread 	*view_th;
    SDL_Thread 	*io_th;
    SDL_Event	event;

    pthread_mutex_t	seq;
    pthread_mutex_t	end;
    pthread_mutex_t	io;
    int		initial;

public:
    IHM		(int, int, int, char*, int, int(*f)(void*));
    ~IHM	();
    int		(*view_func)(void*);

    void 	show();
    int 	get_depth(void);
    int		get_done(void);
    int		get_width(void);
    int		get_height(void);
    SDL_Surface*get_screen(void);
    void	exec_BactPress(char);
    void	exec_KactPress(char);
    void	exec_BactRelease(char);
    void	exec_KactRelease(char);
    
    void	set_done(int);
    void 	set_BactPress(void (*func)(void), int nButton);
    void 	set_KactPress(void (*func)(void), char KeyCode);
    void 	set_BactRelease(void (*func)(void), int nButton);
    void 	set_KactRelease(void (*func)(void), char KeyCode);
    void	wait_end(void);
//    int **	graphics;
    void 	effacer(void);
    void	redraw(void);
};
#endif


