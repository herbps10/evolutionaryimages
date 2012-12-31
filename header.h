#ifndef OUR_HEADER
#define OUR_HEADER

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

class Image
{
  //I moved public: to the top for debugging purposes
  public:
  float *image_buffer;
  float *polygon_buffer;
  polygon polygons[MAX_POLYGONS];
  // private error calculating functions to be called by fitness
  double sumOfError(float *buffer, int height, int width); // compares buffer to image_buffer and sums the error
  double sumOfSquaresError(float *buffer, int height, int width); // compares buffer to image_buffer and sums the square of the error

  //public:
    Image();
    void load_from_file(char *);
    void randomize_polygons();
    void print();
    void render();
    void fitness(Image);
    void mutate();
    Image* recombine(Image);
};

//function declarations from draw_polygons.cpp
//void timer_callback(int value);
void display(void);
void init(void);

//function declarations for algorithm.cpp
void beginGeneticAlgorithm();


#endif
