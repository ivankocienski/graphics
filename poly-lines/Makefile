
GPP=g++
CFLAGS=-Wall -g
LIBS=-lSDL
EXE=poly-lines

OBJECTS=\
	main.o \
	lines.o \
	point.o \
	line-loop.o

all: $(OBJECTS)
	$(GPP) -o $(EXE) $(OBJECTS) $(CFLAGS) $(LIBS)


.cc.o:
	$(GPP) -c $< -o $@ $(CFLAGS)
