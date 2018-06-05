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

void arrow_key_callback(int key, int x, int y);
void kb_callback(unsigned char key, int x, int y);

//GLfloat materialSpec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//GLfloat shininess[] = { 50.0 };
GLfloat ambientLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat position[] = { 1.0f, 1.0f, 1.0f, 0.0f };

//gluQuadricNormals(hexNut, GL_TRUE);

GLfloat WorldXAngle = 0.0f;
GLfloat WorldYAngle = 0.0f;
GLfloat TireRotateAngle = 0.0f;
GLfloat CameraZ = -5.0f;

GLuint hexNut;
GLuint tire;
GLuint triangle;

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
	hexNut = glGenLists(1);
	glNewList(hexNut, GL_COMPILE);
		GLUquadricObj *quad = gluNewQuadric();
		gluQuadricDrawStyle(quad, GLU_FILL);
		glColor3f(0.5, 0.5, 0.5);
		gluCylinder(quad, 0.25, 0.25, 0.1, 6, 2);
		
		glPushMatrix();
		glTranslatef(0.0, 0.0, 0.1);
		gluDisk(quad, 0.0, 0.25, 6, 2);
		glPopMatrix();
		
		glPushMatrix();
		glRotatef(180.0, 0.1, 0, 0);
		gluDisk(quad, 0.0, 0.25, 6, 2);
		glPopMatrix();
	glEndList();
}

void drawTire()
{
	
	tire = glGenLists(1);
	glNewList(tire, GL_COMPILE);
		for(int i = 0; i < 5; i++) {
			
			GLfloat materialClr[] = {0.5, 0.5, 0.5, 1};
			GLfloat materialSpec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			GLfloat shininess[] = { 50.0 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialClr);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpec);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
			
			glPushMatrix();
				glTranslatef(0.0, 0.0, 0.3);
				glRotatef(72.0*i, 0.0, 0.0, 1.0);
				glTranslatef(1.0, 0.0, 0.0);
				glCallList(hexNut);
			glPopMatrix();
		}
		
		GLfloat materialClr[] = {1, 0.1, 0.1, 1};
		GLfloat materialSpec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat shininess[] = { 20.0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialClr);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpec);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
		
		glutSolidTorus(1.2, 2.4, 20, 20);
	glEndList();
}


void kb_callback(unsigned char key, int x, int y)
{
	switch(key) {
		case 'r':
			TireRotateAngle += 1.0f;
			break;
		case 'z':
			CameraZ -= 1.0f;
			break;
		case 'x':
			CameraZ += 1.0f;
			break;
	}
	glutPostRedisplay();
}

void arrow_key_callback(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_LEFT:
			WorldXAngle -= 5.0f;
			break;
		case GLUT_KEY_RIGHT:
			WorldXAngle += 5.0f;
			break;
		case GLUT_KEY_UP:
			WorldYAngle -= 5.0f;
			break;
		case GLUT_KEY_DOWN:
			WorldYAngle += 5.0f;
			break;
	}
	glutPostRedisplay();
}

void update()
{
	//int time = glutGet(GLUT_ELAPSED_TIME);
	//int time = TireRotateAngle;
	
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Rotate the image
	glMatrixMode(GL_MODELVIEW); // Current matrix affects objects positions
	glLoadIdentity(); // Initialize to the identity
	glTranslatef(-2, -2, CameraZ-5); // Translate from origin (in front of viewer)
	glRotatef(WorldXAngle, 0.0, 1.0, 0.0);
	glRotatef(WorldYAngle, 1.0, 0.0, 0.0);

	glDisable(GL_CULL_FACE);
	//glPushMatrix();
		for(int i = 0; i < 2; i++) {
			glPushMatrix();
				glTranslatef(10.0 * i, 0, 0);
				glRotatef(TireRotateAngle, 0.0, 0.0, 1.0);
				glCallList(tire);
			glPopMatrix();
		}
		for(int i = 0; i < 2; i++) {
			glPushMatrix();
				glTranslatef(10.0 * i, 0, -8.0);
				glRotatef(180.0, 1.0, 0, 0);
				glRotatef(-TireRotateAngle, 0.0, 0.0, 1.0);
				glCallList(tire);
			glPopMatrix();
		}
	//glPopMatrix();
	
	glutSwapBuffers();
	//glutPostRedisplay();
}

void init()
{
	glClearColor(0,0,0,0);
	glColor3f(1,1,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	
	glutSpecialFunc(arrow_key_callback);
	glutKeyboardFunc(kb_callback);
	
	drawHexNut();
	drawTire();
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
	glutMainLoop();
	
	return 0;
}
