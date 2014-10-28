
EXE=clipper
CC=gcc
LIBS=-lglfw -lGL -lm
FLAGS=-Wall

OBJ=main.o clipper.o

all: $(OBJ)
	$(CC) $(OBJ) $(FLAGS) $(LIBS) -o $(EXE)

.c.o:
	$(CC) $(FLAGS) -c $< -o $@


