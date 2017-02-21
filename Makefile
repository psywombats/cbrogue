LIBTCODDIR=libtcod-1.5.1
SDLDIR=SDL-1.2.14
CPPFLAGS=$(FLAGS) -I$(LIBTCODDIR)/include -I$(SDLDIR)/include -IChimeraCode -IBrogueCode -IPlatformCode -DBROGUE_TCOD -Wall -fpermissive
CFLAGS=$(FLAGS) -I$(LIBTCODDIR)/include -I$(SDLDIR)/include -IChimeraCode -IBrogueCode -IPlatformCode -DBROGUE_TCOD -Wall

# for release builds, change -g to -s -O2 to get things performant
# ^ yes this is my ghetto build configurations strategy

%.o : %.cpp
	gcc $(CPPFLAGS) -g -o $@ -c $< 

%.o : %.c
	gcc $(CFLAGS) -g -o $@ -c $< 

OBJS=BrogueCode/Architect.o \
	BrogueCode/Combat.o \
	BrogueCode/Dijkstra.o \
	BrogueCode/Globals.o \
	BrogueCode/Grid.o \
	BrogueCode/IO.o \
	BrogueCode/Buttons.o \
	BrogueCode/MainMenu.o \
	BrogueCode/Items.o \
	BrogueCode/Light.o \
	BrogueCode/Monsters.o \
	BrogueCode/Movement.o \
	BrogueCode/RogueMain.o \
	BrogueCode/Random.o \
	BrogueCode/Recordings.o \
	BrogueCode/Time.o \
	ChimeraCode/MonsterGlobals.o \
	ChimeraCode/Blueprints.o \
	PlatformCode/main.o \
	PlatformCode/platformdependent.o \
	PlatformCode/tcod-platform.o 

all : brogue

brogue-icon.o : brogue-icon.ico icon.rc
	windres icon.rc brogue-icon.o

brogue : ${OBJS} brogue-icon.o 
	g++ -o brogue.exe ${OBJS} brogue-icon.o -L. -ltcod-mingw -lSDL -L$(LIBTCODDIR)/ -static-libgcc -static-libstdc++ -mwindows

clean:
	find . -type f -name '*.o' -delete
	rm brogue.exe
