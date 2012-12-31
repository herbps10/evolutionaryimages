// Herb Susmann and Stephen Kowalewski

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"


extern Image *test; //initialized in main(), must be global for display() to see it
extern GLuint fboId;
extern GLuint rboId;
bool firstCallToDisplay = true; //used to prevent display() from being called at start up


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
    if(firstCallToDisplay){
        firstCallToDisplay = false;
        return;
    }

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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // set up a new frambuffer
    // *** borrowed from http://www.songho.ca/opengl/gl_fbo.html **
  
     // create a texture object
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, DEFAULT_WIDTH, DEFAULT_HEIGHT, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // create a renderbuffer object to store depth info
    glGenRenderbuffers(1, &rboId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                          DEFAULT_WIDTH, DEFAULT_HEIGHT );
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    // create a framebuffer object
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    
    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, textureId, 0);
    
    // attach the renderbuffer to depth attachment point
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, rboId);
    
    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
        fprintf(stderr,"problem creating framebuffer object\n");
  



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
