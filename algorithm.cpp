// Herb Susmann and Stephen Kowalewski

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "CImg.h"

#include "header.h"

using namespace cimg_library;
using namespace std;

float rand_one()
{
  return (float)rand() / (float)RAND_MAX;
}

class Image
{
  float **pixel_buffer;
  polygon polygons[MAX_POLYGONS];

  public:
    Image();
    void load_from_file(char *);
    void randomize_polygons();
    void print();
    void render();
    void fitness(Image);
    void mutate();
    Image recombine(Image);
};

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

void Image::render()
{
  
}

void Image::fitness(Image target)
{

}

void Image::mutate()
{

}

Image Image::recombine(Image second)
{

}

int main()
{
  Image target;
  target.load_from_file("target-image.jpg");

  Image *test = new Image();
  test->randomize_polygons();

  test->print();

  return 0;
}
