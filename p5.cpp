#include <GL/glut.h>
#include <GL/freeglut.h>
#include <opencv2/opencv.hpp>
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

typedef struct vertex {
	GLfloat x, y, z;
	vertex(GLfloat _x = 0.0f, GLfloat _y = 0.0f, GLfloat _z = 0.0f) : x(_x), y(_y), z(_z) {}
} vertex;

void arrow_key_callback(int key, int x, int y);
void kb_callback(unsigned char key, int x, int y);
void kb_up_callback(unsigned char key, int x, int y);
vertex cross(vertex v1, vertex v2);

GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat position[] = { 0.0, 1.0f, 0.0f, 0.0f };

GLfloat WorldXAngle = 0.0f;
GLfloat WorldYAngle = 0.0f;
GLfloat TireRotateSpeed = 1.0f;
GLfloat TireRotation = 0.0f;
GLfloat CameraZ = -5.0f;
GLfloat CarPos[] = {0.0f, 0.0f, 0.0f};
GLfloat CarRotation = 0.0f;

GLuint hexNut, tire, tireCap, carFront, axle, carBody, carSide, carEngine, carExhaustPipe, steeringWheel;
GLuint g_texID;

int CameraMode = 0;
int Time;
bool accel = false;
float accelRate = 0.0f;
bool backaccel = false;
bool cruisectrl = false;
bool turnRight = false, turnLeft = false;

vertex cross(vertex v1, vertex v2)
{
	vertex n, a;
	n.x = v1.y * v2.z - v2.y * v1.z;
	n.y = v1.z * v2.x - v2.z * v1.x;
	n.z = v1.x * v2.y - v2.x * v1.y;
	float mag = sqrt(pow(n.x,2)+pow(n.y,2)+pow(n.z,2));
	n.x /= mag;
	n.y /= mag;
	n.z /= mag;
	return n;
}

vertex cross(vertex v1, vertex v2, vertex v3)
{
	vertex va, vb;
	va.x = v2.x - v1.x;
	va.y = v2.y - v1.y;
	va.z = v2.z - v1.z;
	vb.x = v3.x - v1.x;
	vb.y = v3.y - v1.y;
	vb.z = v3.z - v1.z;
	return cross(va, vb);
}

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
	GLfloat materialAmb[] = {r/2, g/2, b/2, 1};
	GLfloat materialSpec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shininess[] = { shine };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialClr);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

void drawCarFront()
{
	vertex v[] = {
		vertex(-.2f,0,-.3f),
		vertex(-0.2f,0,0.3f),
		vertex(0,0.2f,0.3f),
		vertex(0,0.2f,0.3f),
		vertex(0,0.2f,-0.3f),
		vertex(-0.2f,0,-0.3f),
		
		vertex(-.2f,0,.3f),
		vertex(0,0,.5f),
		vertex(0,.2f,.3f),
		
		vertex(-.2f,0,-.3f),
		vertex(0,.2f,-.3f),
		vertex(0,0,-.5f),
		
		vertex(0,0,-.5f),
		vertex(0,.2f,-.3f),
		vertex(0,0,-.3f),
		
		vertex(0,0,-.3f),
		vertex(0,.2f,-.3f),
		vertex(0,.2f,.3f),
		
		vertex(0,0,-.3f),
		vertex(0,0,.3f),
		vertex(0,.2f,.3f),
		
		vertex(0,.2f,.3f),
		vertex(0,0,.5f),
		vertex(0,0,.3f),
		
		vertex(0,0,-.5f),
		vertex(0,0,-.3f),
		vertex(-.2f,0,-.3f),
		
		vertex(-.2f,0,-.3f),
		vertex(0,0,-.3f),
		vertex(0,0,.3f),
		
		vertex(0,0,.3f),
		vertex(-.2f,0,.3f),
		vertex(-.2f,0,-.3f),
		
		vertex(-.2f,0,.3f),
		vertex(0,0,.3f),
		vertex(0,0,.5f)
	};
	carFront = glGenLists(1);
	glNewList(carFront, GL_COMPILE);
		glBegin(GL_TRIANGLES);
			for(unsigned int i = 0; i < sizeof(v)/sizeof(v[0]); i += 3) {
				vertex n = cross(v[i], v[i+1], v[i+2]);
				glNormal3f(n.x, n.y, n.z);
				glVertex3f(v[i].x, v[i].y, v[i].z);
				glVertex3f(v[i+1].x, v[i+1].y, v[i+1].z);
				glVertex3f(v[i+2].x, v[i+2].y, v[i+2].z);
			}
		glEnd();
	glEndList();
	glShadeModel(GL_SMOOTH);
}

void drawCarBody()
{
	vertex v[] = {
		//hood
		vertex(-3.0f,.4f,-1.5f),
		vertex(-3.0f,.4f,1.5f),
		vertex(-1.5f,1.4f,-1.4f),
		vertex(-1.5f,1.4f,1.4f),
		vertex(-1.5f,1.4f,-1.4f),
		vertex(-3.0f,.4f,1.5f),
		//angledbottom
		vertex(-3.0f,.4f,-1.5f),
		vertex(-2.25f,0,-1.5f),
		vertex(-2.25f,0,1.4f),
		vertex(-2.25f,0,1.4f),
		vertex(-3.0f,.4f,1.5f),
		vertex(-3.0f,.4f,-1.5f),
		
		//left
		vertex(-3.0f,.4f,1.5f),
		vertex(-1.5f,1.4f,1.4f),
		vertex(-1.0f,.3f,1.5f),
		vertex(-1.0f,.3f,1.5f),
		vertex(-3.0f,.4f,1.5f),
		vertex(-2.25f,0,1.4f),
		vertex(-1.0f,.3f,1.5f),
		vertex(2.5f,.3f,1.5f),
		vertex(2.5f,0,1.4f),
		vertex(2.5f,0,1.4f),
		vertex(-1.0f,.3f,1.5f),
		vertex(-2.25f,0,1.4f),
		
		//right
		vertex(-3.0f,.4f,-1.5f),
		vertex(-1.5f,1.4f,-1.4f),
		vertex(-1.0f,.3f,-1.5f),
		vertex(-1.0f,.3f,-1.5f),
		vertex(-3.0f,.4f,-1.5f),
		vertex(-2.25f,0,-1.4f),
		vertex(-1.0f,.3f,-1.5f),
		vertex(2.5f,.3f,-1.5f),
		vertex(2.5f,0,-1.4f),
		vertex(2.5f,0,-1.4f),
		vertex(-1.0f,.3f,-1.5f),
		vertex(-2.25f,0,-1.4f),
		
		//inside
		vertex(-1.5f,1.4f,1.4f),
		vertex(-1.5f,1.4f,-1.4f),
		vertex(-2.25f,0,-1.4f),
		vertex(-2.25f,0,-1.4f),
		vertex(-2.25f,0,1.4f),
		vertex(-1.5f,1.4f,1.4f),
		
		//floor
		vertex(-2.25f,0,-1.4f),
		vertex(-2.25f,0,1.4f),
		vertex(2.5f,0,1.4f),
		vertex(2.5f,0,1.4f),
		vertex(2.5f,0,-1.4f),
		vertex(-2.25f,0,-1.4f),
		
		vertex(-2.25f,0.15f,-1.4f),
		vertex(-2.25f,0.15f,1.4f),
		vertex(2.5f,0.15f,1.4f),
		vertex(2.5f,0.15f,1.4f),
		vertex(2.5f,0.15f,-1.4f),
		vertex(-2.25f,0.15f,-1.4f),
	};
	carBody = glGenLists(1);
	glNewList(carBody, GL_COMPILE);
		glBegin(GL_TRIANGLES);
			for(unsigned int i = 0; i < sizeof(v)/sizeof(v[0]); i += 3) {
				vertex n = cross(v[i], v[i+1], v[i+2]);
				glNormal3f(n.x, n.y, n.z);
				glVertex3f(v[i].x, v[i].y, v[i].z);
				glVertex3f(v[i+1].x, v[i+1].y, v[i+1].z);
				glVertex3f(v[i+2].x, v[i+2].y, v[i+2].z);
			}
		glEnd();
	glEndList();
}

void drawCarSide()
{
	vertex v[] = {
		vertex(-1.0f,.5f,1.5f),
		vertex(-1.2f,0,2.0f),
		vertex(1.6f,.5f,1.5f),
		vertex(1.6f,0,2.4f),
		vertex(1.6f,.5f,1.5f),
		vertex(-1.2f,0,2.0f),
		
		vertex(-1.0f,.5f,-1.5f),
		vertex(-1.2f,0,-2.0f),
		vertex(1.6f,.5f,-1.5f),
		vertex(1.6f,0,-2.4f),
		vertex(1.6f,.5f,-1.5f),
		vertex(-1.2f,0,-2.0f),
	};
	carSide = glGenLists(1);
	glNewList(carSide, GL_COMPILE);
		glBegin(GL_TRIANGLES);
			for(unsigned int i = 0; i < sizeof(v)/sizeof(v[0]); i += 3) {
				vertex n = cross(v[i], v[i+1], v[i+2]);
				glNormal3f(n.x, n.y, n.z);
				glVertex3f(v[i].x, v[i].y, v[i].z);
				glVertex3f(v[i+1].x, v[i+1].y, v[i+1].z);
				glVertex3f(v[i+2].x, v[i+2].y, v[i+2].z);
			}
		glEnd();
	glEndList();
}

void drawCarEngine()
{
	carEngine = glGenLists(1);
	glNewList(carEngine,GL_COMPILE);
		glPushMatrix();
			glTranslatef(2.0,1.0,0);
			glScalef(0.8, 0.6, 1.0);
			glutSolidCube(2.0);
		glPopMatrix();
	glEndList();
}

void drawSteeringWheel()
{

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
		gluQuadricNormals(cap, GLU_SMOOTH);
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
		case ' ':
			CameraMode = !CameraMode;
			break;
		case 27:
			exit(0);
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
	glTranslatef(4.0f, 0.0f, CameraZ-5.0f);
	glRotatef(WorldXAngle, 0.0, 1.0, 0.0);
	glRotatef(WorldYAngle, 1.0, 0.0, 0.0);
	
	if(!CameraMode) {
		gluLookAt(0, 0, 0, 0,0, -1,0,1,0);
	} else {
		GLfloat xp, zp, xd, zd, rad = CarRotation / 180.0 * M_PI;
		GLfloat r = sqrt(pow(CarPos[0]-1.0f,2) + pow(CarPos[2],2));
		xp = cos(rad) * r;
		zp = sin(rad) * r;
		xd = CarPos[0];
		zd = CarPos[2];
		gluLookAt(xp, CarPos[1]+2.0f, zp, xd, CarPos[1], zd, 0, 1, 0);
	}

	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
		glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex3f(-50, -1, -50);
			
			glTexCoord2d(0, 1);
			glVertex3f(-50, -1, 50);
			
			glTexCoord2d(1, 1);
			glVertex3f(50, -1, 50);
			
			glTexCoord2d(1, 0);
			glVertex3f(50, -1, -0);
		glEnd();
	glDisable(GL_TEXTURE_2D);
	
	glPushMatrix();
		glutSolidSphere(0.1, 20, 20);
	glPopMatrix();
	
	glPushMatrix();
		glutSolidCube(0.2);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(CarPos[0]-4.0, CarPos[1], CarPos[2]);
		glRotatef(CarRotation, 0, 1, 0);
		glPushMatrix();
			glTranslatef(-2.5, 0, 2.0);
			glRotatef(-TireRotation, 0, 1, 0);
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
			glRotatef(TireRotation, 0.0, 1, 0);
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
			glScalef(0.3, 1.0, 1.0);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			glTranslatef(1.0, 0.2, -12.0);
			glCallList(axle);
		glPopMatrix();
		
		glPushMatrix();
			glScalef(0.3, 1.0, 1.0);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			glTranslatef(-1.0, 0.2, -12.0);
			glCallList(axle);
		glPopMatrix();
	
		glPushMatrix();
			setMaterial(0.8,0.8,0.8,40);
			glTranslatef(-3.5, 0.0, 0.0);
			glScalef(4.0, 4.0, 4.0);
			glCallList(carFront);
		glPopMatrix();
		
		glPushMatrix();
			setMaterial(0.8,0,0,50);
			glCallList(carBody);
		glPopMatrix();
		
		glPushMatrix();
			setMaterial(0.8,0.8,0.8,40);
			glCallList(carSide);
		glPopMatrix();
		
		glPushMatrix();
			setMaterial(0.4,0.4,0.4,30);
			glCallList(carEngine);
		glPopMatrix();
		
		glPushMatrix();
			glScalef(1.2, 0.3, 1.2);
			glRotatef(90.0, 1.0, 0, 0);
			glTranslatef(2.1, 0.8, -8.0);
			glCallList(axle);
		glPopMatrix();
		
		glPushMatrix();
			
		glPopMatrix();
	glPopMatrix();
	
	if(!cruisectrl) {
		if(accel) {
			accelRate = min(20.0f, accelRate + 0.1f);
		} else if (!backaccel && accelRate > 0.0f) {
			//TireRotateSpeed = max(0.0f, TireRotateSpeed - 0.2f);
			accelRate = max(0.0f, accelRate - 0.05f);
		} else if (backaccel) {
			accelRate = max(-20.0f, accelRate - 0.1f);
		} else if (!accel && accelRate < 0.0f) {
			accelRate = min(0.0f, accelRate + 0.05f);
		}
	}
	
	if(turnRight) {
		CarRotation -= accelRate/20;
		TireRotation = min(25.0f, TireRotation + 2.0f);
	} else if(turnLeft) {
		CarRotation += accelRate/20;
		TireRotation = max(-25.0f, TireRotation - 2.0f);
	} else {
		if(TireRotation < 0.0f)
			TireRotation = min(0.0f, TireRotation + 2.0f);
		else
			TireRotation = max(0.0f, TireRotation - 2.0f);
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
	drawCarBody();
	drawCarSide();
	drawCarEngine();
}

Mat cv_readImage(const char *path){
	Mat image = imread(path, CV_LOAD_IMAGE_COLOR);
	if(!image.data){
		cout << "Fail to load image\n";
		return Mat();
	}
	return image.clone();
}

GLuint setupTexture(unsigned char *image, int width, int height){
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		width,
		height,
		0,
		GL_BGR,
		GL_UNSIGNED_BYTE,
		image
	);
	// Bind target back to its default.
	glBindTexture(GL_TEXTURE_2D, 0);
	return tex;
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
	
	Mat image = cv_readImage("dry_grass.jpg");
	g_texID = setupTexture(image.ptr(), image.cols, image.rows);
	glBindTexture(GL_TEXTURE_2D, g_texID);
	
	glutMainLoop();
	
	return 0;
}
