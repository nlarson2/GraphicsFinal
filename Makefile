FFLAGS = main.cpp Image.cpp Maths.cpp Model.cpp Octree.cpp
HFLAGS = Image.h Maths.h Model.h Octree.h
CFLAGS = -Wall -o game -lX11 -lGL -lGLU -lm -lpthread
all: game

game: $(FFLAGS) $(HFLAGS)
	g++ -g $(FFLAGS) $(CFLAGS) -o game

clean:
	rm -f game

