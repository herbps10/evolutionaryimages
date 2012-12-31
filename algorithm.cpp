// Herb Susmann and Stephen Kowalewski

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "CImg.h"

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

// assumes image_buffer and buffer are the same size
double Image::sumOfError(float *buffer, int height, int width){
    int i;
    double error = 0.0;

    if( height != DEFAULT_HEIGHT || width != DEFAULT_WIDTH ) {
        fprintf(stderr, "sumOfError: buffers are not the same size.\n");
        return -1.0;
    }

    for(i=0;i<DEFAULT_HEIGHT*DEFAULT_WIDTH*3;i++) {
        error += image_buffer[i] < buffer[i] ? buffer[i] - image_buffer[i] : image_buffer[i] - buffer[i];
    }
    
    return error;
}


// assumes image_buffer and buffer are the same size
double Image::sumOfSquaresError(float *buffer, int height, int width){
    int i;
    double error = 0.0, diff;

    if( height != DEFAULT_HEIGHT || width != DEFAULT_WIDTH ) {
        fprintf(stderr, "sumOfSquaresError: buffers are not the same size.\n");
        return -1.0;
    }

    for(i=0;i<DEFAULT_HEIGHT*DEFAULT_WIDTH*3;i++) {
        diff = image_buffer[i] - buffer[i];
        error += diff*diff;
    }
    
    return error;
}


Image::Image()
{
};


void Image::load_from_file(char *image_path)
{
  CImg<float> image(image_path);
  int i = 0;

  // Allocate space to the target image buffer
  image_buffer = (float *) malloc(sizeof(float) * image.width() * image.height() * 3);
  if( image_buffer == NULL )
    fprintf(stderr, "error malloc-ing image_buffer\n");

  // Copy the CImg data over into the buffer
  for(int y = image.height() - 1; y >= 0; y--) // for each row bottom to top (to allign with OpenGl getPixelValues(...))
  {
    for(int x = 0; x < image.width(); x++) // for each pixel in row x
    {
      image_buffer[i++] = image(x, y, 0, 0); // red component
      image_buffer[i++] = image(x, y, 0, 1); // green component
      image_buffer[i++] = image(x, y, 0, 2); // blue component 
    }
  } 

//  i=0;
//  printf("%d components\n", image.spectrum());
//  for(int j=0;j<3*image.width()*image.height();j+=3) {
//    printf("(%d,%d) - %f %f %f\n", i/image.width(), i%image.width(), image_buffer[j], image_buffer[j+1], image_buffer[j+2] );
//    i++;
//  }
  //image.display();

  // Allocate space for the polygon_buffer
  // NOTE: image_buffer and polygon_buffer may not be the same size. Either we'll have to adjust that here or we'll have to write a fitness function that is aware of this
  // do not change the size of this buffer without making appropriate changes in display()
  polygon_buffer = (float*)malloc(sizeof(float) * DEFAULT_WIDTH * DEFAULT_HEIGHT * 3);
  if( polygon_buffer == NULL )
    fprintf(stderr, "error malloc-ing polygon_buffer\n");

}

void Image::randomize_polygons()
{
  for(int poly_index = 0; poly_index < MAX_POLYGONS; poly_index++)
  {
    int num_points = rand() % (MAX_POINTS - 3) + 3;

    polygons[poly_index].num_p = num_points;

    polygons[poly_index].color.r = rand_one();
    polygons[poly_index].color.g = rand_one();
    polygons[poly_index].color.b = rand_one();
    polygons[poly_index].color.a = rand_one();

    for(int vertex_index = 0; vertex_index < num_points; vertex_index++)
    {
      polygons[poly_index].points[vertex_index].x = rand_one();
      polygons[poly_index].points[vertex_index].y = rand_one();
      polygons[poly_index].points[vertex_index].z = rand_one();
    }
  }
}

void Image::print()
{
  for(int i = 0; i < MAX_POLYGONS; i++)
  {
    cout << "POLYGON" << endl;
    cout << "Color: (" << polygons[i].color.r << "," << polygons[i].color.g << "," << polygons[i].color.b << "," << polygons[i].color.a << ")" << endl;

    cout << "Points" << endl;
    for(int j = 0; j < polygons[i].num_p; j++)
    {
      cout << polygons[i].points[j].x << "," << polygons[i].points[j].y << "," << polygons[i].points[j].z << endl;
    }
  }
}

//called from display()
void Image::render()
{
   // printf("render called\n");
  for(int poly_index = 0; poly_index < MAX_POLYGONS; poly_index++)
  {
    glColor4fv( (GLfloat*) &polygons[poly_index].color);
    //printf("    %f, %f, %f\n", polygons[poly_index].points[poly_index].x, polygons[poly_index].points[poly_index].y, polygons[poly_index].points[poly_index].z);

    glBegin(GL_POLYGON);
      for(int vertex_index = 0; vertex_index < polygons[poly_index].num_p ; vertex_index++)
      {
        glVertex3fv( (GLfloat*) &polygons[poly_index].points[vertex_index]);
      }
    glEnd();
  }
}

void Image::fitness(Image target)
{

}

void Image::mutate()
{

}

Image* Image::recombine(Image second)
{
    return NULL;
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
