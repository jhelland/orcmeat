#!/usr/bin/make

#========================================================================================== 
# Created by jhelland (12/21/18)
#
# To use, run 'make' in /src.
# To clean up object files, run 'make clean_object' in /src.
#========================================================================================== 

# Include non standard library files:
# 	INCLUDEDIR :: where header files live
# 	LIBDIR 		 :: where library object files live
INCLUDEDIR=/usr/include/
LIBDIR=/usr/lib

# Add more source files here
SOURCE= state_engine.cpp \
				main_menu_state.cpp \
			 	pause_menu_state.cpp \
				music.cpp sound.cpp \
				game.cpp \
				main.cpp \
				game_board.cpp \
				tetromino.cpp

# Compiler
CC= g++

# Name of final executable
EXECUTABLE= qmtetris

# Linux (default)
CFLAGS= -O0 -g -Wall -pedantic -Wno-unused-result -DGL_GLEXT_PROTOTYPES -std=c++17
#LDFLAGS= -lGL -lglut -lGLEW -lXext -lX11 -lm -lncursesw
LDFLAGS= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 

# OsX

# Additional library files
INCLUDEFLAGS= -I. -I$(INCLUDEDIR) -Iinclude/
LIBFLAG= -L$(LIBDIR)

# Don't touch
OBJECT= $(SOURCE:.cpp=.o)

# Also don't touch
all: $(OBJECT) depend
	$(CC) $(CFLAGS) $(INCLUDEFLAG) $(LIBFLAG) $(OBJECT) -o $(EXECUTABLE) $(LDFLAGS)

qmtetris: all

rebuild: clean all

depend:
	$(CC) -M $(SOURCE) > depend

$(OBJECT):
	$(CC) $(CFLAGS) $(INCLUDEFLAG) -c -o $@ $(@:.o=.cpp)

clean_object:
	rm -f $(OBJECT)

clean:
	rm -f $(OBJECT) depend $(EXECUTABLE)

include depend
