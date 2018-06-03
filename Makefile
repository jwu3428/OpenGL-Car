ARGS = -Wall -g
OGL = -lglut -lGL -lGLU -lGLEW

p5.out: p5.o objloader.o
	g++ $(ARGS) p5.o objloader.o -o p5.out $(OGL)

objloader.o: objloader.cpp objloader.hpp
	g++ $(ARGS) -c objloader.cpp

p5.o: p5.cpp objloader.hpp
	g++ $(ARGS) -c p5.cpp

clean:
	rm -f *.o p5.out
