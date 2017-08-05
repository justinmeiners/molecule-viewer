SDLINCLUDE=/usr/local/include
SDLLIB=/usr/local/lib

FLAGS=-std=c++11 -O2
LIBS=-lSDL2 -framework OpenGL
IFLAGS=-I${SDLINCLUDE} -L${SDLLIB}


molecule: main.cpp
	g++ ${FLAGS} ${IFLAGS} ${LIBS} $< -o $@
