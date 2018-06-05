#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "objloader.hpp"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 640
#define MAX_CTRL_PTS 20

using namespace std;

typedef struct RGB {
	GLfloat r, g, b;
	RGB(GLfloat _r = 0.0f, GLfloat _g = 0.0f, GLfloat _b = 0.0f) : r(_r), g(_g), b(_b) {}
} RGB;

vector<vec3> vertices;
vector<vec2> uvs;
vector<vec3> normals;

void update()
{
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);
	
	glutSwapBuffers();
}

void init()
{
	bool res = loadOBJ("DODGE_CHALLENGER_383_MAGNUM/CHALLENGER71.obj", vertices, uvs, normals);
}

int main(int argc, char **argv)
{
	init();
	
	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("P5");
	glutDisplayFunc(update);
	
	glClearColor(0,0,0,0);
	glColor3f(1,1,1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glutMainLoop();
	
	return 0;
}
