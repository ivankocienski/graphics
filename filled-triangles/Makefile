
EXE=filled-triangle-demo
GPP=g++
OBJ=main.o triangle.o point.o rasterizer.o
CFLAGS=-Wall -g
LIBS=-lSDL

all: $(OBJ)
	$(GPP) $(OBJ) -o $(EXE) $(CFLAGS) $(LIBS)

.cc.o:
	$(GPP) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXE)
