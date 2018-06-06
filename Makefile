ARGS = -Wall -g -lglut -lGL -lGLU `pkg-config --libs opencv`

p5:
	g++ p5.cpp $(ARGS) -o p5.out

clean:
	rm -f p5.out
