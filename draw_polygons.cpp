// Stephen Kowalewski

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(__MACOSX__) || defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define DEFAULT_WIDTH 500
#define DEFAULT_HEIGHT 500
#define MAX_POINTS 6 //maximum points per polygon
#define MAX_POLYGONS 5

typedef struct vertex3f {
    float x;
    float y;
    float z; // not particularly relevant for our purposes, but must be there
} vertex3f;

typedef struct color4f {
    float r;
    float g;
    float b;
    float a; // alpha
} color4f;

typedef struct polygon {
    int num_p; // number of points (cannot exceed MAX_POINTS)
    color4f color;
    vertex3f points[MAX_POINTS];
} polygon;

int num_polygons = 0;
polygon polygons[MAX_POLYGONS];

void display(void){
    int i,j;

	//clears the pixels
	glClear(GL_COLOR_BUFFER_BIT);

	
	//draw a polygon	
    for(i=0;i<num_polygons;i++) {
        glColor4f(polygons[i].color.r, polygons[i].color.g, polygons[i].color.b, polygons[i].color.g);
        printf("%f %f %f %f\n", polygons[i].color.r, polygons[i].color.g, polygons[i].color.b, polygons[i].color.g);
        glBegin(GL_POLYGON);
            for(j=0;j<polygons[i].num_p;j++) {
                glVertex3f(polygons[i].points[j].x, polygons[i].points[j].y, polygons[i].points[j].z);
                printf("    %f, %f, %f\n", polygons[i].points[j].x, polygons[i].points[j].y, polygons[i].points[j].z);
            }
        glEnd();
    }
    

//	glColor3f(0.1, 0.3, 1.0);
//	glBegin(GL_POLYGON);
//		glVertex3f(0.20, 0.33, 0.0);
//		glVertex3f(0.35, 0.67, 0.0);
//		glVertex3f(0.65, 0.67, 0.0);
//		glVertex3f(0.80, 0.33, 0.0);
//	glEnd(); 

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
    int i;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

    srand(time(NULL));

    //create 3 polygons
    for(i=0;i<3;i++) {
        polygons[i].num_p = 4;
        polygons[i].points[0].x = (float)rand()/(float)RAND_MAX;
        polygons[i].points[0].y = (float)rand()/(float)RAND_MAX;
        polygons[i].points[0].z = 0.0f;
        polygons[i].points[1].x = (float)rand()/(float)RAND_MAX;
        polygons[i].points[1].y = (float)rand()/(float)RAND_MAX;
        polygons[i].points[1].z = 0.0f;
        polygons[i].points[2].x = (float)rand()/(float)RAND_MAX;
        polygons[i].points[2].y = (float)rand()/(float)RAND_MAX;
        polygons[i].points[2].z = 0.0f;
        polygons[i].points[3].x = (float)rand()/(float)RAND_MAX;
        polygons[i].points[3].y = (float)rand()/(float)RAND_MAX;
        polygons[i].points[3].z = 0.0f;
        polygons[i].color.r = (float)rand()/(float)RAND_MAX;
        polygons[i].color.g = (float)rand()/(float)RAND_MAX;
        polygons[i].color.b = (float)rand()/(float)RAND_MAX;
        polygons[i].color.a = (float)rand()/(float)RAND_MAX;

        num_polygons++;
    }   


	glutInitWindowPosition(100, 100);
	glutCreateWindow("hello");
	init();
	glutDisplayFunc(display);
	glutMainLoop();	// start rendering

	return 0;	
}
