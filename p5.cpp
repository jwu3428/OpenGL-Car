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
void kb_up_callback(unsigned char key, int x, int y);

GLfloat ambientLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat position[] = { 1.0f, 1.0f, 1.0f, 0.0f };

GLfloat WorldXAngle = 0.0f;
GLfloat WorldYAngle = 0.0f;
GLfloat TireRotateSpeed = 1.0f;
GLfloat CameraZ = -5.0f;
GLfloat CarPos[] = {0.0f, 0.0f, 0.0f};
GLfloat CarRotation = 0.0f;

GLuint hexNut, tire, tireCap, carFront, axle;

int Time;
bool accel = false;
float accelRate = 0.0f;
bool backaccel = false;
bool cruisectrl = false;
bool turnRight = false, turnLeft = false;

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

void setMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat shine)
{
	GLfloat materialClr[] = {r, g, b, 1};
	GLfloat materialSpec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shininess[] = { shine };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialClr);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

void drawCarFront()
{
	carFront = glGenLists(1);
	glNewList(carFront, GL_COMPILE);
		glBegin(GL_POLYGON);
			glVertex3f(-0.4f,0,-0.2f);
			glVertex3f(-0.4f,0,0.2f);
			glVertex3f(0, 0, 0.5f);
			glVertex3f(0, 0.2f, 0.2f);
			glVertex3f(0, 0.2f, -0.2f);
			glVertex3f(0, 0, -0.5f);
		glEnd();
	glEndList();
}

void drawAxle()
{
	axle = glGenLists(1);
	glNewList(axle, GL_COMPILE);
		GLUquadricObj *cyl = gluNewQuadric();
		gluQuadricDrawStyle(cyl, GLU_FILL);
		gluCylinder(cyl, 0.1, 0.1, 4.0, 10, 10);
	glEndList(); 
}

void drawHexNut()
{	
	hexNut = glGenLists(1);
	glNewList(hexNut, GL_COMPILE);
		GLUquadricObj *quad = gluNewQuadric();
		gluQuadricNormals(quad, GLU_SMOOTH);
		gluQuadricDrawStyle(quad, GLU_FILL);
		glColor3f(0.5, 0.5, 0.5);
		gluCylinder(quad, 0.25, 0.25, 0.1, 6, 2);
		
		glPushMatrix();
		glTranslatef(0.0, 0.0, 0.1);
		gluDisk(quad, 0.0, 0.25, 6, 2);
		glPopMatrix();
		
		glPushMatrix();
		glRotatef(180.0, 1.0, 0, 0);
		gluDisk(quad, 0.0, 0.25, 6, 2);
		glPopMatrix();
	glEndList();
}

void drawTireCap()
{
	GLfloat h = 0.2f, r = 1.5f;
	tireCap = glGenLists(1);
	glNewList(tireCap, GL_COMPILE);
		GLUquadricObj *cap = gluNewQuadric();
		gluQuadricDrawStyle(cap, GLU_FILL);
		gluCylinder(cap, r, r, h, 20, 10);
		
		glPushMatrix();
			glTranslatef(0.0, 0.0, h);
			gluDisk(cap, 0.0, r, 20, 10);
		glPopMatrix();
		
		glPushMatrix();
			glRotatef(180.0, 1.0, 0, 0);
			gluDisk(cap, 0.0, r, 20, 10);
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
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialClr);
			glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpec);
			glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
			
			glPushMatrix();
				glTranslatef(0.0, 0.0, 0.3);
				glRotatef(72.0*i, 0.0, 0.0, 1.0);
				glTranslatef(1.0, 0.0, 0.0);
				glCallList(hexNut);
			glPopMatrix();
		}
		
		setMaterial(0.8, 0.8, 0, 50);
		glPushMatrix();
			cout << tireCap<< endl;
			glCallList(tireCap);
		glPopMatrix();
		
		GLfloat materialClr[] = {0.1, 0.1, 0.1, 1};
		GLfloat materialSpec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat shininess[] = { 20.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialClr);
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpec);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		
		glutSolidTorus(0.75, 2.0, 20, 20);
		
	glEndList();
}


void kb_callback(unsigned char key, int x, int y)
{
	switch(key) {
		case 'w':
			//cout << "Accelerating..." << endl;
			cruisectrl = false;
			accel = true;
			break;
		case 's':
			cruisectrl = false;
			backaccel = true;
			break;
		case 'a':
			turnLeft = true;
			break;
		case 'd':
			turnRight = true;
			break;
		case 'f':
			cruisectrl = !cruisectrl;
			break;
		case 'r':
			TireRotateSpeed += 0.1f;
			break;
		case 'R':
			TireRotateSpeed -= 0.1f;
		case 'z':
			CameraZ -= 1.0f;
			break;
		case 'x':
			CameraZ += 1.0f;
			break;
	}
	glutPostRedisplay();
}

void kb_up_callback(unsigned char key, int x, int y)
{
	switch(key) {
		case 'w':
			//cout << "Decelerating..." << endl;
			accel = false;
			break;
		case 's':
			backaccel = false;
			break;
		case 'a':
			turnLeft = false;
			break;
		case 'd':
			turnRight = false;
			break;
	}
	//glutPostRedisplay();
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
	//Time = glutGet(GLUT_ELAPSED_TIME);
	Time = 1;
	
	//glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Rotate the image
	glMatrixMode(GL_MODELVIEW); // Current matrix affects objects positions
	glLoadIdentity(); // Initialize to the identity
	glTranslatef(0.0f, 0.0f, CameraZ-5.0f);
	glRotatef(WorldXAngle, 0.0, 1.0, 0.0);
	glRotatef(WorldYAngle, 1.0, 0.0, 0.0);

	glDisable(GL_CULL_FACE);
	
	glPushMatrix();
		glutSolidSphere(0.1, 20, 20);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(CarPos[0]-4.0, CarPos[1], CarPos[2]);
		glRotatef(CarRotation, 0, 1, 0);
		glPushMatrix();
			glTranslatef(-2.5, 0, 2.0);
			glRotatef(TireRotateSpeed, 0.0, 0.0, 1.0);
			glScalef(0.3, 0.3, 0.3);
			glCallList(tire);
		glPopMatrix();
	
		glPushMatrix();
			glTranslatef(2.5, 0, 2.0);
			glRotatef(TireRotateSpeed, 0.0, 0.0, 1.0);
			glScalef(0.3, 0.3, 0.3);
			glCallList(tire);
		glPopMatrix();
	
		glPushMatrix();
			glTranslatef(-2.5, 0, -2.0);
			glRotatef(180.0, 1.0, 0, 0);
			glRotatef(-TireRotateSpeed, 0.0, 0.0, 1.0);
			glScalef(0.3, 0.3, 0.3);
			glCallList(tire);
		glPopMatrix();
	
		glPushMatrix();
			glTranslatef(2.5, 0, -2.0);
			glRotatef(180.0, 1.0, 0, 0);
			glRotatef(-TireRotateSpeed, 0.0, 0.0, 1.0);
			glScalef(0.3, 0.3, 0.3);
			glCallList(tire);
		glPopMatrix();
	
		glPushMatrix();
			setMaterial(0.5, 0.5, 0.5, 50.0);
			glTranslatef(-2.5,0,-2.0);
			glCallList(axle);
		glPopMatrix();
	
		glPushMatrix();
			glTranslatef(2.5,0,-2.0);
			glCallList(axle);
		glPopMatrix();
	
		glPushMatrix();
			glScalef(1.2, 1.0, 1.0);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			glTranslatef(0.0, 0.0, -2.0);
			glCallList(axle);
		glPopMatrix();
	
		glPushMatrix();
			setMaterial(1,1,1,20);
			glTranslatef(-4.0, 0.0, 0.0);
			glScalef(4.0, 4.0, 4.0);
			glCallList(carFront);
		glPopMatrix();
	glPopMatrix();
	
	if(!cruisectrl) {
		if(accel) {
			accelRate = min(10.0f, accelRate + 0.1f);
		} else if (!backaccel && accelRate > 0.0f) {
			//TireRotateSpeed = max(0.0f, TireRotateSpeed - 0.2f);
			accelRate = max(0.0f, accelRate - 0.05f);
		} else if (backaccel) {
			accelRate = max(-10.0f, accelRate - 0.1f);
		} else if (!accel && accelRate < 0.0f) {
			accelRate = min(0.0f, accelRate + 0.05f);
		}
	}
	
	if(turnRight) {
		CarRotation -= accelRate/10;
	} else if(turnLeft) {
		CarRotation += accelRate/10;
	}
		
	
	TireRotateSpeed = TireRotateSpeed + accelRate;
	//CarPos[0] -= accelRate/100;
	CarPos[0] -= cos(CarRotation / 180.0 * M_PI) * accelRate/100;
	CarPos[2] += sin(CarRotation / 180.0 * M_PI) * accelRate/100;
	cout << CarPos[0] << ' ' << CarPos[1]  << ' ' << CarPos[2] << endl;
	
	glutSwapBuffers();
	glutPostRedisplay();
}

void init()
{
	glClearColor(0,0,0,0);
	glColor3f(1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutSpecialFunc(arrow_key_callback);
	glutKeyboardFunc(kb_callback);
	glutKeyboardUpFunc(kb_up_callback);
	
	drawHexNut();
	drawTireCap();
	drawTire();
	drawCarFront();
	drawAxle();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("P5");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(update);
	glutMainLoop();
	
	return 0;
}
