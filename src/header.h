#ifndef OUR_HEADER
#define OUR_HEADER

#if defined(__MACOSX__) || defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "CImg.h"

#define ITERATIONS 1000000

#define DEFAULT_WIDTH 250
#define DEFAULT_HEIGHT 250
#define MAX_POINTS 3 //maximum points per polygon
#define MAX_POLYGONS 2000
#define D 2 // 2 for (x,y), 3 for (x,y,z)

#define POPULATION_SIZE 80

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

typedef struct vertex2f {
	float x;
	float y;
} vertex2f;

typedef struct polygon {
    color4f color;
    vertex2f points[MAX_POINTS];
    int num_p; // number of points (cannot exceed MAX_POINTS)
} polygon;

//function declarations from draw_polygons.cpp
//void timer_callback(int value);
void display(void);
void init(void);

//function declarations for algorithm.cpp
void beginGeneticAlgorithm();


#endif
