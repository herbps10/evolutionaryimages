// Herb Susmann and Stephen Kowalewski

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"


extern Image *test; //initialized in main(), must be global for display() to see it
extern GLuint fboId;

//int num_polygons = 0;
//polygon polygons[MAX_POLYGONS];
int timer_value = 0;

//void timer_callback(int value)
//{
//  test->randomize_polygons();
//  timer_value++;
//  glutPostRedisplay();
//
//}


void display(void){
    //int i,j;
    printf("display\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
    test->render();

    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	glClear(GL_COLOR_BUFFER_BIT);
    test->render();

    //the buffer will contain the pixels in each row from left to right starting with the bottom row
    glReadPixels(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT, GL_RGB, GL_FLOAT, test->polygon_buffer);
    
    //print out the buffer to make sure it's correct
//    int offset = 0;
//    for(int z=0;z<DEFAULT_HEIGHT;z++) { // for each row
//        for(int x=0;x<DEFAULT_WIDTH;x++) { // for each pixel in row z
//            //printf("%.3f  ", ( test->polygon_buffer[offset] +  test->polygon_buffer[offset+1] +  test->polygon_buffer[offset+2] ) / 3.0f );
//            printf("(%d,%d) - %.3f %.3f %.3f\n", z, x, test->polygon_buffer[offset], test->polygon_buffer[offset+1], test->polygon_buffer[offset+2]);
//            offset += 3;
//        }
//        putchar('\n');
//        putchar('\n');
//    }

	//process routines
	//glFlush();
	glutSwapBuffers();// Make sure changes appear onscreen

    //glutTimerFunc(1000, timer_callback, timer_value);
    
	
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

    //other stuff
	//glutReshapeFunc(myReshapeFunc);
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
