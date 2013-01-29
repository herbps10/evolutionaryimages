// Herb Susmann and Stephen Kowalewski

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "CImg.h"

#include "image.h"
#include "image.c"

#include "header.h"

using namespace cimg_library;
using namespace std;

Image *test = NULL; //initialized in main(), must be global for display() to see it
GLuint rboId;
GLuint fboId;


float rand_one()
{
#if defined(__MACOSX__) || defined(__APPLE__)
  return (float)((double)arc4random() / (double)4294967295); // largest value is 2^32-1 (roughly double RAND_MAX)
#else
  return (float)random() / (float)RAND_MAX;
#endif
}

void beginGeneticAlgorithm(int value)
{
    if(value!=1) return;
    printf("This was executed\n");
    sleep(1); // so I can see the changes. 
    test->randomize_polygons();
    display();

}

int main(int argc, char** argv)
{ 
  //misc setup
  srandom((unsigned int)time(NULL));

  // set up CImg stuff
  //Image target;
  //target.load_from_file( (char*)"target-image.jpg" );

  test = new Image();
  test->load_from_file( (char*)"test.gif" );
  //test->randomize_polygons();

  // set up OpenGL
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

  glutInitWindowPosition(100, 100);
  glutCreateWindow("hello");
  init();
  glutDisplayFunc(display);

   
  // switch back to window-system-provided framebuffer
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // *** END from songho.ca ***

  glutTimerFunc(20, beginGeneticAlgorithm, 1); // start beginGeneticAlgorithm almost immediately 
  glutMainLoop();	// start rendering


  //test->print();

  return 0;
}
