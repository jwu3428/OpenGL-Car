#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

using namespace std;

typedef struct RGB {
	GLfloat r, g, b;
	RGB(GLfloat _r = 0.0f, GLfloat _g = 0.0f, GLfloat _b = 0.0f) : r(_r), g(_g), b(_b) {}
} RGB;

GLUquadricObj *hexNut = gluNewQuadric();
//gluQuadricNormals(hexNut, GL_TRUE);

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 40.0);
	/* if ( h==0 )
	gluPerspective ( 80, ( float ) w, 1.0, 5000.0 );
	else
	gluPerspective ( 80, ( float ) w / ( float ) h, 1.0, 5000.0 );
	*/ 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawCylinder()
{
	/*
	glNewList(cyl, GL_COMPILE);
		GLUquadric *nut = gluNewQuadric();
		gluQuadricDrawStyle(nut, GLU_FILL);
		glColor3f(1, 0, 0);
		gluCylinder(nut, 1.0, 1.0, 0.3, 6, 2);
	glEndList();
	*/
	gluQuadricDrawStyle(hexNut, GLU_FILL);
	glColor3f(0.5, 0.5, 0.5);
	gluCylinder(hexNut, 1.0, 1.0, 0.4, 6, 2);
}

void update()
{
	int time = glutGet(GLUT_ELAPSED_TIME);
	
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Rotate the image
	glMatrixMode(GL_MODELVIEW); // Current matrix affects objects positions
	glLoadIdentity(); // Initialize to the identity
	glTranslatef(0, 0.0, -5.0); // Translate from origin (in front of viewer)
	glRotatef(0.0, 0.0, 1.0, 0.0); // Rotate around y-axis
	glRotatef(0.0, 1.0, 0.0, 0.0); // Set Azimuth angle

	glDisable(GL_CULL_FACE);
	glPushMatrix();
	glRotatef(time * 1.0, 0.0, 0.0, 1.0);
	glTranslatef(1.5, 0.0, 0.0);
	drawCylinder();
	glPopMatrix();
	
	glutSwapBuffers();
	glutPostRedisplay();
}

void init()
{
	
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("P5");
	glutReshapeFunc(reshape);
	glutDisplayFunc(update);
	glClearColor(0,0,0,0);
	glColor3f(1,1,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	
	return 0;
}
