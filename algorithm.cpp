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

float rand_one()
{
  return (float)random() / (float)RAND_MAX;
}


Image::Image()
{
};


void Image::load_from_file(char *image_path)
{
  CImg<float> image(image_path);

  
  // Allocate space to the target image buffer
  pixel_buffer = (float **) malloc(sizeof(float *) * image.width());
  for(int i = 0; i < image.width(); i++)
    pixel_buffer[i] = (float *) malloc(sizeof(float) * image.height());
  

  // Copy the CImg data over into the buffer
  for(int x = 0; x < image.width(); x++)
  {
    for(int y = 0; y < image.height(); y++)
    {
      pixel_buffer[x][y] = image(x, y);
    }
  } 
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
    printf("render called\n");
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

}

int main(int argc, char** argv)
{ 
  //misc setup
  srandom((unsigned int)time(NULL));

  // set up CImg stuff
  Image target;
  target.load_from_file((char*)"target-image.jpg");

  test = new Image();
  test->randomize_polygons();

  // set up OpenGL
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

  glutInitWindowPosition(100, 100);
  glutCreateWindow("hello");
  init();
  glutDisplayFunc(display);
  glutTimerFunc(10, beginGeneticAlgorithm, 1); // start beginGeneticAlgorithm almost immediately 
  glutMainLoop();	// start rendering


  //test->print();

  return 0;
}
