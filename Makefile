ARGS = -Wall -g -lglut -lGL -lGLU `pkg-config --libs opencv`

p5:
	g++ opengl-car.cpp $(ARGS) -o opengl-car.out

clean:
	rm -f opengl-car.out
