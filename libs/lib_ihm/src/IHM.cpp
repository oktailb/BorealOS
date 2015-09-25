#include <IHM.hh>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void rien(void)
{
    return;
}

IHM::~IHM()
{
    SDL_KillThread(io_th);
    SDL_KillThread(view_th);
    SDL_Quit();
}

IHM::IHM( int width, int height, int depth, char *name, int mode, int (*f)(void*))
{
    initial = 0;
    done=0;
    for(int i=0;i<256;i++)
    {
	KactPress[i] = rien;
	KactRelease[i] = rien;
    }
    for(int i=0;i<3;i++)
    {
	BactRelease[i] = rien;
	BactPress[i] = rien;
    }
    if( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE|SDL_INIT_EVENTTHREAD) < 0 ) {
        fprintf(stderr,"Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    atexit(SDL_Quit);
    
    !mode?
	screen = SDL_SetVideoMode(width, height, depth, SDL_HWSURFACE|SDL_DOUBLEBUF)
	 :
	screen = SDL_SetVideoMode(width, height, depth, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN)
	;

    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n",width,height,depth,SDL_GetError());
        exit(1);
    }
    this->name = name;
    this->width = width;
    this->height = height;
    this->depth = depth;
    view_func = f;
    
    if((io_th = SDL_CreateThread(StartStatic_IOThread, this))<0)
    {
	fprintf(stderr,"Couldn't initialize SDL_Thread io: %s\n", SDL_GetError());
        exit(1);
    }
}

void IHM::redraw(void)
{
    while(!initial);
    if((view_th = SDL_CreateThread(view_func,NULL))<0)
    {
	fprintf(stderr,"Couldn't initialize SDL_Thread io: %s\n", SDL_GetError());
        exit(1);
    }
}

void IHM::show(void)
{
    if((view_th = SDL_CreateThread(view_func,NULL))<0)
    {
	fprintf(stderr,"Couldn't initialize SDL_Thread io: %s\n", SDL_GetError());
        exit(1);
    }
    initial=1;
    SDL_WaitThread(io_th, NULL);
    SDL_WaitThread(view_th, NULL);
}

extern "C"
{
    int IHM::StartStatic_IOThread(void* pUser)
    {
	IHM* pThis = (IHM*)pUser;
        return pThis->IOThread();
    }
}

int IHM::IOThread(void)
{
    while(!done)
    {
	SDL_WaitEvent(&event);
        switch( event.type )
	{
	    case SDL_ACTIVEEVENT:
		if(event.active.state == SDL_APPACTIVE & event.active.gain)
	        redraw();
		break;
    	    case SDL_KEYDOWN:
		exec_KactPress(event.key.keysym.sym);
    		break;
    	    case SDL_KEYUP:
		exec_KactRelease(event.key.keysym.sym);
    		break;
    	    case SDL_MOUSEBUTTONDOWN:
		exec_BactPress(event.button.button);
    		break;
    	    case SDL_MOUSEBUTTONUP:
		exec_BactRelease(event.button.button);
    		break;
	    case SDL_QUIT:
		done=1;
		break;
    	    default:
    		break;
	}
    }
    return 0;
}

int IHM::get_depth(void)
{
    return depth;
}

int IHM::get_width(void)
{
    return width;
}

int IHM::get_height(void)
{
    return height;
}

SDL_Surface* IHM::get_screen(void)
{
    return screen;
}

void IHM::effacer(void)
{
}


void IHM::set_BactPress(void (*func)(void), int nButton)
{
    BactPress[nButton-1] = func;
}

void IHM::set_KactPress(void (*func)(void), char KeyCode)
{
    KactPress[(int)KeyCode] = func;
}

void IHM::set_BactRelease(void (*func)(void), int nButton)
{
    BactRelease[nButton-1] = func;
}

void IHM::set_KactRelease(void (*func)(void), char KeyCode)
{
    KactRelease[(int)KeyCode] = func;
}

int IHM::get_done(void)
{
    return done;
}

void IHM::set_done(int d)
{
    done=d;
}

void IHM::exec_BactPress(char vec)
{
    BactPress[(int)vec]();
}

void IHM::exec_KactPress(char vec)
{
    KactPress[(int)vec]();
}

void IHM::exec_BactRelease(char vec)
{
    BactRelease[(int)vec]();
}

void IHM::exec_KactRelease(char vec)
{
    KactRelease[(int)vec]();
}
