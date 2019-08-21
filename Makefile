

CROSS_COMPILE = C:/devkitGP2X/bin/arm-linux-
SDL_BASE = C:/devkitGP2X/bin/arm-linux-
LDFLAGS = -static

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
STRIP = $(CROSS_COMPILE)strip

CFLAGS = `$(SDL_BASE)sdl-config --cflags` -O2 -Wall
CXXFLAGS = `$(SDL_BASE)sdl-config --cflags` -O2 -Wall
LIBS = `$(SDL_BASE)sdl-config --libs` -lSDL_mixer -lsmpeg -lvorbisfile -lvorbis -logg -lmikmod -lSDL -lpthread 

SDLTEST_TARGET = jurl.gpe
SDLTEST_OBJS = jurl.o main.o fading.o font.o targa.o highscore.o sound.o

all : $(SDLTEST_TARGET)

$(SDLTEST_TARGET) : $(SDLTEST_OBJS)
	$(CXX) $(LDFLAGS) -o $(SDLTEST_TARGET) $(SDLTEST_OBJS) $(LIBS)
	$(STRIP) $(SDLTEST_TARGET)

clean:
	rm -f $(ALL_TARGETS) *.o *~
