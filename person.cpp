/******************************************************************************
*
* Official Name:  Farhana Mustafa
*
* E-mail:  fmustafa@syr.edu
*
* Assignment:  Homework 1
*
* Environment/Compiler:  Visual Studio 2015
*
* Date:  February 5, 2018
*
* References:  none
*
* Interactions:   Press the escape key to quit.
*				  Press p/P to change from ortho view to perspective view.
*				  Press the spacebar to toggle between wireframe and filled.
*
******************************************************************************/

#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

// Globals.
static float R = 1.0; // Radius of head.
static int v = 6; // Number of longitudinal (verticle) slices.
static int h = 4; // Number of latitudinal (horizontal) slices 
//on hemisphere.

static float cx=0.0, cy=0.0, cz=-10.0;  //center of sphere
static float bodylength=4.0;
static float armlength=4.0;
static float leglength=6.0;
static float armangle= -PI/18.0;
static float cameraArm = PI / 9;
static float legangle=-(4.0*PI/10.0);
static long font = (long)GLUT_BITMAP_8_BY_13; //Fonts
static float view = 0.0;

static int isWire = 0; // Is wireframe?
static int stippleID = 0; // Stipple ID.

static float vertices[] =
{
	-5.0, 2.0, 0.0,
	4.0, 2.0, 0.0,
	4.0, 7.0, 0.0,
	-5.0, 7.0, 0.0,
	-5.0, 2.0, 0.0
};

//color array
static float colors[] =
{
	0.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0
};
											  // Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{
	char *c;

	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void drawHead(float cx, float cy, float cz)
{
  int i, j;
  // Based on hemisphere.cpp
  // cx, cy,cz is the center of the sphere, R is the radius.
  // This is the head
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3ub(rand() % 256, rand() % 256, rand() % 256);
  
  for(j = 0; j < h; j++)
  {
    // One latitudinal triangle strip. top half
    glBegin(GL_TRIANGLE_STRIP);
    for(i = 0; i <= v; i++)
    {
      glVertex3f( R * cos( (float)(j+1)/h * PI/2.0 ) * cos( 2.0 * (float)i/v * PI )+cx,
                 R * sin( (float)(j+1)/h * PI/2.0 )+cy,
                 R * cos( (float)(j+1)/h * PI/2.0 ) * sin( 2.0 * (float)i/v * PI )+cz );
      glVertex3f( R * cos( (float)j/h * PI/2.0 ) * cos( 2.0 * (float)i/v * PI  )+cx,
                 R * sin( (float)j/h * PI/2.0  )+cy,
                 R * cos( (float)j/h * PI/2.0 ) * sin( 2.0 * (float)i/v * PI)+cz );         
    }
    glEnd();
    // One latitudinal triangle strip. bottom half
    glBegin(GL_TRIANGLE_STRIP);
    for(i = 0; i <= v; i++)
    {
      glVertex3f( R * cos( (float)(j+1)/h * PI/2.0 ) * cos( 2.0 * (float)i/v * PI )+cx,
                 -1*R * sin( (float)(j+1)/h * PI/2.0 )+cy,
                 R * cos( (float)(j+1)/h * PI/2.0 ) * sin( 2.0 * (float)i/v * PI )+cz );
      glVertex3f( R * cos( (float)j/h * PI/2.0 ) * cos( 2.0 * (float)i/v * PI  )+cx,
                 -1*R * sin( (float)j/h * PI/2.0  )+cy,
                 R * cos( (float)j/h * PI/2.0 ) * sin( 2.0 * (float)i/v * PI)+cz );         
    }
    glEnd();
  }
}


void drawStickBody(float cx, float cy, float cz)
{
	float t = 0.0;
	if (isWire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3ub(rand() % 256, rand() % 256, rand() % 256);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 10; i++) {
		glVertex3f(cx + 0.5 * cos(t), cy + 0.5 * sin(t) - bodylength - 1, -15.0);
		t += 2 * PI / 10;
		glVertex3f(cx + 0.5 * cos(t), cy + 0.5 * sin(t), -15.0);
		
	}
	glEnd();
}

void drawStickArms(float cx, float cy, float cz, float armangle)
{
  glLineWidth(5.0);
  glColor3ub(rand() % 256, rand() % 256, rand() % 256);
  glBegin(GL_LINES);
  glVertex3f(cx,cy-R-.2*bodylength,cz); //down 20% of body
  glVertex3f(cx+armlength*cos(armangle),
             cy-R-.2*bodylength+armlength*sin(armangle),cz); 
  glVertex3f(cx,cy-R-.2*bodylength,cz); //down 20% of body
  glVertex3f(cx-armlength*cos(armangle),
             cy-R-.2*bodylength+armlength*sin(armangle),cz); 
  glEnd();
  glLineWidth(1.0);
  glColor3f(0.0,0.0,0.0);
}

void drawStickLegs(float cx, float cy, float cz)
{
  glLineWidth(5.0);
  glColor3ub(rand() % 256, rand() % 256, rand() % 256);
  glBegin(GL_LINES);
  glVertex3f(cx,cy-R-bodylength,cz); //down at bottom of body
  glVertex3f(cx+leglength*cos(legangle),
    cy-R-bodylength+leglength*sin(legangle),cz);//
  glVertex3f(cx,cy-R-bodylength,cz); //down at bottom of body
  glVertex3f(cx-leglength*cos(legangle),
   cy-R-bodylength+leglength*sin(legangle),cz);//
  glEnd();
  glLineWidth(1.0);
  glColor3f(0.0,0.0,0.0);
}

void person1() 
{
	drawStickLegs(-15.0, 0.0, -5.0);
	drawStickArms(-15.0, 0.0, -5.0, armangle);
	drawStickBody(-15.0, 0.0, -5.0);
	drawHead(-15.0, 0.0, -5.0);
}

void person2()
{
	drawStickLegs(-11.0, 1.0, -5.0);
	drawStickArms(-11.0, 1.0, -5.0, armangle);
	drawStickBody(-11.0, 1.0, -5.0);
	drawHead(-11.0, 1.0, -5.0);
}

void person3()
{
	drawStickLegs(11.0, 1.0, -5.0);
	drawStickArms(11.0, 1.0, -5.0, armangle);
	drawStickBody(11.0, 1.0, -5.0);
	drawHead(11.0, 1.0, -5.0);
}

void person4()
{
	drawStickLegs(-3.0, -7.0, -5.0);
	drawStickArms(-3.0, -7.0, -5.0, armangle);
	drawStickBody(-3.0, -7.0, -5.0);
	drawHead(-3.0, -7.0, -5.0);
}

void person5()
{
	drawStickLegs(2.0, -7.0, -5.0);
	drawStickArms(2.0, -7.0, -5.0, armangle);
	drawStickBody(2.0, -7.0, -5.0);
	drawHead(2.0, -7.0, -5.0);
}

void person6()
{
	drawStickLegs(-12.0, -10.0, -10.0);
	drawStickArms(-12.0, -10.0, -10.0, cameraArm);
	drawStickBody(-12.0, -10.0, -10.0);
	drawHead(-12.0, -10.0, -10.0);
}

void drawTV() {
	if (isWire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_POLYGON);
		for (int i = 0; i < 5; ++i) glArrayElement(i % 8);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_POLYGON);
		glVertex3f(-4.5, 2.5, 0.0);
		glVertex3f(3.5, 2.5, 0.0);
		glVertex3f(3.5, 6.5, 0.0);
		glVertex3f(-4.5, 6.5, 0.0);
	glEnd();
}

void createNBAScene() {
	//basketball
	float t = 0;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_POLYGON);
	for (int i = 0; i < 10; ++i) {
		glColor3f(1.0, 0.5, 0.0);
		glVertex3f(1 + 1.5 * cos(t), 4.5 + 1.5 * sin(t), -10.0);
		t += 2 * PI / 10;
	}
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	//Stipple
	glEnable(GL_LINE_STIPPLE); // Enable line stippling.
	glRasterPos3f(30.0, 40.0, 0.0);
	//glLineStipple(1, 0x00FF);
	glLineStipple(5, 0x5555);

	glBegin(GL_LINES);
	glVertex3f(1.0, 3.1, -11.0);
	glVertex3f(1.0, 5.9, -11.0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(-0.5, 4.4, -11.0);
	glVertex3f(2.6, 4.4, -11.0);
	glEnd();

	glDisable(GL_LINE_STIPPLE); // Disable line stippling.
}

void ceilingFan() {

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(-10.0, 8.0, 0.0);
	glVertex3f(-8.0, 8.0, 0.0);
	glVertex3f(-9.0, 13.0, 0.0);
	glVertex3f(-10.0, 8.0, 0.0);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(-9.0, 13.0, 0.0);
	glVertex3f(-4.0, 14.5, 0.0);
	glVertex3f(-4.0, 12.5, 0.0);
	glVertex3f(-9.0, 13.0, 0.0);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(-9.0, 13.0, 0.0);
	glVertex3f(-14.0, 14.5, 0.0);
	glVertex3f(-14.0, 12.5, 0.0);
	glVertex3f(-9.0, 13.0, 0.0);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(-9.0, 13.0, 0.0);
	glVertex3f(-10.0, 18.0, 0.0);
	glVertex3f(-8.0, 18.0, 0.0);
	glVertex3f(-9.0, 13.0, 0.0);
	glEnd();

	//light
	float t = 0;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_POLYGON);
	for (int i = 0; i < 10; ++i) {
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(-9 + 0.5 * cos(t), 13 + 0.5 * sin(t), -10.0);
		t += 2 * PI / 10;
	}
	glEnd();
}

// Drawing routine.
void drawScene(void)
{
  glClear (GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (view)
	  glFrustum(-16.0, 16.0, -20.0, 20.0, 7.0, 11.0);
  else glOrtho(-20.0, 20.0, -20.0, 20.0, 0.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  person1();
  person2();
  person3();
  person4();
  person5();
  person6();
  drawTV();
  createNBAScene();
  ceilingFan();

  glColor3f(1.0, 0.3, 0.7);
  glRasterPos3f(-4.0, 19.0, 0.0);
  writeBitmapString((void*)font, "My Roommates");

  glColor3f(1.0, 0.3, 0.7);
  glRasterPos3f(10.0, -19.0, 0.0);
  writeBitmapString((void*)font, "Farhana Mustafa");

  glutSwapBuffers();
}

// Initialization routine.
void setup(void) 
{
  //white background
  glClearColor(1.0, 1.0, 1.0, 0.0); 

  // Enable two vertex arrays: co-ordinates and color.
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  // Specify locations for the co-ordinates and color arrays.
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glColorPointer(3, GL_FLOAT, 0, colors);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (view)
		glFrustum(-16.0, 16.0, -20.0, 20.0, 7.0, 11.0);
	else glOrtho(-20.0, 20.0, -20.0, 20.0, 0.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
  switch(key) 
  {
    case 27:
      exit(0);
      break;
    case 'p':
	  view = 1;
      glutPostRedisplay();
      break;
    case 'P':
	  view = 0;
      glutPostRedisplay();
      break;
	case ' ':
		if (isWire == 0) isWire = 1;
		else isWire = 0;
		glutPostRedisplay();
		break;
    default:
      break;
  }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
  cout << "Interaction:" << endl;
  cout << "Press p/P to change from ortho view to perspective view" << endl
	  << "Press the spacebar to toggle between wireframe and filled." << endl
	  << "Press Esc to exit the program." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
  printInteraction();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("A Person");
  setup(); 
  glutDisplayFunc(drawScene); 
  glutReshapeFunc(resize);  
  glutKeyboardFunc(keyInput);
  glutMainLoop(); 
  
  return 0;  
}

