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

GLfloat materialSpec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat shininess[] = { 50.0 };
GLfloat ambientLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat position[] = { 1.0f, 1.0f, 1.0f, 0.0f };

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

void drawHexNut()
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
	
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.4);
	gluDisk(hexNut, 0.0, 1.0, 6, 2);
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(180.0, 1.0, 0, 0);
	gluDisk(hexNut, 0.0, 1.0, 6, 2);
	glPopMatrix();
}

void drawTire()
{
	
}

void update()
{
	//int time = glutGet(GLUT_ELAPSED_TIME);
	int time = 1;
	
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Rotate the image
	glMatrixMode(GL_MODELVIEW); // Current matrix affects objects positions
	glLoadIdentity(); // Initialize to the identity
	glTranslatef(0, 0.0, -5.0); // Translate from origin (in front of viewer)
	glRotatef(0.0, 0.0, 1.0, 0.0); // Rotate around y-axis
	glRotatef(-20, 1.0, 0.0, 0.0); // Set Azimuth angle

	glDisable(GL_CULL_FACE);
	glPushMatrix();
	glRotatef(time * 1.0, 0.0, 0.0, 1.0);
	glTranslatef(1.5, 0.0, 0.0);
	drawHexNut();
	glPopMatrix();
	
	glutSwapBuffers();
	//glutPostRedisplay();
}

void init()
{
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("P5");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(update);
	glClearColor(0,0,0,0);
	glColor3f(1,1,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	
	return 0;
}
