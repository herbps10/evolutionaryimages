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
  return (float)random() / (float)RAND_MAX;
}


Image::Image()
{
};


void Image::load_from_file(char *image_path)
{
  CImg<float> image(image_path);

  
  // Allocate space to the target image buffer
  image_buffer = (float *) malloc(sizeof(float *) * image.width() * image.height() );

  // Copy the CImg data over into the buffer
  for(int x = 0; x < image.width(); x++)
  {
    for(int y = 0; y < image.height(); y++)
    {
      image_buffer[x * image.width() + y] = image(x, y);
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
  Image target;
  target.load_from_file( (char*)"target-image.jpg" );

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
  
  // set up a new frambuffer
  // *** borrowed from http://www.songho.ca/opengl/gl_fbo.html **

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
  
  // switch back to window-system-provided framebuffer
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // *** END from songho.ca ***

  glutTimerFunc(2000, beginGeneticAlgorithm, 1); // start beginGeneticAlgorithm almost immediately 
  glutMainLoop();	// start rendering


  //test->print();

  return 0;
}
