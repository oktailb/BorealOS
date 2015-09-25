#include <toolkit.hh>
#include <borealGL.hh>

BorealAPP::BorealAPP(char* xml_file, void (**func)(void*), void ** args, IHM* ihm)
{
    this->ihm = ihm;
    widget_list = NULL;
    parse(xml_file, func, args);
}

BorealAPP::BorealAPP(void (*func)(BorealAPP*,void*), void* arg, IHM* ihm)
{
    func(this, arg);
    this->ihm = ihm;
    widget_list = NULL;
}

BorealAPP::~BorealAPP()
{
    WIDGET_LIST* lb = widget_list;
    for(;widget_list!=NULL;lb = widget_list->next)
    {
	delete lb;
    }
}

void BorealAPP::add_window(unsigned int window_control, char* theme_path, int width, int height, int x_pos, int y_pos, char* title, void(*opf)(void), void(*orf)(void), int focus)
{
    BOREAL_WINDOW* w;
    w = new BOREAL_WINDOW(x_pos, y_pos, width, height, opf, orf, window_control, theme_path, title, focus);
    add(w, widget_list);
}

void BorealAPP::add_button(int x_pos, int  y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), int mode, char* text)
{
    BUTTON* w;
    w = new BUTTON(x_pos, y_pos, width, height, on_press, on_release, mode, text);
    add(w, widget_list);
}

void BorealAPP::add_scroll_bar(int x_pos, int y_pos, int width, int height, int orientation, void (*on_press)(void), void (*on_release)(void), int level)
{
    SCROLL_BAR* w;
    w = new SCROLL_BAR(x_pos, y_pos, width, height, on_press, on_release, orientation, level);
    add(w, widget_list);
}

void BorealAPP::add_progress_bar(int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), int ext_angle, int int_angle, int level)
{
    PROGRESS_BAR* w;
    w = new PROGRESS_BAR(x_pos, y_pos, width, height, on_press, on_release, ext_angle, int_angle, level);
    add(w, widget_list);
}

void BorealAPP::add_text_zone(int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* font, int font_size, int font_style, char* text)
{
    TEXT_ZONE* w;
    w = new TEXT_ZONE(x_pos, y_pos, width, height, on_press, on_release, font, font_size, font_style, text);
    add(w, widget_list);
}

void BorealAPP::add_radio_button(int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* label, int grp, int value)
{
    RADIO_BUTTON* w;
    w = new RADIO_BUTTON(x_pos, y_pos, width, height, on_press, on_release, label, grp, value);
    add(w, widget_list);
}

void BorealAPP::add_select_case(int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* label, int value)
{
    SELECT_CASE* w;
    w = new SELECT_CASE(x_pos, y_pos, width, height, on_press, on_release,label, value);
    add(w, widget_list);
}

void BorealAPP::add_label_text(int x_pos, int y_pos, int width, int height, void (*on_press)(void), void (*on_release)(void), char* font, int font_size, int font_style, char* label)
{
    LABEL_TEXT* w;
    w = new LABEL_TEXT(x_pos, y_pos, width, height, on_press, on_release, font, font_size, font_style, label);
    add(w, widget_list);
}

void* BorealAPP::exec_call_back(char* func_name, char* args[])
{
    CALLBACKS_LIST* ptr;
    for(ptr = callbacks; ptr != NULL; ptr = ptr->next)
    {
	if(!strcmp(func_name, ptr->callback_name))
	{
	    return ptr->callback(args);
	}
    }
    return NULL;
}

void callback_register(void* (*callback)(char**), char* name, CALLBACKS_LIST* cb)
{
    CALLBACKS_LIST* lb;
    for(lb = cb;lb != NULL; lb = lb->next);
    lb = (CALLBACKS_LIST*) malloc(sizeof(CALLBACKS_LIST));
    lb->callback = callback;
    lb->callback_name = name;
    lb->next = NULL;
}

void add(WIDGET* w, WIDGET_LIST* l)
{
    WIDGET_LIST* lb;
    for(lb = l;lb != NULL;lb = lb->next);
    lb = (WIDGET_LIST*) malloc(sizeof(WIDGET_LIST));
    memcpy(lb->w ,w, sizeof(w));
    lb->next = NULL;
}

int parse(char* xml_file, void (**func)(void*), void** args)
{
    int f, i, j, k, base, nbl, nbb, nbp;
    struct stat* st;
    char* dump;
    char** bufl;
    char** bufb;
    char** buff;
    char** para;
    
    st = new struct stat;
    
    if((f=open(xml_file, O_RDONLY, 0666))<0){perror("open:");exit(-1);};
    
    fstat(f, st);
    dump = (char*) malloc((st->st_size)*sizeof(char));
    if(read(f, dump, st->st_size)<0)
    {
        perror("read:");
        exit(-1);
    }
    
    nbl=0;
    for(i=0;i<st->st_size;i++)if(dump[i]=='\n')nbl++;
    bufl = (char**) malloc(nbl*sizeof(char*));
    
    nbb=0;
    for(i=0;i<st->st_size;i++)if(dump[i]=='<')nbb++;
    bufb = (char**) malloc(nbb*sizeof(char*));
    buff = (char**) malloc(nbb*sizeof(char*));
    
    base=0;
    j=0;
    k=0;
    for(i=0;i<st->st_size;i++)
    {
	if(dump[i] == '<')
	{
	    bufb[j] = &dump[i+1];
	    base = i;
	    j++;
	}
    }
    
    nbp=0;
    base=0;
    if((para = (char**) malloc(sizeof(char*)))==NULL)
    {
	perror("malloc :");
	exit(-1);
    }

    for(i=0;i<st->st_size;i++)
    {
	if(dump[i] == '>')
	{
	    k=j;
	    j=i;
	    while(dump[j] != '<')
	    {
		j++;
		if (j > st->st_size)break;
	    }
	    if((dump[j+1] == '/') && (dump[k+1] != '/'))
	    {	
		if(nbp != 0)
		{
		    para = (char**) realloc(para,(nbp+1)*sizeof(char*));
		}
		base = j-i;
		if((para[nbp] = (char*) malloc(sizeof(char)*base)) == NULL)
		{
		    perror("malloc :");
		    exit(-1);
		}
		strncpy(para[nbp], &dump[i+1], j-i-1);
		para[nbp][base-1] = '\0';
		nbp++;
	    }
	    else
	    {
		i=j;
	    }
	}
    }
    
    for(i=0; i<nbb; i++)
    {
	buff[i] = epure(bufb[i], '>');
    }

    free(bufl);
    free(bufb);
    free(dump);
    close(f);
    
    base=0;
    
    for(i=0; i<nbb; i++)
    {
	if(!strcmp(buff[i],"boreal window"))
	{
	    for(j=i;j<nbb;j++)
	    {
		if(!strcmp(buff[j],"/boreal window"))
		{
		    //entre i et j ...
		    for(k=0;k<j;k++)
		    {
			if(!strcmp("window control", buf[i])
			{
			    if(!strcmp("L_CLOSE",param[base]))
			    {
				
			    }
			    if(!strcmp("R_CLOSE",param[base]))
			    {
			    
			    }
			    if(!strcmp("L_MAXIMISE",param[base]))
			    {
			    
			    }
			    if(!strcmp("R_MAXIMISE",param[base]))
			    {
			    
			    }
			    if(!strcmp("L_MINIMISE",param[base]))
			    {
			    
			    }
			    if(!strcmp("R_MINIMISE",param[base]))
			    {
			    
			    }
			    if(!strcmp("L_MENU",param[base]))
			    {
			    
			    }
			    if(!strcmp("R_MENU",param[base]))
			    {
			    
			    }
			    base ++;
			}
			if(!strcmp("theme path", buf[i])
			{
			
			}
			if(!strcmp("x_pos", buf[i])
			{
			
			}
			if(!strcmp("y_pos", buf[i])
			{
			
			}
			if(!strcmp("title", buf[i])
			{
			
			}
			if(!strcmp("on press func", buf[i])
			{
			
			}
			if(!strcmp("on release func", buf[i])
			{
			
			}
			if(!strcmp("focus", buf[i])
			{
			
			}
		    }
		    break;
		}
	    }
	}	
    }
    
    free(buff);
    free(para);
    return 0;
}
