#ifndef __WIDGET_HH__
#define __WIDGET_HH__

class WIDGET
{
    private:
	int x_pos;
	int y_pos;
	int width;
	int height;
        void (*on_press_func)(void);
	void (*on_release_func)(void);
	
    public:
	WIDGET(int x, int y, int w, int h, void(*opf)(void), void(*orf)(void));
	void (*draw)(void*);
	int get_x_pos(void);
	int get_y_pos(void);
	int get_width(void);
	int get_height(void);
	void set_x_pos(int);
	void set_y_pos(int);
	void set_width(int);
	void set_height(int);
	void set_draw(void (*f)(void*));
        void exec_on_press_func(void);
	void exec_on_release_func(void);
};

#endif
