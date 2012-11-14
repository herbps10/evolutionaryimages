// Herb Susmann and Stephen Kowalewski

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"

#if defined(__MACOSX__) || defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


int num_polygons = 0;
polygon polygons[MAX_POLYGONS];

void display(void){
    int i,j;

	//clears the pixels
	glClear(GL_COLOR_BUFFER_BIT);

	
	//draw a polygon	
    for(i=0;i<num_polygons;i++) {
        glColor4fv( (GLfloat*) &polygons[i].color);
        //printf("%f %f %f %f\n", polygons[i].color.r, polygons[i].color.g, polygons[i].color.b, polygons[i].color.a);
        glBegin(GL_POLYGON);
            for(j=0;j<polygons[i].num_p;j++) {
                glVertex3fv( (GLfloat*) &polygons[i].points[j]);
                //printf("    %f, %f, %f\n", polygons[i].points[j].x, polygons[i].points[j].y, polygons[i].points[j].z);
            }
        glEnd();
    }
    

	//process routines
	glFlush();
	
} //end display


void init(void){
	
	//set color for when display is cleared
	glClearColor(0.0, 0.0, 0.0, 0.0);

	//initial vieing values
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 2.0f); // top left is (0,0) displays depths between -1 and 2
    glEnable(GL_BLEND); //required for transperency
    glBlendFunc(GL_ONE,GL_ONE);
	
}

//
//int main(int argc, char** argv){
//    int i;
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//	glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
//
//    srand(time(NULL));
//
//    //create 3 polygons
//    for(i=0;i<3;i++) {
//        polygons[i].num_p = 3;
//        polygons[i].points[0].x = (float)rand()/(float)RAND_MAX;
//        polygons[i].points[0].y = (float)rand()/(float)RAND_MAX;
//        polygons[i].points[0].z = (float)i/10.0f;
//        polygons[i].points[1].x = (float)rand()/(float)RAND_MAX;
//        polygons[i].points[1].y = (float)rand()/(float)RAND_MAX;
//        polygons[i].points[1].z = (float)i/10.0f;
//        polygons[i].points[2].x = (float)rand()/(float)RAND_MAX;
//        polygons[i].points[2].y = (float)rand()/(float)RAND_MAX;
//        polygons[i].points[2].z = (float)i/10.0f;
//        polygons[i].points[3].x = (float)rand()/(float)RAND_MAX;
//        polygons[i].points[3].y = (float)rand()/(float)RAND_MAX;
//        polygons[i].points[3].z = (float)i/10.0f;
//        polygons[i].color.r = (float)rand()/(float)RAND_MAX;
//        polygons[i].color.g = (float)rand()/(float)RAND_MAX;
//        polygons[i].color.b = (float)rand()/(float)RAND_MAX;
//        polygons[i].color.a = (float)rand()/(float)RAND_MAX;
//
//        num_polygons++;
//    }   
//
//
//	glutInitWindowPosition(100, 100);
//	glutCreateWindow("hello");
//	init();
//	glutDisplayFunc(display);
//	glutMainLoop();	// start rendering
//
//	return 0;	
//}
