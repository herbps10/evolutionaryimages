// Stephen Kowalewski

#include <stdio.h>
#include <stdlib.h>

#if defined(__MACOSX__) || defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define DEFAULT_WIDTH 500
#define DEFAULT_HEIGHT 500

void display(void){

	//clears the pixels
	glClear(GL_COLOR_BUFFER_BIT);

	
	//draw a polygon	
	glColor3f(0.1, 0.3, 1.0);
	glBegin(GL_POLYGON);
		glVertex3f(0.20, 0.33, 0.0);
		glVertex3f(0.35, 0.67, 0.0);
		glVertex3f(0.65, 0.67, 0.0);
		glVertex3f(0.80, 0.33, 0.0);
	glEnd(); 

	//process routines
	glFlush();
	
} //end display


void init(void){
	
	//set color for when display is cleared
	glClearColor(0.0, 0.0, 0.0, 0.0);

	//initial vieing values
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	
}


int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hello");
	init();
	glutDisplayFunc(display);
	glutMainLoop();	// start rendering

	return 0;	
}
