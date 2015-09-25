CC=g++
INCLUDE=-I. -I$(HOME)/Work/include -I./include -I./src -I/usr/include/SDL 
CFLAGS=-W -Wall -O3 $(INCLUDE)

TARGET=-o boreal

SRC  = task.cpp
SRC += IHM.cpp
SRC += libBoreal.cpp 
SRC += toolkit.cpp 
SRC += borealGL.cpp 
SRC += boreal_window.cpp 
SRC += button.cpp 
SRC += label_text.cpp 
SRC += progress_bar.cpp 
SRC += radio_button.cpp 
SRC += scroll_bar.cpp 
SRC += select_case.cpp 
SRC += text_zone.cpp 
SRC += widget.cpp

OBJ = $(SRC:.cpp=.o)

LIBS =  -L/usr/X11R6/lib -lX11 -lm -lpthread -D_REENTRANT -L/usr/lib -lSDL
all:borealIHM

clean:
	rm -rf objs/*.{o,a,la,so} src/*.{o,a,la,so} core.[0-9]* boreal .depend

borealIHM:$(OBJ)
	$(CC) $(TARGET) src/main.cpp $(OBJ_C) $(OBJ_TK) $(CFLAGS) $(LIBS)
